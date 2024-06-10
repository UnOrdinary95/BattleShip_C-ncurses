// gcc main.c solo.c multi.c client.c serveur.c -o main -lncurses && ./main

#include <stdio.h>
#include <stdlib.h>

#include "solo.h"
#include "multi.h"
#include "client.h"
#include "serveur.h"

#include "organisation.h"
#include "function.h"

char grille_A[DIMENSION][DIMENSION] = {
    {'+','+','+','+','+','+','+','+','+','+','+','+','+','+'},
    {'+','+','A','B','C','D','E','F','G','H','I','J','+','+'},
    {'+','0','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','1','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','2','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','3','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','4','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','5','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','6','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','7','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','8','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','9','!','!','!','!','!','!','!','!','!','!','+','+'},
    {'+','+','+','+','+','+','+','+','+','+','+','+','+','+'},
    {'+','+','+','+','+','+','+','+','+','+','+','+','+','+'},
};

char grille_A_bateaux[DIMENSION][DIMENSION];
char grille_B[DIMENSION][DIMENSION];
char grille_B_bateaux[DIMENSION][DIMENSION];

// Initialise les bateaux des joueurs de taille 3, avec le compteur, le tableau vide et l'état actuelle du bateau
Bateau bateau1_A = {3, 3, {0}, 0}; 
Bateau bateau2_A = {3, 3, {0}, 0}; 
Bateau bateau3_A = {3, 3, {0}, 0}; 

Bateau bateau1_B = {3, 3, {0}, 0}; 
Bateau bateau2_B = {3, 3, {0}, 0}; 
Bateau bateau3_B = {3, 3, {0}, 0};

// Initialise les 2 joueurs ainsi qu'une liste de joueur
Joueur Joueur_A = {{0}, grille_A_bateaux, grille_A, {0}, {0}, {&bateau1_A, &bateau2_A, &bateau3_A}};
Joueur Joueur_B = {{0}, grille_B_bateaux, grille_B, {0}, {0}, {&bateau1_B, &bateau2_B, &bateau3_B}};
Joueur* liste_joueurs[2] = {&Joueur_A, &Joueur_B};

// gcc main.c game1.c game2.c serveur.c -o main -lncurses
void afficherMenu() {
    printf("\n\nBienvenue au jeu de bataille navale !\n");
    printf("Choisissez un mode de jeu:\n");
    printf("1. Jouer contre l'ordinateur\n");
    printf("2. Jouer contre un autre joueur (local)\n");
    printf("3. Jouer contre un autre joueur (via sockets)\n");
    printf("4. Lancer le serveur de jeu\n");
    printf("Votre choix: ");
}

int main() {
    int choix;

    afficherMenu();
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            system("clear");
            jouerContreOrdinateur();
            break;
        case 2:
            system("clear");
            jouerContreJoueurLocal();
            break;
        case 3:
            system("clear");
            jouerContreJoueurSocket();
            break;
        case 4:
            system("clear");
            lancerServeur();
            break;
        default:
            printf("Choix invalide.\n");
            break;
    }

    return 0;
}
