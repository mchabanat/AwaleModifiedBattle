#include <iostream>
#include "AwaleGame.h"
#include <cstdlib>   // pour std::rand, std::srand
#include <ctime>

using namespace std;

int main() {
    srand(time(nullptr)); // Pour le choix aléatoire d'un coup pour l'ia

    int gamemode = 1; // 1. SinglePlayer (versus IA)
                      // 2. Multiplayer
                      // 3. Mode automatique (IA vs IA)

    int playerBegin = 2; // 1 pour que le joueur 1 commence
                         // 2 pour que le joueur 2 commence

    int nbIA = 1; // 1 pour IA joueur 1, 2 pour IA joueur 2


    AwaleGame game(gamemode, playerBegin);
    game.playGame(nbIA);

    return 0;
}
