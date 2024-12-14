#include <iostream>
#include "AwaleGame.h"

using namespace std;

int main() {
    int gamemode = 1; // 1. SinglePlayer (versus IA)
                      // 2. Multiplayer

    int playerBegin = 2; // 1 pour que le joueur 1 commence (Humain)
                         // 2 pour que le joueur 2 commence (IA)

    AwaleGame game(gamemode, playerBegin);
    game.playGame();

    return 0;
}
