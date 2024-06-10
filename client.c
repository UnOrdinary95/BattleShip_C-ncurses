// gcc a_client.c -o a_client -lncurses && ./a_client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <ncurses.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "organisation.h"
#include "function.h"


void clear_text(int starty, int startx, int endy, int endx) {
    int y, x;

    // Obtenir les dimensions de la fenêtre ncurses
    int maxy = getmaxy(stdscr);
    int maxx = getmaxx(stdscr);

    // Limiter les coordonnées pour éviter un dépassement des limites de la fenêtre
    starty = (starty < 0) ? 0 : (starty >= maxy) ? maxy - 1 : starty;   
    // Si starty est inférieur à 0, starty = 0.
    // Sinon, si starty est supérieur ou égal à maxy, starty = maxy - 1.
    // Sinon starty = starty.
    startx = (startx < 0) ? 0 : (startx >= maxx) ? maxx - 1 : startx;
    endy = (endy < 0) ? 0 : (endy >= maxy) ? maxy - 1 : endy;
    endx = (endx < 0) ? 0 : (endx >= maxx) ? maxx - 1 : endx;   

    // Effacer la partie du texte
    for (y = starty; y <= endy; y++) {
        for (x = startx; x <= endx; x++) {
            move(y, x);
            printw(" "); // Écrire un espace pour effacer le caractère existant
        }
    }
    refresh(); // Rafraîchir l'écran pour afficher les modifications
}

// Choix du nom des joueurs
void enter_name(Joueur *Joueur_A){
    char nom_A[12];
    
    printw("\nSaississez votre pseudo : ");
    getnstr(nom_A, sizeof(nom_A));  // Permet de saisir une chaine de caractère

    strcpy((*Joueur_A).nom, nom_A);
}

// Affiche la grille
void print_grid(char uneGrille[][DIMENSION]){
    for(int i = 0; i < DIMENSION; i++)
    {
        for(int j = 0; j < DIMENSION; j++)
        {
            if(uneGrille[i][j] == BORDURE)
                printw("   ");
            else if(uneGrille[i][j] == CASE_VIDE)
                printw(" - ");
            else if(uneGrille[i][j] == TIR_REUSSI){
                attron(COLOR_PAIR(1));  // ROUGE
                printw(" X ");
                attroff(COLOR_PAIR(1));
            }
            else if(uneGrille[i][j] == TIR_MANQUE){
                attron(COLOR_PAIR(2));  // BLEU
                printw(" O ");  // ·
                attroff(COLOR_PAIR(2));
            }
            else if(uneGrille[i][j] == BATEAU)
                printw(" o ");
            else
                printw(" %c ",uneGrille[i][j]);
        }
        printw("\n");
    }
    refresh();
}

// Copie la grille de base
void copy_grid() {
    // Utilisation de memcpy pour copier le tableau "grille_A"
    memcpy(grille_A_bateaux, grille_A, sizeof(grille_A));
}

// Place un bateau d'un joueur dans la grille (En effectuant des saisies de coordonnées des cases du bateau)
void ship_positioning(Joueur unJoueur, Bateau *unBateau, char grille_bateau[][DIMENSION]){
    char char1, char2;
    int indiceLigne, indiceColonne;
    int indiceLigne2, indiceColonne2;
    int indiceLigne3, indiceColonne3;
    int issue_possible[4] = {1, 1, 1, 1}; // HAUT // BAS // GAUCHE // DROITE

    // Placement de la première case
    while(1){
        mvprintw(3, 55, "%s, entre les coordonnées de la première case du bateau (LETTRE/CHIFFRE) : ", unJoueur.nom);
        char1 = getch();
        char2 = getch();
        curs_set(0);
        // scanf(" %c %c", &char1, &char2);

        // CAS 1 : 
        // Vérifier si le caractère est entre 'A' et 'F' et si le chiffre est entre '0' et '9'
        if ((char1 < 'A' || char1 > 'J') || (char2 < '0' || char2 > '9')){
            mvprintw(4, 55, "La saisie n'est pas valide. Réessayez.\n");
            refresh();
            sleep(1);
            clear_text(4,55,4,getmaxx(stdscr));
            clear_text(3,55,3,getmaxx(stdscr));
            curs_set(1);
            continue;
        }

        // CAS 2 :
        indiceLigne = char2 - '0' + 2;
        indiceColonne = char1 - 'A' + 2;

        if (grille_bateau[indiceLigne][indiceColonne] == BATEAU){
            mvprintw(4, 55, "Cette case est une case d'un bateau !\n");
            refresh();
            sleep(1);
            clear_text(4,55,4,getmaxx(stdscr));
            clear_text(3,55,3,getmaxx(stdscr));
            curs_set(1);
            continue;
        }

        // CAS 3 :
        // Vers le bas
        if (indiceLigne + (unBateau->taille - 1) > MAX_GRILLE)
            issue_possible[1] = 0;
        else{
            for (int i = 1; i < unBateau->taille; i++){
                if (grille_bateau[indiceLigne + i][indiceColonne] == BATEAU)
                    issue_possible[1] = 0;
            }
        }
        // Vers la droite
        if (indiceColonne + (unBateau->taille - 1) > MAX_GRILLE)
            issue_possible[3] = 0;
        else{
            for (int i = 1; i < unBateau->taille; i++){
                if (grille_bateau[indiceLigne][indiceColonne + i] == BATEAU)
                    issue_possible[3] = 0;
            }
        }
        // Vers le haut
        if (indiceLigne - (unBateau->taille - 1) < MIN_GRILLE)
            issue_possible[0] = 0;
        else{
            for (int i = 1; i < unBateau->taille; i++){
                if (grille_bateau[indiceLigne - i][indiceColonne] == BATEAU)
                    issue_possible[0] = 0;
            }
        }
        // Vers la gauche
        if (indiceColonne - (unBateau->taille - 1) < MIN_GRILLE)
            issue_possible[2] = 0;
        else{
            for (int i = 1; i < unBateau->taille; i++){
                if (grille_bateau[indiceLigne][indiceColonne - i] == BATEAU)
                    issue_possible[2] = 0;
            }
        }

        // CAS 4 :
        if (issue_possible[0]+issue_possible[1]+issue_possible[2]+issue_possible[3] == 0)
            {
                mvprintw(4, 55, "Dans cette case, aucune issue n'est possible !\n");
                refresh();
                sleep(1);
                clear_text(4,55,4,getmaxx(stdscr));
                clear_text(3,55,3,getmaxx(stdscr));
                curs_set(1);
                continue;
            }
        else{
            unBateau->cases[0].ligneCase = indiceLigne;
            unBateau->cases[0].colonneCase = indiceColonne;
            break;
        }
    }

    // Placement de la deuxième case
    while(1){
        while(1){
            mvprintw(4, 55, "%s, entre les coordonnées de la deuxième case du bateau (LETTRE/CHIFFRE) : ", unJoueur.nom);
            char1 = getch();
            char2 = getch();
            curs_set(0);
            // scanf(" %c %c", &char1, &char2);

            indiceLigne2 = char2 - '0' + 2;
            indiceColonne2 = char1 - 'A' + 2;

            if (grille_bateau[indiceLigne2][indiceColonne2] == BATEAU){
                mvprintw(5, 55, "Cette case est une case d'un bateau !\n");
                refresh();
                sleep(1);
                clear_text(5,55,5,getmaxx(stdscr));
                clear_text(4,55,4,getmaxx(stdscr));
                curs_set(1);
                continue;
            }

            // Regarde si la case est possible grâce au tableau des issues possible
            if (issue_possible[0] == 1 && indiceLigne - 1 == indiceLigne2 && indiceColonne == indiceColonne2){
                indiceLigne3 = indiceLigne2 - 1;
                indiceColonne3 = indiceColonne2;
                break;  // Pour ça qu'on fait un while dans un while
            }
            else if (issue_possible[1] == 1 && indiceLigne + 1 == indiceLigne2 && indiceColonne == indiceColonne2){
                indiceLigne3 = indiceLigne2 + 1;
                indiceColonne3 = indiceColonne2;
                break;
            }
            else if (issue_possible[2] == 1 && indiceColonne - 1 == indiceColonne2 && indiceLigne == indiceLigne2){
                indiceColonne3 = indiceColonne2 - 1;
                indiceLigne3 = indiceLigne2;
                break;
            }
            else if (issue_possible[3] == 1 && indiceColonne + 1 == indiceColonne2 && indiceLigne == indiceLigne2){
                indiceColonne3 = indiceColonne2 + 1;
                indiceLigne3 = indiceLigne2;
                break;
            }
            else{
                mvprintw(5, 55, "La saisie n'est pas valide. Réessayez.\n");
                refresh();
                sleep(1);
                clear_text(5,55,5,getmaxx(stdscr));
                clear_text(4,55,4,getmaxx(stdscr));
                curs_set(1);
                continue;
            }
        }
        unBateau->cases[1].ligneCase = indiceLigne2;
        unBateau->cases[1].colonneCase = indiceColonne2;
        break;
    }

    // Placement de la troisième case
    int test_indice_l, test_indice_c;
    do{
        mvprintw(5, 55, "%s, entre les coordonnées de la troisième case du bateau (LETTRE/CHIFFRE) : ", unJoueur.nom);
        char1 = getch();
        char2 = getch();
        curs_set(0);
        // scanf(" %c %c", &char1, &char2);

        test_indice_l = char2 - '0' + 2;
        test_indice_c = char1 - 'A' + 2;
        if (test_indice_l != indiceLigne3 || test_indice_c != indiceColonne3){
            mvprintw(6, 55, "La saisie n'est pas valide. Réessayez.\n");
            refresh();
            sleep(1);
            clear_text(6,55,6,getmaxx(stdscr));
            clear_text(5,55,5,getmaxx(stdscr));
            curs_set(1);
        }
    } while (test_indice_l != indiceLigne3 || test_indice_c != indiceColonne3);

    unBateau->cases[2].ligneCase = indiceLigne3;
    unBateau->cases[2].colonneCase = indiceColonne3;
}

// Ajoute un bateau dans la grille
void add_ship(char grille_bateau[][DIMENSION], Bateau *unBateau){
    for (int i = 0; i < 3; i++){
        grille_bateau[unBateau->cases[i].ligneCase][unBateau->cases[i].colonneCase] = BATEAU;
    }
}

// Permet aux joueurs de spécifier les emplacements des bateaux au début du jeu.
void print_positioning_function(){
    char nombre_ordinaux[3][10] = {"premier", "deuxième", "troisième"};

    clear();
    refresh();
    move(2, 0);

    printw("--------Placement des bateaux--------\n");
    print_grid(Joueur_A.grille_bateau);
    for (int j = 0; j < 3; j++){
        mvprintw(2, 55, "%s, tu vas placer le %s bateau.\n", Joueur_A.nom, nombre_ordinaux[j]);
        ship_positioning(Joueur_A, Joueur_A.liste_bateaux[j], Joueur_A.grille_bateau); 
        add_ship(Joueur_A.grille_bateau, Joueur_A.liste_bateaux[j]);

        clear();
        move(2, 0);
        printw("--------Placement des bateaux--------\n");
        print_grid(Joueur_A.grille_bateau);
    }
    clear();
    refresh();
}

//////// FONCTION SOCKETS

// Permet de convertir un entier en un tableau contenant l'entier
int* int_to_array(int entier) {
    int *array = malloc(sizeof(int)); // Allouer de la mémoire pour un seul entier
    
    array[0] = entier; // Initialiser le tableau avec l'entier
    
    return array;
}

// Envoie un message au serveur
void envoyer(int client_socket, const char *message, int entier){
    if (message != NULL){
        if (send(client_socket, message, strlen(message), 0) == -1){
            printw("Erreur lors de l'envoie de message.");
            refresh();
            getch();
            exit(EXIT_FAILURE);
        }
    }
    else{
        if (send(client_socket, &entier, sizeof(entier), 0) == -1){
            printw("Erreur lors de l'envoie de message.");
            refresh();
            getch();
            exit(EXIT_FAILURE);
        }
    }
}

// Reçoit un message de type string d'un client
char* recevoir_str(int client_socket){
    char buffer[TAILLE_MESSAGE];
    int bytes = recv(client_socket, buffer, TAILLE_MESSAGE, 0);

    if (bytes == - 1){
        printw("Une erreur s'est produite lors de la réception du message.");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    }
    else if (bytes == 0){
        printw("Connexion fermée par le côté distant");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    }
    else{
        buffer[bytes] = '\0'; // Ajoute le terminateur NULL pour indiquer la fin du chaine de caractère
        // Copie le message dans une mémoire alloué dynamiquement 
        char* message = (char*)malloc(strlen(buffer) + 1);
        if (message == NULL) {
            printw("Une erreur est survenu durant l'allocation de la mémoire");
            refresh();
            getch();
            exit(EXIT_FAILURE);
        }
        strcpy(message, buffer); // Copie le buffer sur message
        return message;
    }
}

// Reçoit un message de type int d'un client
int recevoir_int(int client_socket) {
    int entier;
    int bytes_received;
    int bytes_left = sizeof(entier);
    char *buffer = (char*)&entier; // Pointeur de type char pour traiter les données byte par byte

    while (bytes_left > 0) {
        bytes_received = recv(client_socket, buffer, bytes_left, 0);
        if (bytes_received <= 0) {
            if (bytes_received == 0) {
                printw("Connexion fermée par le côté distant");
                refresh();
                getch();
            } else {
                printw("Une erreur s'est produite lors de la réception du message.");
                refresh();
                getch();
            }
            exit(EXIT_FAILURE);
        }
        bytes_left -= bytes_received;
        buffer += bytes_received;
    }

    return entier;
}


int jouerContreJoueurSocket() {
    // Code ici
    initscr();  // Initialisation de la fenêtre ncurses

    // Création du socket
    int client_socket;

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        printw("Erreur lors de la création du socket.");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in adresse_serveur;
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);

    // Convertir l'adresse IPv4 et la configurer dans serv_addr
    if(inet_pton(AF_INET, IP_SERVEUR, &adresse_serveur.sin_addr) == -1) {
        printw("Erreur lors de la conversion de l'adresse IP");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    }

    // Établir la connexion avec le serveur
    if (connect(client_socket, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur)) == -1) {
        printw("Une erreur s'est produite lors de la connexion au socket.");
        refresh();
        getch();
        exit(EXIT_FAILURE);
    }

    // Reçoit un message du serveur (Atteindre le serveur)
    char* message = recevoir_str(client_socket);
    mvprintw(1,0, "%s", message);
    refresh();
    free(message);
    sleep(3);
    clear();
    refresh();
/////////////////////////////////////////////////////////////////////////////////////////////
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires    
    start_color();  // Initialisation du support des couleurs
    init_pair(1, COLOR_RED, COLOR_BLACK);   // Créer la couleur Rouge
    init_pair(2, COLOR_CYAN, COLOR_BLACK);  // Créer la couleur Bleu (Cyan)

    int nombre_tour = 1, max_tour = 75;

    copy_grid();

    mvprintw(1, 1, " ____        _   _   _      ____  _     _       ");
    mvprintw(2, 1, "| __ )  __ _| |_| |_| | ___/ ___|| |__ (_)_ __  ");
    mvprintw(3, 1, "|  _ \\ / _` | __| __| |/ _ \\___ \\| '_ \\| | '_ \\ ");
    mvprintw(4, 1, "| |_) | (_| | |_| |_| |  __/___) | | | | | |_) |");
    mvprintw(5, 1, "|____/ \\__,_|\\__|\\__|_|\\___|____/|_| |_|_| .__/ ");
    mvprintw(6, 1, "                                         |_|    ");
    enter_name(&Joueur_A);
    print_positioning_function();

    // Select
    envoyer(client_socket, NULL, 1);

    // Attente de l'autre client...
    int attente = recevoir_int(client_socket);

    if (attente){
        // Message d'attente
        char* message2 = recevoir_str(client_socket);
        mvprintw(5,55,"%s\n", message2);
        refresh();
        free(message2);
        int freeze = recevoir_int(client_socket);
    }

/////////////////////////////////////////////////////////////////////////////////////////////
    // Envoie pseudo A
    envoyer(client_socket, Joueur_A.nom, 0);
    usleep(50000);
    
    // Envoie grille_bateau_A
    send(client_socket, Joueur_A.grille_bateau, sizeof(char) * DIMENSION * DIMENSION, 0);
    usleep(50000);

    // Envoie grille_A
    send(client_socket, Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION, 0);
    usleep(50000);

    // Envoie des bateaux_A
    for (int i = 0; i < NOMBRE_BATEAUX; i++){
        for (int j = 0; j < MAX_TAILLE; j++){
            envoyer(client_socket, NULL, Joueur_A.liste_bateaux[i]->cases[j].ligneCase);
            usleep(50000);
            
            envoyer(client_socket, NULL, Joueur_A.liste_bateaux[i]->cases[j].colonneCase);
            usleep(50000);
            // 9 fois
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    // Recoit pseudo B
    char* nom_B = recevoir_str(client_socket);  
    strcpy(Joueur_B.nom, nom_B);

    // Recoit grille_bateau_B
    int read_val = read(client_socket, Joueur_B.grille_bateau, sizeof(char) * DIMENSION * DIMENSION);

    // Recoit grille_B
    int read_val2 = read(client_socket, Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION);

    // Recoit bateaux_B
    for (int i = 0; i < NOMBRE_BATEAUX; i++){
        for (int j = 0; j < MAX_TAILLE; j++){
            Joueur_B.liste_bateaux[i]->cases[j].ligneCase = recevoir_int(client_socket);
            Joueur_B.liste_bateaux[i]->cases[j].colonneCase = recevoir_int(client_socket);
            // 9 fois
        }
    }
/////////////////////////////////////////////////////////////////////////////////////////////
    Bateau* found_ship;
    int fin_de_partie = 0;
    while(fin_de_partie == 0){
        mvprintw(2, 0, "[TOUR %d/%d]\n\n", nombre_tour, max_tour);
        printw("Votre grille :\n");
        print_grid(Joueur_A.grille);
        printw("\n\nGrille de %s :\n", nom_B);
        print_grid(Joueur_B.grille);
        
        mvprintw(2, 55, "Tu vas effectuer un tir.\n");    // Déplacer le curseur à la position spécifiée par les coordonnées (y, x) avant d'afficher
        refresh();
        int pareil;
        char char1 = 0, char2 = 0;

        do{
            pareil = 0;
            mvprintw(3, 55, "%s, saisit les coordonnées (LETTRE/CHIFFRE) : ", Joueur_A.nom);
            char1 = getch();
            char2 = getch();
            curs_set(0); // Désactive l'affichage du curseur

            // Quitter le programme
            if (char1 == 'Z'){
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
                if(char1 == Joueur_A.list_char1[i] && char2 == Joueur_A.list_char2[i])
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

        Joueur_A.list_char1[nombre_tour - 1] = char1;
        Joueur_A.list_char2[nombre_tour - 1] = char2;

        int indiceLigne = char2 - '0' + 2;
        int indiceColonne = char1 - 'A' + 2;

        int bateau_trouve = 0;
        
        for (int i = 0; i < NOMBRE_BATEAUX; i++){
            for (int j = 0; j < MAX_TAILLE; j++){
                if (indiceLigne == Joueur_B.liste_bateaux[i]->cases[j].ligneCase 
                && indiceColonne == Joueur_B.liste_bateaux[i]->cases[j].colonneCase){
                    found_ship = Joueur_B.liste_bateaux[i];
                    bateau_trouve = 1;
                    break;
                }
            }
            if (bateau_trouve)
                break;
        }

        switch(Joueur_B.grille_bateau[indiceLigne][indiceColonne])
        {
            case BATEAU:
                if((found_ship->compteur - 1) == 0){
                    found_ship->compteur -= 1;
                    found_ship->isDestroyed = 1;
                    Joueur_A.grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                    attron(COLOR_PAIR(1));
                    mvprintw(4, 55, "Touché coulé !");
                    attroff(COLOR_PAIR(1));
                    refresh();
                    sleep(1);
                }
                else{
                    found_ship->compteur -= 1;
                    Joueur_A.grille[indiceLigne][indiceColonne] = TIR_REUSSI;
                    attron(COLOR_PAIR(1));
                    mvprintw(4, 55, "Touché !");
                    attroff(COLOR_PAIR(1));
                    refresh();
                    sleep(1);
                }
                break;
            case CASE_VIDE:
                Joueur_A.grille[indiceLigne][indiceColonne] = TIR_MANQUE;
                attron(COLOR_PAIR(2));
                mvprintw(4, 55, "Raté !");
                attroff(COLOR_PAIR(2));
                refresh();
                sleep(1);
                break;
        }

        sleep(1);
        nombre_tour += 1;
/////////////////////////////////////////////////////////////////////////////////////////////
        // Envoie le tour actuelle
        envoyer(client_socket, NULL, nombre_tour);

        // Attente de l'autre client...
        int attente = recevoir_int(client_socket);

        if (attente){
            // Message d'attente
            char* message2 = recevoir_str(client_socket);
            mvprintw(5,55,"%s\n", message2);
            refresh();
            free(message2);
        }
/////////////////////////////////////////////////////////////////////////////////////////////
        int bateaux_detruite = 0;

        for(int j = 0; j < NOMBRE_BATEAUX; j++){
            if(Joueur_B.liste_bateaux[j]->isDestroyed == 1){
                bateaux_detruite += 1;
            }
        }

        if (bateaux_detruite > 0){
            envoyer(client_socket, NULL, 1);
        }
        else{
            envoyer(client_socket, NULL, 0);
        }

        fin_de_partie = recevoir_int(client_socket);
        if (fin_de_partie == 1){
            clear();
            refresh();
            move(2,0);
        }
        else{
            move(6,55);
        }

        char* message3 = recevoir_str(client_socket);
        printw("%s", message3);
        refresh();
        free(message3);

        sleep(1);

        // Envoie grille_A
        send(client_socket, Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION, 0);

        // Recoit grille_B
        read_val2 = read(client_socket, Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION);  
    }
    sleep(2);
    clear();
    mvprintw(2,0,"FIN CLIENT !");
    refresh();
    sleep(2);
    endwin();
    close(client_socket);

    return 0;
}