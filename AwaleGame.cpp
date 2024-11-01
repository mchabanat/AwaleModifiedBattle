//
// Created by matis on 23/10/2024.
//

#include "AwaleGame.h"
#include <iostream>

using namespace std;

AwaleGame::AwaleGame(): _scorePlayer1(0), _scorePlayer2(0), _currentPlayer(1) {
    for (int i = 0; i < 16; ++i) {
        _board[i].red = 2;
        _board[i].blue = 2;
    }
}

void AwaleGame::displayBoard() {
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
}

void AwaleGame::playGame() {
    do {
        displayBoard();

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

    } while (true);
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

    // Vide le trou de départ de la couleur choisie
    if (move.color == Red) {
        _board[move.holeNumber].red = 0;
    } else {
        _board[move.holeNumber].blue = 0;
    }

    int index = move.holeNumber;
    int capturedSeeds = 0;

    // Distribution des graines bleues
    if (move.color == Blue) {
        while (seeds > 0) {
            index = (index + 1) % 16;

            // Ne pas redistribuer dans le trou de départ
            if (index == move.holeNumber) continue;

            _board[index].blue++;
            seeds--;

            // Capture des seeds dans le trou actuel
            int seedsInHole = _board[index].blue + _board[index].red;
            if (seedsInHole == 2 || seedsInHole == 3) {
                capturedSeeds += seedsInHole;
                _board[index].blue = 0;
                _board[index].red = 0;
            } else {
                capturedSeeds = 0;
            }
        }
    }

    // Distribution des graines rouges avec saut de 2 cases
    if (move.color == Red) {
        // Passer directement au premier trou de l’adversaire
        index = (index + 1) % 16;

        while (seeds > 0) {
            _board[index].red++;
            seeds--;

            // Capture des seeds dans le trou actuel
            int seedsInHole = _board[index].blue + _board[index].red;
            if (seedsInHole == 2 || seedsInHole == 3) {
                capturedSeeds += seedsInHole;
                _board[index].blue = 0;
                _board[index].red = 0;
            } else {
                capturedSeeds = 0;
            }

            // Incrément de 2 à chaque tour pour passer aux cases adverses
            index = (index + 2) % 16;
        }
    }

    // Ajout des seeds capturées au score du joueur courant
    if (_currentPlayer == 1) {
        _scorePlayer1 += capturedSeeds;
    } else {
        _scorePlayer2 += capturedSeeds;
    }

    // Changer de joueur
    switchPlayer();

    return true;
}

inline void AwaleGame::switchPlayer() {
    _currentPlayer = 3 - _currentPlayer;
}
