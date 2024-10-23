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
    move.holeNumber = input[0] - '0' - 1;

    if (move.holeNumber < 0 || move.holeNumber > 15) {
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

    switch (input[1]) {
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


    // Jouer le coup

}
