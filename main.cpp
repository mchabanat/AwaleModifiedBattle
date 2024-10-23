#include <iostream>
#include "AwaleGame.h"

using namespace std;

Coup lireCoup(const std::string& saisie) {
    Coup coup;
    coup.numeroCase = saisie[0] - '0'; // Convertit le caractère en entier

    if (saisie[1] == 'B') {
        coup.couleur = Bleu;
    } else if (saisie[1] == 'R') {
        coup.couleur = Rouge;
    }

    return coup;
}

int main() {
    //string saisie;
    //cout << "Entrez un coup (ex : 5R ou 2B) : ";
    //cin >> saisie;

    //Coup coup = lireCoup(saisie);

    AwaleGame game;
    game.playGame();

    return 0;
}
