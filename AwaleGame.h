//
// Created by matis on 23/10/2024.
//

#ifndef AWALEGAME_H
#define AWALEGAME_H
#include <string>

enum Color { Blue, Red };

class AwaleGame {
private:
    struct Hole { // Type pour le nombre de boules de chaque couleurs par case
        int red;
        int blue;
    };

    struct Move {
        int holeNumber;
        Color color;
    };

    Hole _board[16];
    int _scorePlayer1;
    int _scorePlayer2;
    int _currentPlayer;    // 1 pour joueur 1, 2 pour joueur 2

public:
    AwaleGame();
    void displayBoard();
    void playGame();
    bool makeMove(const std::string &);
};

#endif //AWALEGAME_H
