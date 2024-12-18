//
// Created by matis on 23/10/2024.
//

#include "AwaleGame.h"
#include <iostream>
#include <limits>

using namespace std;

AwaleGame::AwaleGame(int gamemode, int playerToBegin): _gamemode(gamemode), _playerBegin(playerToBegin), _scorePlayer1(0), _scorePlayer2(0), _currentPlayer(1), _totalSeedsOnBoard(64) {
    if (gamemode < 1 || gamemode > 2) {
        cerr << "Gamemode invalide. Utilisez 1 pour singleplayer ou 2 pour multiplayer." << endl;
        exit(EXIT_FAILURE);
    }

    if (playerToBegin < 1 || playerToBegin > 2) {
        cerr << "PlayerBegin invalide. Utilisez '1' pour joueur 1(humain) ou '2' pour joueur 2(IA)." << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 16; ++i) {
        _board[i].red = 2;
        _board[i].blue = 2;
    }
}

void AwaleGame::displayBoard() {
    cout << "\n";
    // Affichage indice case
    for (int i = 0; i < 8; ++i) {
        cout << "   " << i+1 << "    ";
    }
    cout << "\n";

    // Affichage rangée supérieure
    cout << " ";
    for (int i = 0; i < 8; ++i) {
        cout << _board[i].red << "R ";
        cout << _board[i].blue << "B";
        if (i < 7) cout << " | ";
    }
    cout << "\n";

    // Affichage des scores
    cout << "-------+-------+-------+-------+-------+-------+-------+-------" << "\n";

    // Affichage de la rangée inférieure
    cout << " ";
    for (int i = 15; i > 7; --i) {
        cout << _board[i].red << "R ";
        cout << _board[i].blue << "B";
        if (i > 8) cout << " | ";
    }
    cout << "\n";

    // Affichage indice case
    for (int i = 15; i > 7; --i) {
        cout << "   " << i+1 << "   ";
    }
    cout << "\n\n";
}

void AwaleGame::playGame() {
    if (_gamemode == 1) {
        cout << "Mode singleplayer sélectionné." << endl;
        if (_playerBegin == 2) {
            switchPlayer(); // L'IA commence
        }
    } else {
        cout << "Mode multiplayer sélectionné." << endl;
    }

    do {
        displayBoard();
        displayScores();

        if (_currentPlayer == 1) {
            cout << "Joueur 1, choisissez un trou pair (2,4,...,16) et une couleur (B-R) :";
            string choice;
            cin >> choice;
            if (!makeMove(choice)) {
                cout << "Coup non valide, essayez à nouveau.\n";
            }
        } else {
            if (_gamemode == 1) {
                cout << "L'IA reflechit...\n";
                Move best = findBestMove(5); // profondeur = 5, ajuster selon la performance souhaitée
                cout << "L'IA joue le coup : " << (best.holeNumber + 1);
                if (best.color == Red) {
                    cout << "R\n";
                } else {
                    cout << "B\n";
                }
                makeMove(best);
            } else {
                cout << "Joueur 2, choisissez un trou impair (1,3,...,15) et une couleur (B-R) :";
                string choice;
                cin >> choice;
                if (!makeMove(choice)) {
                    cout << "Coup non valide, essayez à nouveau.\n";
                }
            }
        }
    } while (!isGameOver());

    cout << "La partie est terminée !\n";
    displayScores();
}

bool AwaleGame::makeMove(const string &input) {
    // Vérif de l'input
    Move move;
    // Vérifier la longueur pour savoir si l'index est à un ou deux chiffres
    if (input.length() == 3) {
        // Extraction pour les numéros à deux chiffres (10 à 16)
        move.holeNumber = stoi(input.substr(0, 2));
    } else if (input.length() == 2) {
        // Extraction pour les numéros à un chiffre (1 à 9)
        move.holeNumber = input[0] - '0';
    } else {
        // Format d'entrée invalide
        return false;
    }

    if (move.holeNumber < 1 || move.holeNumber > 16) {
        return false;
    } else {
        // Si le joueur 2 a saisi un nombre pair, ce n'est pas valide
        if (move.holeNumber % 2 == 0 && _currentPlayer == 2) {
            return false;
        }
        // Si le joueur 1 a saisi un nombre impair, ce n'est pas valide
        if (move.holeNumber % 2 != 0 && _currentPlayer == 1) {
            return false;
        }
    }

    move.holeNumber-=1;

    switch (input.back()) {
        case 'R':
            move.color = Red;
        break;
        case 'B':
            move.color = Blue;
        break;
        default:
            return false;
    }

    // Vérifier que le trou joué contient la couleur choisie
    if ((move.color == Red && _board[move.holeNumber].red == 0) || (move.color == Blue && _board[move.holeNumber].blue == 0)) {
        // Le trou ne contient pas la couleur choisie
        return false;
    }

    return makeMove(move);
}

// Surcharge pour exécuter un Move directement (utilisé par l'IA)
bool AwaleGame::makeMove(const Move &move) {
    int seeds = (move.color == Red) ? _board[move.holeNumber].red : _board[move.holeNumber].blue;

    // Vider le trou de la couleur jouée
    if (move.color == Red) {
        _board[move.holeNumber].red = 0;
    } else {
        _board[move.holeNumber].blue = 0;
    }

    int index = move.holeNumber;
    int holeVisited = 0;

    // Distribution bleue
    if (move.color == Blue) {
        while (seeds > 0) {
            index = (index + 1) % 16;
            if (index == move.holeNumber) continue; // Si on revient au trou de départ, on le saute
            _board[index].blue++;
            holeVisited++;
            seeds--;
        }
    }

    // Distribution rouge
    if (move.color == Red) {
        index = (index + 1) % 16;
        while (seeds > 0) {
            _board[index].red++;
            holeVisited++;
            seeds--;
            index = (index + 2) % 16;
        }
    }

    // Capture des graines
    captureSeeds(move, holeVisited);
    // Changer de joueur
    switchPlayer();

    return true;
}

void AwaleGame::captureSeeds(const Move movePlayed, int nbOfHoleVisited) {
    int capturedSeeds = 0;
    bool visitHolePlayed = true;

    if (movePlayed.color == Blue) {
        int index = (movePlayed.holeNumber+nbOfHoleVisited)%16;
        do {
            int seedsInHole = _board[index].blue + _board[index].red;
            nbOfHoleVisited--;
            if (seedsInHole == 2 || seedsInHole == 3) {
                capturedSeeds += seedsInHole;
                _board[index].blue = 0;
                _board[index].red = 0;
            } else {
                visitHolePlayed = false;
                break;
            }
            index = (index - 1 + 16) % 16;
        } while(nbOfHoleVisited > 0);
    }

    if (movePlayed.color == Red) {
        int index = (movePlayed.holeNumber + 1 + 2 * (nbOfHoleVisited-1)) % 16; // +1 : Pour avancer d’une case pour la première graine.
                                                                                // +2*(nbOfHoleVisited−1) : Pour les graines restantes, chaque graine avance de 2 cases supplémentaires.
        do {
            // Capture des seeds dans le trou actuel
            int seedsInHole = _board[index].blue + _board[index].red;
            nbOfHoleVisited--;
            if (seedsInHole == 2 || seedsInHole == 3) {
                capturedSeeds += seedsInHole;
                _board[index].blue = 0;
                _board[index].red = 0;
            } else {
                visitHolePlayed = false;
                break;
            }
            index = (index - 2 + 16) % 16;
        } while(nbOfHoleVisited > 0);
    }

    // Verification sur le trou joué
    if (visitHolePlayed) {
        int seedsInHole = _board[movePlayed.holeNumber].blue + _board[movePlayed.holeNumber].red;
        if (seedsInHole == 2 || seedsInHole == 3) {
            capturedSeeds += seedsInHole;
            _board[movePlayed.holeNumber].blue = 0;
            _board[movePlayed.holeNumber].red = 0;
        }
    }

    // Ajout des seeds capturées au score du joueur courant
    _totalSeedsOnBoard -= capturedSeeds;
    if (_currentPlayer == 1) {
        _scorePlayer1 += capturedSeeds;
    } else {
        _scorePlayer2 += capturedSeeds;
    }
}

inline void AwaleGame::switchPlayer() {
    _currentPlayer = 3 - _currentPlayer;
}

bool AwaleGame::isGameOver() {
    // Un joueur a un score >= 33
    if (_scorePlayer1 >= 33 || _scorePlayer2 >= 33) {
        return true;
    }

    // Les deux joueurs ont un score = 32 (match nul)
    if (_scorePlayer1 == 32 && _scorePlayer2 == 32) {
        return true;
    }

    // Il reste moins de 8 graines sur le plateau
    if (_totalSeedsOnBoard < 8) {
        return true;
    }

    // Si un joueur est affamé
    if (isStarved()) {
        int remainingSeeds = 0;
        int start = (_currentPlayer == 1) ? 1 : 0; // Commencer par les cases du joueur non-affamé

        // Parcours des cases du joueur non-affamé
        for (int i = start; i < 16; i += 2) {
            remainingSeeds += _board[i].red + _board[i].blue;
            _board[i].red = 0;  // Vider les graines
            _board[i].blue = 0;
        }

        // Ajouter les graines restantes au score de l'autre joueur
        if (_currentPlayer == 1) {
            _scorePlayer2 += remainingSeeds;
        } else {
            _scorePlayer1 += remainingSeeds;
        }

        return true;  // Le jeu se termine ici en cas de famine
    }

    return false;
}

void AwaleGame::displayScores() {
    cout << "Score Joueur 1 : " << _scorePlayer1 << "\n";
    cout << "Score Joueur 2 : " << _scorePlayer2 << "\n";
}

bool AwaleGame::isStarved() {
    int start = _currentPlayer == 1 ? 1 : 0;
    bool hasSeeds = false;

    // Parcours des cases du joueur actuel
    for (int i = start; i < 16; i += 2) {  // Sauter une case pour vérifier uniquement les cases paires ou impaires
        if (_board[i].red > 0 || _board[i].blue > 0) {
            hasSeeds = true;
            break;  // Si on trouve des graines, on sait que le joueur n'est pas affamé
        }
    }

    return !hasSeeds; // Retourne vrai si aucune graine n'est trouvée
}

// Fonctions pour IA
vector<AwaleGame::Move> AwaleGame::generateAllMoves(int player) {
    vector<Move> moves;
    // Trous contrôlés par le joueur
    // Joueur 1 => trous pairs: indices (1,3,5...) => en code : i%2 !=0
    // Joueur 2 => trous impairs: indices (0,2,4...) => i%2 ==0
    int start = (player == 1) ? 1 : 0;
    for (int i = start; i < 16; i += 2) {
        // Si red > 0, move Red possible
        if (_board[i].red > 0) {
            Move m;
            m.holeNumber = i;
            m.color = Red;
            moves.push_back(m);
        }
        // Si blue > 0, move Blue possible
        if (_board[i].blue > 0) {
            Move m; m.holeNumber = i; m.color = Blue;
            moves.push_back(m);
        }
    }

    return moves;
}

int AwaleGame::evaluateBoard() {
    return _scorePlayer2 - _scorePlayer1;
}

bool AwaleGame::simulateMove(const Move &m, AwaleGame &nextState) {
    nextState = *this; // Copie de l'état courant
    return nextState.makeMove(m);
}

int AwaleGame::minimax(AwaleGame state, int depth, int alpha, int beta, bool maximizingPlayer) {
    // Condition d'arrêt
    if (depth == 0 || state.isGameOver()) {
        return state.evaluateBoard();
    }

    int playerToMove = maximizingPlayer ? 2 : 1; // Ici on suppose que le joueur 2 = IA (maximizing)
    vector<Move> moves = state.generateAllMoves(playerToMove);

    if (moves.empty()) {
        // Pas de coups possibles => évaluation
        return state.evaluateBoard();
    }

    if (maximizingPlayer) {
        int maxEval = numeric_limits<int>::min();
        for (auto &m : moves) {
            AwaleGame child = state;
            if (state.simulateMove(m, child)) {
                int eval = minimax(child, depth-1, alpha, beta, false);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break; // coupe alpha-beta
            }
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        for (auto &m : moves) {
            AwaleGame child = state;
            if (state.simulateMove(m, child)) {
                int eval = minimax(child, depth-1, alpha, beta, true);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break; // coupe
            }
        }
        return minEval;
    }
}

AwaleGame::Move AwaleGame::findBestMove(int depth) {
    // Ici l'IA est le joueur 2, maximizingPlayer = true
    vector<Move> moves = generateAllMoves(2);
    Move bestMove;
    int bestValue = numeric_limits<int>::min();
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    for (auto &m : moves) {
        AwaleGame child = *this;
        if (simulateMove(m, child)) {
            int eval = minimax(child, depth-1, alpha, beta, false);
            if (eval > bestValue) {
                bestValue = eval;
                bestMove = m;
            }
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
    }

    return bestMove;
}