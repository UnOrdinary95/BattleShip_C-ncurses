#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "organisation.h"
#include "function.h"


void enter_name_2(Joueur *Joueur_A, Joueur *Joueur_B);
void choose_difficulty(int *max_tour);
void copy_grid_2();
void print_positioning_function_2();
int print_result(int nombre_tour, int max_tour);
void print_stats(int partie_jouees, char plays_txt[25], int partie_gagneesA, int partie_gagneesB);
int load_data(const char* fichier_texte);
void save_data(const char* fichier_texte,int uneDonnee);


// Choix du nom des joueurs
void enter_name_2(Joueur *Joueur_A, Joueur *Joueur_B){
    char nom_A[12], nom_B[12];
    
    printw("\n[Joueur A] - Saississez votre pseudo : ");
    getnstr(nom_A, sizeof(nom_A));  // Permet de saisir une chaine de caractère

    printw("\n[Joueur B] - Saississez votre pseudo : ");
    getnstr(nom_B, sizeof(nom_B));  // Permet de saisir une chaine de caractère

    strcpy((*Joueur_A).nom, nom_A);
    strcpy((*Joueur_B).nom, nom_B);
}

// Propose au joueur de choisir un niveau de difficulté
void choose_difficulty(int *max_tour){
    int difficulty;

    printw("\nVeuillez choisir la difficulté parmi les options suivantes :\n");
    printw("1- Facile (75 tours max)\n2- Normal (50 tours max)\n3- Difficile (35 tours max)\n");

    do
    {
        printw("\nChoix : ");
        int caractère = getch();    // getch() attend la saisie d'une touche du clavier et renvoie le code ASCII du caractère (un int)
        switch (caractère)
        {
            case '1':
                difficulty = 1;
                break;
            case '2':
                difficulty = 2;
                break;
            case '3':
                difficulty = 3;
                break;
            default:
                printw("\nVeuillez choisir un chiffre entre 1 et 3 !\n");
                refresh();
                sleep(1);
                break;
        }
    }while(difficulty < 1 || difficulty > 3);

    sleep(1);

    if(difficulty == 1)
        *max_tour = 75;
    else if(difficulty == 2)
        *max_tour = 50;
    else if (difficulty == 3)
        *max_tour = 35;
    
    clear();
    refresh();
}

// Copie la grille de base
void copy_grid_2() {
    // Utilisation de memcpy pour copier le tableau "grille_A"
    memcpy(grille_A_bateaux, grille_A, sizeof(grille_A));
    memcpy(grille_B, grille_A, sizeof(grille_A));
    memcpy(grille_B_bateaux, grille_A, sizeof(grille_A));
}

// Permet aux joueurs de spécifier les emplacements des bateaux au début du jeu.
void print_positioning_function_2(){
    char nombre_ordinaux[3][10] = {"premier", "deuxième", "troisième"};

    for (int i = 0; i < 2; i++){
        clear();
        refresh();
        move(2, 0);
        printw("--------Placement des bateaux de %s--------\n", liste_joueurs[i]->nom);
        print_grid(liste_joueurs[i]->grille_bateau);
        for (int j = 0; j < 3; j++){
            mvprintw(2, 55, "%s, tu vas placer le %s bateau.\n", liste_joueurs[i]->nom, nombre_ordinaux[j]);
            ship_positioning(*liste_joueurs[i], liste_joueurs[i]->liste_bateaux[j], liste_joueurs[i]->grille_bateau);
            add_ship(liste_joueurs[i]->grille_bateau, liste_joueurs[i]->liste_bateaux[j]);

            clear();
            move(2, 0);
            printw("--------Placement des bateaux de %s--------\n", liste_joueurs[i]->nom);
            print_grid(liste_joueurs[i]->grille_bateau);
        }
    }
    clear();
    refresh();
}

// Affiche le résultat de la partie
int print_result(int nombre_tour, int max_tour){
    for (int i = 0; i < 2; i++){
        if(nombre_tour > max_tour){
            clear();
            mvprintw(2, 0, "MAX TOUR atteint, personne n'a gagné durant cette partie !\n");
            printw("======================================================\n");
            sleep(1);
            return -1;
        }

        int bateaux_detruite=0;

        for(int j = 0; j < NOMBRE_BATEAUX; j++){
            if(liste_joueurs[2 - 1 - i]->liste_bateaux[j]->isDestroyed == 1){
                bateaux_detruite += 1;
            }
        }
        
        if (bateaux_detruite == NOMBRE_BATEAUX){
            clear();
            mvprintw(2, 0, "%s a gagner cette partie !\n", liste_joueurs[i]->nom);
            printw("======================================================\n");
            sleep(1);
            if (i == 0){
                return -2;
            }
            else{
                return -3;
            }
        }
    }
    refresh();
}

// Affiche les statistiques
void print_stats(int partie_jouees, char plays_txt[25], int partie_gagneesA, int partie_gagneesB){
    partie_jouees = partie_jouees + 1;
    save_data(plays_txt, partie_jouees);

    mvprintw(3, 0, "- STATISTIQUES :\n");
    printw("Partie Jouées : %d\nPartie Gagnées (Joueur A) : %d\nPartie Gagnées (Joueur B) : %d\n", partie_jouees, partie_gagneesA, partie_gagneesB);
    refresh();
    sleep(3);
}

// Charge une donnée d'un fichier texte
int load_data(const char* fichier_texte) 
{
    FILE *fichier = fopen(fichier_texte, "r");
    int uneDonnee = 0;

    if (fichier != NULL) {
        fscanf(fichier, "%d", &uneDonnee);
        fclose(fichier);
    }

    return uneDonnee;
}

// Sauvegarde une donnée dans un fichier texte
void save_data(const char* fichier_texte,int uneDonnee) {
    FILE *fichier = fopen(fichier_texte, "w");

    if (fichier != NULL) {
        fprintf(fichier, "%d", uneDonnee);
        fclose(fichier);
    }
}


int jouerContreJoueurLocal(){
    // Code ici
    initscr();  // Initialisation de la fenêtre ncurses
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires    
    start_color();  // Initialisation du support des couleurs
    init_pair(1, COLOR_RED, COLOR_BLACK);   // Créer la couleur Rouge
    init_pair(2, COLOR_CYAN, COLOR_BLACK);  // Créer la couleur Bleu (Cyan)

    char plays_txt[25] = "partie_jouées.txt";
    char winsA_txt[25] = "partie_gagnées_A.txt";
    char winsB_txt[25] = "partie_gagnées_B.txt";
    int partie_jouees = load_data(plays_txt);
    int partie_gagneesA = load_data(winsA_txt);
    int partie_gagneesB = load_data(winsB_txt);

    int nombre_tour = 1, max_tour;

    copy_grid_2();

    mvprintw(1, 1, " ____        _   _   _      ____  _     _       ");
    mvprintw(2, 1, "| __ )  __ _| |_| |_| | ___/ ___|| |__ (_)_ __  ");
    mvprintw(3, 1, "|  _ \\ / _` | __| __| |/ _ \\___ \\| '_ \\| | '_ \\ ");
    mvprintw(4, 1, "| |_) | (_| | |_| |_| |  __/___) | | | | | |_) |");
    mvprintw(5, 1, "|____/ \\__,_|\\__|\\__|_|\\___|____/|_| |_|_| .__/ ");
    mvprintw(6, 1, "                                         |_|    ");
    enter_name_2(&Joueur_A, &Joueur_B);
    choose_difficulty(&max_tour);
    print_positioning_function_2();

    Bateau* found_ship;
    while(1){
        for (int iteration = 0; iteration < 2; iteration++){    // G3
            mvprintw(2, 0, "TOUR %d/%d (%s)\n\n", nombre_tour, max_tour, liste_joueurs[iteration]->nom);
            print_grid(liste_joueurs[iteration]->grille);
            
            mvprintw(2, 55, "%s va effectuer un tir.\n", liste_joueurs[iteration]->nom);    // Déplacer le curseur à la position spécifiée par les coordonnées (y, x) avant d'afficher
            int pareil;
            char char1 = 0, char2 = 0;

            do{
                pareil = 0;
                mvprintw(3, 55, "%s, saisit les coordonnées (LETTRE/CHIFFRE) : ", liste_joueurs[iteration]->nom);
                char1 = getch();
                char2 = getch();
                curs_set(0); // Désactive l'affichage du curseur

                // Quitter le programme
                if (char1 == 'Z'){
                    partie_jouees = partie_jouees + 1;
                    save_data(plays_txt, partie_jouees);
                    clear();
                    mvprintw(3, 0, "- STATISTIQUES :\n");
                    printw("Partie Jouées : %d\nPartie Gagnées (Joueur A) : %d\nPartie Gagnées (Joueur B) : %d\n", partie_jouees, partie_gagneesA, partie_gagneesB);
                    refresh();
                    sleep(5);
                    return 0;
                }

                // Vérifier si le caractère est entre 'A' et 'F' et si le chiffre est entre '0' et '9'
                if ((char1 < 'A' || char1 > 'J') || (char2 < '0' || char2 > '9')){
                    mvprintw(4, 55,"La saisie n'est pas valide. Réessayez.\n");
                    refresh();
                    sleep(1);
                    clear_text(4,55,4,getmaxx(stdscr));
                    clear_text(3,55,3,getmaxx(stdscr));
                    curs_set(1);
                }

                for(int i = 0; i < NOMBRE_CASES_GRILLE; i++){
                    if(char1 == liste_joueurs[iteration]->list_char1[i] && char2 == liste_joueurs[iteration]->list_char2[i])
                    {
                        pareil = 1;
                        mvprintw(4, 55, "Vous avez déjà saisi cette combinaison auparavant !\n");
                        refresh();
                        sleep(1);
                        clear_text(4,55,4,getmaxx(stdscr));
                        clear_text(3,55,3,getmaxx(stdscr));
                        curs_set(1);
                    }
                }
            } while (((char1 < 'A' || char1 > 'J') || (char2 < '0' || char2 > '9')) || pareil);

            liste_joueurs[iteration]->list_char1[nombre_tour - 1] = char1;
            liste_joueurs[iteration]->list_char2[nombre_tour - 1] = char2;

            int indiceLigne = char2 - '0' + 2;
            int indiceColonne = char1 - 'A' + 2;

            int bateau_trouve = 0;
            // 'iteration' représente l'un des deux joueurs (0 = Joueur A et 1 = Joueur B)
            /* Si iteration = 0, index = 2 - 1 - 0 = 1 pour cibler le joueur B à l'index 1
               Si iteration = 1, index = 2 - 1 - 1 = 0 pour cibler le joueur A à l'index 0 */
            for (int i = 0; i < NOMBRE_BATEAUX; i++){
                for (int j = 0; j < MAX_TAILLE; j++){
                    if (indiceLigne == liste_joueurs[2 - 1 - iteration]->liste_bateaux[i]->cases[j].ligneCase &&
                        indiceColonne == liste_joueurs[2 - 1 - iteration]->liste_bateaux[i]->cases[j].colonneCase){
                            found_ship = liste_joueurs[2 - 1 - iteration]->liste_bateaux[i];
                            bateau_trouve = 1;
                            break;
                    }
                }
                if (bateau_trouve)
                    break;
            }

            switch(liste_joueurs[2-1-iteration]->grille_bateau[indiceLigne][indiceColonne])
            {
                case BATEAU:
                    if((found_ship->compteur - 1) == 0){
                        found_ship->compteur -= 1;
                        found_ship->isDestroyed = 1;
                        liste_joueurs[iteration]->grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                        attron(COLOR_PAIR(1));
                        mvprintw(4, 55, "Touché coulé !");
                        attroff(COLOR_PAIR(1));
                        refresh();
                        sleep(1);
                    }
                    else{
                        found_ship->compteur -= 1;
                        liste_joueurs[iteration]->grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                        attron(COLOR_PAIR(1));
                        mvprintw(4, 55, "Touché !");
                        attroff(COLOR_PAIR(1));
                        refresh();
                        sleep(1);
                    }
                    break;
                case CASE_VIDE:
                    liste_joueurs[iteration]->grille[indiceLigne][indiceColonne] = TIR_MANQUE;
                    attron(COLOR_PAIR(2));
                    mvprintw(4, 55, "Raté !");
                    attroff(COLOR_PAIR(2));
                    refresh();
                    sleep(1);
                    break;
            }
        }

        sleep(1);
        nombre_tour += 1;

        int result = print_result(nombre_tour, max_tour);
        
        if(result == -1){
            print_stats(partie_jouees, plays_txt, partie_gagneesA, partie_gagneesB);
            return 0;
        }
        if(result == -2){
            partie_gagneesA = partie_gagneesA + 1;
            save_data(winsA_txt, partie_gagneesA);
            print_stats(partie_jouees, plays_txt, partie_gagneesA, partie_gagneesB);
            return 0;
        }
        if (result == -3){
            partie_gagneesB = partie_gagneesB + 1;
            save_data(winsB_txt, partie_gagneesB);
            print_stats(partie_jouees, plays_txt, partie_gagneesA, partie_gagneesB);
            return 0;
        }
    }
    endwin();
    return EXIT_SUCCESS;
}