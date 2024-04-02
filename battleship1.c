#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>     // Timer

// CONSTANTES : 

#define DIMENSION 14    // Taille pour créer la grille 2D
#define MIN_GRILLE 2    // Index minimum de la grille de jeu
#define MAX_GRILLE 11   // Index maximum de la grille de jeu

#define BORDURE '+'     // BORDURE vaut '+' dans la grille
#define CASE_VIDE '!'   // CASE_VIDE vaut '!' dans la grille
#define TIR_REUSSI 'r'  // TIR_REUSSI vaut 'r' dans la grille
#define TIR_MANQUE 'm'  // TIR_MANQUE vaut 'm' dans la grille
#define BATEAU 'b'      // BATEAU vaut 'b' dans la grille

#define MAX_TAILLE 3    // Nombre de cases maximum d'un bateau
#define NOMBRE_BATEAUX 3    // Nombre de bateaux dans la grille

#define RESET "\x1b[0m" // Couleur par défaut
#define RED "\x1b[31m"  // Couleur rouge

// STRUCTURES :

// Structure représentant les cases d'un bateau
typedef struct{
    int ligneCase;    // Ligne de la case
    int colonneCase;  // Colonne de la case
} Case;

// Structure représentant les bateaux
typedef struct{
    int taille;             // Taille (nombre de cases) d'un bateau
    int compteur;           // Compteur de case restante d'un bateau 
    Case cases[MAX_TAILLE]; // C'est un tableau avec les coordonnées (ligne;colonne) de chaque case d'un bateau
    int isDestroyed;        // L'état actuel du bateau (0 = Pas complétement détruite, 1 = Détruite)
} Bateau;

// VARIABLES GLOBAUX :
char grille[DIMENSION][DIMENSION] = {   // Initialise la grille
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
char copy_grille[DIMENSION][DIMENSION]; // Initialise une copie de la grille (En utilisant la fonction copy_grid)

// Initialise 3 bateaux de taille 3, avec le compteur, le tableau vide et l'état actuelle du bateau
Bateau bateau1 = {3, 3, {0}, 0};
Bateau bateau2 = {3, 3, {0}, 0};
Bateau bateau3 = {3, 3, {0}, 0};

// FONCTIONS :

void print_grid();
void copy_grid();
int random(int a);
int random2(int a, int b);
void ship_coordinate(Bateau *unBateau);
int test_adjacent_coordinate(Bateau unBateau, Bateau *bateau_test, int ligneTest, int colonneTest, int direction);
int test_reverse_adjacent_coordinate(Bateau unBateau, Bateau *bateau_test, int ligneTest, int colonneTest, int direction);
Bateau* ship_research(Bateau unBateau, int ligne, int colonne);

// Affiche la grille
void print_grid(){
    for(int i = 0; i < DIMENSION; i++)
    {
        for(int j = 0;j < DIMENSION;j++)
        {
            if(grille[i][j] == BORDURE)
                printf(" \u25FC ");
            else if(grille[i][j] == CASE_VIDE)
                printf("   ");
            else if(grille[i][j] == TIR_REUSSI)
                printf(RED" X "RESET);
            else if(grille[i][j] == TIR_MANQUE)
                printf(" \u00B7 ");
            else if(grille[i][j] == BATEAU)
                printf(" o ");
            else
                printf(" %c ",grille[i][j]);
        }
        printf("\n");
    }
}

void print_grid2(){
    for(int i = 0; i < DIMENSION; i++)
    {
        for(int j = 0;j < DIMENSION;j++)
        {
            if(copy_grille[i][j] == BORDURE)
                printf(" \u25FC ");
            else if(copy_grille[i][j] == CASE_VIDE)
                printf("   ");
            else if(copy_grille[i][j] == TIR_REUSSI)
                printf(RED" X "RESET);
            else if(copy_grille[i][j] == TIR_MANQUE)
                printf(" \u00B7 ");
            else if(copy_grille[i][j] == BATEAU)
                printf(" o ");
            else
                printf(" %c ",copy_grille[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour copier la grille de base
void copy_grid() {
    // Utilisation de memcpy pour copier le tableau
    memcpy(copy_grille, grille, sizeof(grille));
}

// Retourne un entier aléatoire (entre 0 et a - 1)
int random(int a){ 
    return rand()%a;
}

// Retourne un entier aléatoire (entre a et b)
int random2(int a, int b){
    return rand()%(b-a+1)+a;
}

// Place un bateau sur la grille sans que ça se chevauche
void ship_coordinate(Bateau *unBateau){
    int result = 0;
    Bateau bateau_test;

    while (result == 0)
    {

        bateau_test.taille = unBateau->taille;  // Copie la taille du bateau
        // Initialiser les tableaux à 0
        for (int i = 0; i < MAX_TAILLE; i++) {
            bateau_test.cases[i].ligneCase = 0;
            bateau_test.cases[i].colonneCase = 0;
        }

        int direction = random(2);   // 0 = Horizontal; 1 = Vertical
        int ligneTest, colonneTest;

        // Génere un coordonnée qui entre dans l'intervalle du plateau et deviens la première case du bateau si il n'y a pas de bateau dans cette case

        do
        {
            ligneTest = random2(MIN_GRILLE, MAX_GRILLE);
            colonneTest = random2(MIN_GRILLE, MAX_GRILLE);   
        } 
        while (copy_grille[ligneTest][colonneTest] == BATEAU);

        bateau_test.cases[0].ligneCase = ligneTest;
        bateau_test.cases[0].colonneCase = colonneTest;

        result = test_adjacent_coordinate(*unBateau, &bateau_test, ligneTest, colonneTest, direction);
    
    }
    
    // Copie les coordonnés des cases dans la liste du bateau en argument et modifie la grille à l'aide de ces coordonnées
    for (int i = 0; i < unBateau->taille; i++) 
    {
        unBateau->cases[i].ligneCase = bateau_test.cases[i].ligneCase;  // Copy each element individually
        unBateau->cases[i].colonneCase = bateau_test.cases[i].colonneCase;

        copy_grille[unBateau->cases[i].ligneCase][unBateau->cases[i].colonneCase] = BATEAU;
    }
}

// Teste les coordonnées à l'aide de la direction en l'incrémentant et si c'est possible (On l'affecte à la liste des coordonnées)
int test_adjacent_coordinate(Bateau unBateau, Bateau *bateau_test, int ligneTest, int colonneTest, int direction){
    // Teste si les coordonnées ne dépasseront pas les bordures du plateau
    if (direction == 0 && colonneTest + (unBateau.taille - 1) > MAX_GRILLE) // Horizontal
        return test_reverse_adjacent_coordinate(unBateau, bateau_test, ligneTest, colonneTest, direction);

    if (direction == 1 && ligneTest + (unBateau.taille - 1) > MAX_GRILLE)   // Vertical
        return test_reverse_adjacent_coordinate(unBateau, bateau_test, ligneTest, colonneTest, direction);

    // Si la case est un bateau, on passe à la deuxième fonction sinon on affecte à la liste des coordonnées
    for(int i = 1; i < unBateau.taille; i++){
        switch (direction)
        {
        case 0:
            if(copy_grille[ligneTest][colonneTest + i] == BATEAU){
                for (int i = 1; i < unBateau.taille; i++){
                    bateau_test->cases[i].ligneCase = 0;
                    bateau_test->cases[i].colonneCase = 0;
                }
                return test_reverse_adjacent_coordinate(unBateau, bateau_test, ligneTest, colonneTest, direction);    
            }
            else{
                bateau_test->cases[i].ligneCase = ligneTest;
                bateau_test->cases[i].colonneCase = colonneTest + i;
            }
            break;
        case 1:
            if(copy_grille[ligneTest + i][colonneTest] == BATEAU){
                for (int i = 1; i < unBateau.taille; i++) {
                    bateau_test->cases[i].ligneCase = 0;
                    bateau_test->cases[i].colonneCase = 0;
                }
                return test_reverse_adjacent_coordinate(unBateau, bateau_test, ligneTest, colonneTest, direction);
            }
            else{
                bateau_test->cases[i].ligneCase = ligneTest + i;
                bateau_test->cases[i].colonneCase = colonneTest;
            }
            break;
        }
    }
    return 1;
}

// Si la fonction précedente n'est pas possible alors on fait la même chose mais en décrémentant (Return 0 = Recommence la boucle dans ship_coordon)
int test_reverse_adjacent_coordinate(Bateau unBateau, Bateau *bateau_test, int ligneTest, int colonneTest, int direction){
    // Teste si les coordonnées ne dépasseront pas les bordures du plateau
    if (direction == 0 && colonneTest - (unBateau.taille - 1) < MIN_GRILLE)
        return 0;
    if (direction == 1 && ligneTest - (unBateau.taille - 1) < MIN_GRILLE)
        return 0;

    // Si la case est un bateau, retourne 0 sinon on affecte à la liste des coordonnées
    for(int i = 1; i < unBateau.taille; i++){
        switch (direction)
        {
        case 0:
            if(copy_grille[ligneTest][colonneTest - i] == BATEAU)
                return 0;
            else{
                bateau_test->cases[i].ligneCase = ligneTest;
                bateau_test->cases[i].colonneCase = colonneTest - i;
            }
            break;
        case 1:
            if(copy_grille[ligneTest - i][colonneTest] == BATEAU)
                return 0;
            else{
                bateau_test->cases[i].ligneCase = ligneTest - i;
                bateau_test->cases[i].colonneCase = colonneTest;
            }
            break;
        }
    }
    return 1;
}

Bateau* ship_research(Bateau unBateau, int ligne, int colonne) {
    Bateau* found_ship = (Bateau*)malloc(sizeof(Bateau));
    for(int i = 0; i < unBateau.taille; i++) {
        if(copy_grille[ligne][colonne] == copy_grille[unBateau.cases[i].ligneCase][unBateau.cases[i].colonneCase]) {
            memcpy(found_ship, &unBateau, sizeof(Bateau)); // Copier la structure pour éviter de retourner une variable locale
            return found_ship; // Retourne un pointeur vers la structure Bateau
        }
    }
    free(found_ship); // Libérer la mémoire allouée si aucun bateau n'est trouvé
    return NULL; // Aucun Bateau trouvé, retourne un pointeur nul
}

// MAIN :
int main() {
    // Code ici
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires
    copy_grid();
    ship_coordinate(&bateau1);
    ship_coordinate(&bateau2);
    ship_coordinate(&bateau3);
    int nombre_tour = 1, max_tour = 75;
    // print_grid2();
    Bateau* list_ship[NOMBRE_BATEAUX] = {&bateau1, &bateau2, &bateau3};
    Bateau* found_ship;
    char list_char1[100], list_char2[100];   // 100 case dans la grille

    while(1)
    {
        printf("TOUR %d/%d\n\n", nombre_tour,max_tour);
        sleep(1);

        print_grid();
        int same=0;
        char char1=0, char2=0;
        do 
        {
            printf("\nSaisir coordonnée : ");
            scanf(" %c %c", &char1, &char2);

            // Quitter le programme
            if (char1 == 'Z')
                return EXIT_SUCCESS;
            // Vérifier si le caractère est entre 'A' et 'F' et si le chiffre est entre '0' et '9'
            if ((char1 < 'A' || char1 > 'J') || (char2 < '0' || char2 > '9')) {
            printf("La saisie n'est pas valide. Réessayez.\n");
            }
            for(int i = 0; i < 100; i++){
                if(char1 == list_char1[i] && char2 == list_char2[i])
                {
                    same = 1;
                    printf("Vous avez déjà saisi cette combinaison auparavant !\n");
                }
            }
        } while (((char1 < 'A' || char1 > 'J') || (char2 < '0' || char2 > '9')) || same);

        list_char1[nombre_tour - 1] = char1;
        list_char2[nombre_tour - 1] = char2;

        int indiceLigne = char2 - '0' + 2;
        int indiceColonne = char1 - 'A' + 2;
        int bateau_trouve = 0;
        for(int i = 0; i < NOMBRE_BATEAUX; i++){
            for(int j = 0; j < list_ship[i]->taille; j++){
                if(indiceLigne == list_ship[i]->cases[j].ligneCase && indiceColonne == list_ship[i]->cases[j].colonneCase){
                    found_ship = list_ship[i];
                    bateau_trouve = 1;
                    break;
                }
            }
            if(bateau_trouve)
                break;
        }

        switch(copy_grille[indiceLigne][indiceColonne])
        {
            case BATEAU:
                if((found_ship->compteur - 1) == 0){
                    found_ship->compteur -= 1;
                    found_ship->isDestroyed = 1;
                    grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                    printf("Touché coulé !");
                    printf("Compteur = %d", found_ship->compteur);
                }
                else{
                    found_ship->compteur -= 1;
                    grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                    printf("Touché !");
                    printf("Compteur = %d", found_ship->compteur);
                }
                break;
            case CASE_VIDE:
                grille[indiceLigne][indiceColonne] = TIR_MANQUE;
                printf(RED"Raté !"RESET);
                break;
        }

        sleep(1);
        printf("\n======================================================\n");
        nombre_tour += 1;
        if(nombre_tour > max_tour){
            printf("Vous avez perdu !\n");
            return 0;
        }
        int bateaux_detruite = 0;
        for(int i = 0; i < NOMBRE_BATEAUX; i++){
            if(list_ship[i]->isDestroyed == 1){
                bateaux_detruite += 1;
            }
        }
        if(bateaux_detruite == NOMBRE_BATEAUX){
            printf("Vous avez gagnez !\n");
            return 0;
        }
    }
    return EXIT_SUCCESS;
}