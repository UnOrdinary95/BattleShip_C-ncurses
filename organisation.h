#ifndef ORGANISATION_H
#define ORGANISATION_H

#define DIMENSION 14    // Taille grille 2D
#define MIN_GRILLE 2    // Index minimum de la grille
#define MAX_GRILLE 11   // Index maximum de la grille
#define NOMBRE_CASES_GRILLE 100 // Nombre de cases dans la grille

#define BORDURE '+'     // BORDURE vaut '+' dans la grille
#define CASE_VIDE '!'   // CASE_VIDE vaut '!' dans la grille
#define TIR_REUSSI 'r'  // TIR_REUSSI vaut 'r' dans la grille
#define TIR_MANQUE 'm'  // TIR_MANQUE vaut 'm' dans la grille
#define BATEAU 'b'      // BATEAU vaut 'b' dans la grille

#define NOMBRE_BATEAUX 3    // Nombre de bateaux dans la grille
#define MAX_TAILLE 3    // Nombre de cases maximum d'un bateau

// Sockets
// #define IP_SERVEUR "192.168.1.28"
#define IP_SERVEUR "192.168.43.103"  // IP en faisant un partage de connexion
#define PORT 32777
#define TAILLE_MESSAGE 1024
#define NOMBRE_CLIENT 2

// Structure représentant les cases d'un bateau
typedef struct{
    int ligneCase;      // Ligne de la case
    int colonneCase;    // Colonne de la case
} Case;

// Structure représentant les bateaux
typedef struct{
    int taille;             // Taille (nombre de cases) d'un bateau
    int compteur;           // Compteur de case restante d'un bateau 
    Case cases[MAX_TAILLE]; // C'est un tableau avec les coordonnées (ligne;colonne) de chaque case d'un bateau
    int isDestroyed;        // L'état actuel du bateau (0 = Pas complétement détruite, 1 = Détruite)
} Bateau;

// Stucture représentant les joueurs
typedef struct{
    char nom[15];                           // Nom du joueur
    char (*grille_bateau)[DIMENSION];       // Sa grille contenant les bateaux
    char (*grille)[DIMENSION];              // Grille de jeu
    char list_char1[NOMBRE_CASES_GRILLE];   // Liste de caractère déjà saisie (LETTRE)
    char list_char2[NOMBRE_CASES_GRILLE];   // Liste de caractère déjà saisie (CHIFFRE)
    Bateau* liste_bateaux[NOMBRE_BATEAUX];  // Liste de bateaux du joueur
} Joueur;

extern char grille_A[DIMENSION][DIMENSION];
extern char grille_A_bateaux[DIMENSION][DIMENSION];
extern char grille_B[DIMENSION][DIMENSION];
extern char grille_B_bateaux[DIMENSION][DIMENSION];

extern Bateau bateau1_A;
extern Bateau bateau2_A;
extern Bateau bateau3_A;
extern Bateau bateau1_B;
extern Bateau bateau2_B;
extern Bateau bateau3_B;

extern Joueur Joueur_A;
extern Joueur Joueur_B;
extern Joueur* liste_joueurs[2];


#endif /* ORGANISATION.H */