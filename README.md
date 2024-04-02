
# SAE2 - Bataille Navale

Ce projet a été réalisé dans le cadre d'une SAE (Situation d'Apprentissage et d'Evaluation) en première année de BUT GEII (Bachelor Universitaire de Technologie || Génie Électrique et Informatique Industrielle) durant le second semestre.

Le projet est écrit en langage C, compilable et exécutable dans un environnement Linux.




## Compilation

Pour compiler le fichier battleship1.c, utilisez la commande suivante :

```bash
    gcc battleship1.c -o battleship1 && ./battleship1
```
## Captures d'écran

![App Screenshot](https://i.ibb.co/pygnbMb/Screenshot-2024-04-02-225126.png)


## Sujet

#### Introduction :

La Bataille Navale est un jeu classique qui met à l'épreuve la stratégie et la logique des joueurs. 

Dans ce projet, vous êtes chargés de développer une version simplifiée de la Bataille Navale en utilisant le langage de programmation C. 


Le jeu se déroulera en mode console, avec des fonctionnalités de placement aléatoire des bateaux et d'interaction joueur-ordinateur.



#### Objectif :

L'objectif principal de ce projet est de créer un jeu fonctionnel de Bataille Navale en C, mettant en œuvre les concepts fondamentaux du langage, tels que les tableaux, les boucles, les fonctions, et les structures. 

Ce projet vous offre une opportunité d'appliquer vos connaissances en programmation en résolvant des problèmes concrets.

#### Fonctionnalités principales :

1 - Plateau de Jeu: Implémentez une grille de jeu 10x10 pour représenter l'océan. Utilisez des caractères pour afficher les emplacements des bateaux, les tirs réussis, les tirs manqués, etc.

2 - Bateaux: Créez une structure représentant un bateau, avec des attributs tels que la position (coordonnées), la taille, et un indicateur de coulé. Générez aléatoirement trois bateaux de taille 3 sur le plateau.

3 - Placement Aléatoire: Assurez-vous que les bateaux sont placés aléatoirement sur le plateau, sans chevauchement. 

4- Tour de Jeu: Implémentez la logique du jeu, permettant au joueur de spécifier des coordonnées pour tirer sur le plateau. Affichez des messages indiquant si le tir a touché, manqué, ou coulé un bateau.

5 - Fin de Jeu: Le jeu doit se terminer lorsque tous les bateaux ont été coulés. Affichez le nombre de tours nécessaires pour remporter la victoire.



#### Suggestions d'amélioration (bonus) :

a) Interface Graphique: Ajoutez une interface graphique pour améliorer l'expérience utilisateur.

b) Niveaux de Difficulté: Intégrez des niveaux de difficulté en ajustant la disposition des bateaux ou en limitant le nombre de tirs.

c) Statistiques: Enregistrez et affichez les statistiques du joueur, telles que le nombre de parties jouées, gagnées, etc.

d) Bateaux Personnalisés: Permettez au joueur de spécifier les emplacements des bateaux au début du jeu.

e) Mode Deux Joueurs: Ajoutez un mode multijoueur où deux joueurs peuvent s'affronter sur des grilles distinctes.


## Avancement

Étant donné que le second semestre est en cours, il est probable que le dépôt GitHub ne soit pas à jour.

- 02/04/2024 : Ajout de battleship1.c contenant les fonctionnalités principales
- L'implémentation des suggestions sont bientôt achevés (il ne manque plus qu'à créer une fonction qui permet le choix du mode de jeu)

