# AwaleModifiedBattle
Ce dépot contient le code d'un jeu qui ressemble au jeu de société d'origine africaine « Awalé ». 
Les règles ont été modifiées par notre professeur.
C'est un jeu C++ en console développé dans le cadre du cours AI Game Programming du Master Informatique de Sophia Antipolis.
L'objectif de ce jeu est pour nous de développer une IA la plus forte possible avec l'algorithme MinMax avec Alpha-Beta Cut pour combattre les IA des autres groupes de la classe. 

## Auteurs
Le jeu a été développé en équipe de 2 :
- Matis Chabanat
- Raphael Viaud

## Installation
Il y a deux paramètres à configurer dans le main.cpp :
1. Gamemode 
-	« 1 » pour faire une partie entre vous et l’IA (mode singleplayer) 
-	« 2 » pour faire une partie entre vous et un autre joueur (mode multiplayer)
-	« 3 » pour faire une partie entre 2 IA (mode automatique)
2. playerBegin 
-	« 1 » pour que le joueur 1 commence 
-	« 2 » pour que le joueur 2 commence

/!\ 
J'ai eu des bugs avant la compétition donc pour jouer, 
- laisser Gamemode à 1
- Mettre playerBegin à 2
- Et un 3e paramètre nbIA qu'il faut mettre à 1 si l'IA commence ou à 2 si elle joue en 2e
/!\

Une fois la configuration terminée, exécutez et jouez avec les règles suivantes.

## Règles
Les règles du jeu sont les suivantes : 

### Le plateau
Il y a 16 trous, 8 par joueur
Les trous sont numérotés de 1 à 16. On tourne dans le sens des aiguilles d'une montre : Le trou 1 suit le trou 16 dans le sens des aiguilles d'une montre. 
Le premier joueur a les trous impairs, le deuxième joueur a les trous pairs.
(Attention, ceci est très différent du logiciel)

Il y a deux couleurs : rouge et bleu
Au début, il y a 2 graines de chaque couleur par trou.

### But du jeu
Le jeu commence avec 2+2 graines dans chaque trou. Le but du jeu est de capturer plus de graines que son adversaire. Comme il y a un nombre pair de graines, il est possible que le jeu se termine par un match nul, chaque joueur ayant capturé 32 graines.

### Déplacer les graines
Les joueurs déplacent les graines à tour de rôle. Lors d'un tour, un joueur choisit l'un des trous de hauteur qu'il contrôle. Le joueur retire les graines de ce trou (voir ci-dessous pour la gestion des couleurs), et les distribue, en les faisant tomber une par trou dans le sens des aiguilles d'une montre (c'est-à-dire dans un ordre non décroissant) à partir de ce trou, dans un processus appelé « semis ». 
Les déplacements sont effectués en fonction des couleurs. Une couleur est d'abord définie et toutes les graines de cette couleur sont jouées, 
Si les graines sont bleues, elles sont réparties dans chaque trou. Si les graines sont rouges, elles sont distribuées uniquement dans les trous de l'adversaire.

Les graines ne sont pas distribuées dans le trou d'où elles sont tirées. Le trou de départ est toujours laissé vide ; s'il contenait 16 graines (ou plus), il est sauté, et la seizième graine est placée dans le trou suivant. 
Ainsi, un coup est exprimé par NC où N est le numéro du trou, C est la couleur qui est jouée
Par exemple, 3R signifie que l'on joue les graines rouges du trou 3 (et seulement les rouges). 

### Capture 
La capture n'a lieu que lorsqu'un joueur porte le compte d'un trou à exactement deux ou trois graines (de n'importe quelle couleur). Il capture toujours les graines du trou correspondant, et éventuellement plus : Si l'avant-dernière graine a également amené un trou à deux ou trois graines, celles-ci sont également capturées, et ainsi de suite jusqu'à ce qu'un trou soit atteint qui ne contienne pas deux ou trois graines. Les graines capturées sont mises de côté. Affamer l'adversaire EST PERMIS
Attention, il est permis de prendre les graines de son propre trou et les graines sont capturées indépendamment de leurs couleurs.
Prendre toutes les graines de l'adversaire est autorisé. En cas d'affamement, toutes les graines sont capturées par le dernier joueur.
Le jeu s'arrête lorsqu'il y a strictement moins de 8 graines sur le plateau. Dans ce cas, les graines restantes ne sont pas prises en compte.

### Victoire
La partie est terminée lorsqu'un joueur a capturé 33 graines ou plus, ou que chaque joueur a pris 32 graines (tirage au sort), ou qu'il ne reste que strictement moins de 8 graines. Le gagnant est le joueur qui a plus de graines que son adversaire.



