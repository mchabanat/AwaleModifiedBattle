//
// Created by matis on 23/10/2024.
//

#include "AwaleGame.h"
#include <iostream>

using namespace std;

AwaleGame::AwaleGame(): _scorePlayer1(0), _scorePlayer2(0), _currentPlayer(1), _totalSeedsOnBoard(64) {
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
    do {
        displayBoard();

        // Afficher les scores
        displayScores();

        // Saisie du coup du joueur
        if (_currentPlayer == 1) {
            cout << "Joueur 1, choisissez un trou pair (2,4,...,16) et une couleur (B-R) :";
        } else {
            cout << "Joueur 2, choisissez un trou impair (1,3,...,15) et une couleur (B-R) :";
        }
        string choice;
        cin >> choice;

        // Faire le mouvement
        if (!makeMove(choice)) {
            cout << "Coup non valide, essayez a nouveau.\n";
        }

    } while (!isGameOver());

    cout << "La partie est terminee !\n";
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

    // Jouer le coup
    int seeds = (move.color == Red) ? _board[move.holeNumber].red : _board[move.holeNumber].blue;
    int holeVisited = 0;

    // Vide le trou de départ de la couleur choisie
    if (move.color == Red) {
        _board[move.holeNumber].red = 0;
    } else {
        _board[move.holeNumber].blue = 0;
    }

    int index = move.holeNumber;

    // Distribution des graines bleues
    if (move.color == Blue) {
        while (seeds > 0) {
            index = (index + 1) % 16;

            // Ne pas redistribuer dans le trou de départ
            if (index == move.holeNumber) continue;

            _board[index].blue++;
            holeVisited++;
            seeds--;
        }
    }

    // Distribution des graines rouges avec saut de 2 cases
    if (move.color == Red) {
        // Passer directement au premier trou de l’adversaire
        index = (index + 1) % 16;

        while (seeds > 0) {
            _board[index].red++;
            holeVisited++;
            seeds--;

            // Incrément de 2 à chaque tour pour passer aux cases adverses
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
