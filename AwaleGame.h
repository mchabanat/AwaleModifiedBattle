//
// Created by matis on 23/10/2024.
//

#ifndef AWALEGAME_H
#define AWALEGAME_H
#include <string>

enum Color { Blue, Red };

class AwaleGame {
private:
    int _gamemode; // 1 pour singleplayer (vs IA), 2 pour multiplayer
    int _playerBegin; // 1 pour que l'humain commence, 2 pour que l'IA commence

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
    int _currentPlayer;    // 1 pour joueur Humain, 2 pour joueur IA
    int _totalSeedsOnBoard;

public:
    AwaleGame(int gamemode, int playerToBegin);
    void displayBoard();
    void playGame();
    bool makeMove(const std::string &);
    void captureSeeds(Move, int);
    void switchPlayer();
    bool isGameOver();
    void displayScores();
    bool isStarved();

};

#endif //AWALEGAME_H
