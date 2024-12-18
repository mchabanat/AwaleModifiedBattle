//
// Created by matis on 23/10/2024.
//

#include "AwaleGame.h"
#include <iostream>
#include <limits>
#include <algorithm> // pour std::max
#include <cstdlib>   // pour std::rand, std::srand

using namespace std;

AwaleGame::AwaleGame(int gamemode, int playerToBegin): _gamemode(gamemode), _playerBegin(playerToBegin), _scorePlayer1(0), _scorePlayer2(0), _currentPlayer(1), _totalSeedsOnBoard(64), _movesSinceLastCapture(0) {
    if (gamemode < 1 || gamemode > 3) {
        cerr << "Gamemode invalide. Utilisez 1 pour singleplayer, 2 pour multiplayer ou 3 pour IA vs IA." << endl;
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
        cout << "Mode singleplayer selectionne." << endl;
        if (_playerBegin == 2) {
            switchPlayer(); // L'IA commence
        }
    } else if (_gamemode == 2) {
        cout << "Mode multiplayer selectionne." << endl;
    } else if (_gamemode == 3) {
        cout << "Mode IA vs IA selectionne." << endl;
    }

    do {
        displayBoard();
        displayScores();

        if (_gamemode == 3) {
            // IA vs IA
            cout << "Tour du joueur " << _currentPlayer << " (IA)...\n";
            Move best = findBestMove( 6); // profondeur au choix
            cout << "L'IA " << _currentPlayer << " joue le coup : " << (best.holeNumber + 1)
                 << (best.color == Red ? 'R' : 'B') << "\n";
            makeMove(best);
        } else if (_gamemode == 1) {
            // Humain vs IA
            if (_currentPlayer == 1) {
                cout << "Joueur 1, choisissez un trou pair (2,4,...,16) et une couleur (B-R) :";
                string choice;
                cin >> choice;
                if (!makeMove(choice)) {
                    cout << "Coup non valide, essayez a nouveau.\n";
                }
            } else {
                cout << "L'IA reflechit...\n";
                Move best = findBestMove(5);
                cout << "L'IA joue le coup : " << (best.holeNumber + 1)
                     << (best.color == Red ? 'R' : 'B') << "\n";
                makeMove(best);
            }
        } else {
            // Mode 2 : Humain vs Humain
            if (_currentPlayer == 1) {
                cout << "Joueur 1, choisissez un trou pair (2,4,...,16) et une couleur (B-R) :";
            } else {
                cout << "Joueur 2, choisissez un trou impair (1,3,...,15) et une couleur (B-R) :";
            }
            string choice;
            cin >> choice;
            if (!makeMove(choice)) {
                cout << "Coup non valide, essayez a nouveau.\n";
            }
        }
    } while (!isGameOver());

    cout << "La partie est terminee !\n";
    displayBoard();
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

    // Mise à jour de movesSinceLastCapture
    if (capturedSeeds > 0) {
        _movesSinceLastCapture = 0;
    } else {
        _movesSinceLastCapture++;
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

int AwaleGame::evaluateBoard(int maximizingPlayer) {
    int aiScore, opponentScore;
    if (maximizingPlayer == 1) {
        aiScore = _scorePlayer1;
        opponentScore = _scorePlayer2;
    } else {
        aiScore = _scorePlayer2;
        opponentScore = _scorePlayer1;
    }
    int scoreDifference = aiScore - opponentScore;

    // Critère 1 : Différence de score
    // Plus le scoreDifference est élevé, mieux c'est pour l'IA.
    int evaluation = scoreDifference * 10;

    // Critère 2 : Trous capturables
    // On compte combien de trous sont capturables et par qui.
    // on donne un petit avantage si beaucoup de trous sont à 2 ou 3 graines juste avant
    // le tour de l'IA, et un désavantage s'ils sont faciles à capturer par l'adversaire.
    int holesWith2or3 = 0;
    for (int i = 0; i < 16; i++) {
        int totalSeeds = _board[i].red + _board[i].blue;
        if (totalSeeds == 2 || totalSeeds == 3) {
            holesWith2or3++;
        }
    }

    if (_currentPlayer == maximizingPlayer) {
        evaluation += holesWith2or3 * 2; // Bonus si l'IA peut capturer au prochain coup.
    } else {
        evaluation -= holesWith2or3 * 2; // Malus si l'adversaire peut en profiter avant l'IA.
    }

    // Critère 3 : Contrôle des graines
    // On peut essayer de compter le nombre total de graines appartenant au joueur 2 vs joueur 1.
    // cela peut aider à prévoir le futur.
    int player1SeedsOnBoard = 0;
    int player2SeedsOnBoard = 0;
    for (int i = 0; i < 16; i++) {
        int seeds = _board[i].red + _board[i].blue;
        if ((i % 2) == 1) {
            // Trous impairs : joueur 1
            player1SeedsOnBoard += seeds;
        } else {
            // Trous pairs : joueur 2
            player2SeedsOnBoard += seeds;
        }
    }

    // Si maximizingPlayer == 1, aiSeedsOnBoard correspond à player1SeedsOnBoard
    // sinon aiSeedsOnBoard correspond à player2SeedsOnBoard
    int aiSeedsOnBoard = (maximizingPlayer == 1) ? player1SeedsOnBoard : player2SeedsOnBoard;
    int oppSeedsOnBoard = (maximizingPlayer == 1) ? player2SeedsOnBoard : player1SeedsOnBoard;

    int boardControl = aiSeedsOnBoard - oppSeedsOnBoard;
    // On donne un petit poids à ce contrôle : s'il y a beaucoup plus de graines chez l'IA,
    // elle a potentiellement plus d'options à l'avenir.
    evaluation += boardControl;

    // Critère 4 : Phase du jeu
    // Moins il reste de graines sur le plateau, plus la différence de score est critique.
    // s'il reste très peu de graines, on vient multiplier l'impact du scoreDifference.
    if (_totalSeedsOnBoard < 20) {
        evaluation += scoreDifference * 5;
    }

    // Critère 5 : Famine
    // On considère la famine si un des joueurs n'a plus de graines dans ses trous.
    if (oppSeedsOnBoard == 0) {
        // L'adversaire est affamé, énorme bonus
        evaluation += 100;
    }
    if (aiSeedsOnBoard == 0) {
        // L'IA est affamée, énorme malus
        evaluation -= 100;
    }

    // Critère 6 : prise de risque
    int riskBonus = (64 - _totalSeedsOnBoard) / 5; // On divise par 5 pour modérer l'effet
    evaluation += riskBonus;

    // Critère 7 : anti-stagnation
    int stagnationPenalty = (_movesSinceLastCapture / 5);
    evaluation -= stagnationPenalty;

    return evaluation;
}

bool AwaleGame::simulateMove(const Move &m, AwaleGame &nextState) {
    nextState = *this; // Copie de l'état courant
    return nextState.makeMove(m);
}

int AwaleGame::minimax(AwaleGame state, int depth, int alpha, int beta, bool maximizing, int maximizingPlayer) {
    if (depth == 0 || state.isGameOver()) {
        return state.evaluateBoard(maximizingPlayer);
    }

    int playerToMove = maximizing ? maximizingPlayer : (3 - maximizingPlayer);

    // Génération des coups pour playerToMove
    auto moves = state.generateAllMoves(playerToMove);

    if (moves.empty()) {
        return state.evaluateBoard(maximizingPlayer);
    }

    if (maximizing) {
        int maxEval = numeric_limits<int>::min();
        for (auto &m : moves) {
            AwaleGame child = state;
            if (child.makeMove(m)) {
                int eval = minimax(child, depth-1, alpha, beta, false, maximizingPlayer);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        for (auto &m : moves) {
            AwaleGame child = state;
            if (child.makeMove(m)) {
                int eval = minimax(child, depth-1, alpha, beta, true, maximizingPlayer);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) break;
            }
        }
        return minEval;
    }
}

AwaleGame::Move AwaleGame::findBestMove(int depth) {
    int maximizingPlayer = _currentPlayer;
    auto moves = generateAllMoves(maximizingPlayer);
    Move bestMove;
    int bestValue = numeric_limits<int>::min();
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    // Vecteur pour stocker tous les meilleurs coups
    std::vector<Move> bestMoves;

    for (auto &m : moves) {
        AwaleGame child = *this;
        if (child.makeMove(m)) {
            int eval = minimax(child, depth-1, alpha, beta, false, maximizingPlayer);
            if (eval > bestValue) {
                bestValue = eval;
                bestMoves.clear();
                bestMoves.push_back(m);
            } else if (eval == bestValue) {
                // Ce coup a la même valeur que le meilleur coup actuel
                bestMoves.push_back(m);
            }
            alpha = max(alpha, eval);
            if (beta <= alpha) break;
        }
    }

    // Si plusieurs coups ont la même meilleure valeur, on en choisit un au hasard
    if (!bestMoves.empty()) {
        int randomIndex = rand() % bestMoves.size();
        return bestMoves[randomIndex];
    }

    // Si pour une raison quelconque on n'a pas de meilleur coup, on renvoie le premier coup
    // (Cas théorique qui ne devrait pas arriver si on gère correctement)
    return moves.front();
}