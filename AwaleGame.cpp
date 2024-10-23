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
        cout << "Joueur " << _currentPlayer << ", choisissez un trou (1-16) et une couleur (B-R) :";
        string choice;
        cin >> choice;


    } while (true);
}
