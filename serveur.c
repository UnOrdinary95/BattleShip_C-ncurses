// gcc a_serveur.c -o a_serveur && ./a_serveur

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "organisation.h"
#include "function.h"


// Envoie un message à ou aux client(s)
void s_envoyer(int client_sockets[], int nbr_connexion, const char *message, int entier){
    if (message != NULL){
        for (int i = 0; i < nbr_connexion; i++){
            if (send(client_sockets[i], message, strlen(message), 0) == -1){
                perror("Erreur lors de l'envoie de message.");
                exit(EXIT_FAILURE);
            }
        }
    }
    else{
        for (int i = 0; i < nbr_connexion; i++){
            if (send(client_sockets[i], &entier, sizeof(entier), 0) == -1){
                perror("Erreur lors de l'envoie de message.");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int lancerServeur(){
    printf("Lancement du serveur...\n");
    
    fd_set readfds; // ensemble des sockets en attente de lecture
    int max_sd;     // plus grand descripteur de fichier
    int activity;   // résultat de la fonction select()

    // Création du socket
    int serveur_socket;

    if ((serveur_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Erreur lors de la création du socket.");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    struct sockaddr_in adresse_serveur;
    adresse_serveur.sin_family = AF_INET;
    adresse_serveur.sin_port = htons(PORT);
    adresse_serveur.sin_addr.s_addr = INADDR_ANY;

    // Liaison du socket à l'adresse et au port
    if (bind(serveur_socket, (struct sockaddr *)&adresse_serveur, sizeof(adresse_serveur)) == -1) {
        perror("Erreur lors de la liaison du socket.");
        exit(EXIT_FAILURE);
    }

    // Mise en écoute du socket
    if (listen(serveur_socket, NOMBRE_CLIENT) == -1) {
        perror("Erreur lors de la mise en écoute du socket.");
        exit(EXIT_FAILURE);
    }

    printf("Serveur en attente de connexions...\n\n");

    // Accepter les connexions entrantes
    int nbr_connexion = 0, client_sockets[2];

    while (nbr_connexion != NOMBRE_CLIENT){
        if ((client_sockets[nbr_connexion] = accept(serveur_socket, NULL, NULL)) == -1) {
            perror("Erreur lors de l'acceptation de la connexion.");
            exit(EXIT_FAILURE);
        }
        printf("Client %d connecté !\n", nbr_connexion + 1);
        nbr_connexion += 1;
    }

    s_envoyer(client_sockets, NOMBRE_CLIENT, "[Serveur] - Tu as atteint le serveur !", 0);

    FD_ZERO(&readfds); // Initialisation de l'ensemble des sockets à surveiller
    max_sd = 0;

    // Ajout de sockets actives à l'ensemble
    for (int i = 0; i < NOMBRE_CLIENT; ++i) {
        // Vérifie si le socket est valide
        if (client_sockets[i] != -1) {
            FD_SET(client_sockets[i], &readfds); // Ajout du socket à l'ensemble
            
            if (client_sockets[i] > max_sd) {
                max_sd = client_sockets[i]; // Mise à jour du plus grand descripteur
            }
        }
    }

    // Attente d'activité sur l'un des sockets
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if (activity < 0) {
        perror("Une erreur est survenu lors de la sélection de client");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NOMBRE_CLIENT; ++i) {
        if (FD_ISSET(client_sockets[i], &readfds)) {
            // Le socket i a des données à lire
            int chiffre_A = recevoir_int(client_sockets[i]);

            s_envoyer(int_to_array(client_sockets[i]), 1, NULL, 1);   // Le premier client à avoir send le nombre, on souhaite lui afficher un message disant de patienter
            s_envoyer(int_to_array(client_sockets[2 - 1 - i]), 1, NULL, 0);   // Tandis que l'autre non
            s_envoyer(int_to_array(client_sockets[i]), 1, "[Serveur] - En attente de l'autre joueur...", 0);  // On envoie le message à lui personnellement

            int chiffre_B = recevoir_int(client_sockets[2 - 1 - i]);    // On attend la réponse du deuxième client même si elle ne nous sert à rien
            s_envoyer(int_to_array(client_sockets[i]), 1, NULL, 1);   // Pour permettre d'enlever le freeze   
            break;
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////
    // RECEVOIR
    // Pseudo
    char* nom_A = recevoir_str(client_sockets[0]);
    strcpy(Joueur_A.nom, nom_A);
    char* nom_B = recevoir_str(client_sockets[1]);
    strcpy(Joueur_B.nom, nom_B);

    // Grille_bateau
    int read_bgrille_A = read(client_sockets[0], Joueur_A.grille_bateau, sizeof(char) * DIMENSION * DIMENSION);
    int read_bgrille_B = read(client_sockets[1], Joueur_B.grille_bateau, sizeof(char) * DIMENSION * DIMENSION);

    // Grille
    int read_grille_A = read(client_sockets[0], Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION);
    int read_grille_B = read(client_sockets[1], Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION);

    // Bateaux
    for (int i = 0; i < NOMBRE_BATEAUX; i++){
        for (int j = 0; j < MAX_TAILLE; j++){
            Joueur_A.liste_bateaux[i]->cases[j].ligneCase = recevoir_int(client_sockets[0]);
            Joueur_B.liste_bateaux[i]->cases[j].ligneCase = recevoir_int(client_sockets[1]);  

            Joueur_A.liste_bateaux[i]->cases[j].colonneCase = recevoir_int(client_sockets[0]);
            Joueur_B.liste_bateaux[i]->cases[j].colonneCase = recevoir_int(client_sockets[1]);
            // 9 fois
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////
    // SEND
    // Pseudo
    s_envoyer(int_to_array(client_sockets[0]), 1, Joueur_B.nom, 0);
    s_envoyer(int_to_array(client_sockets[1]), 1, Joueur_A.nom, 0);

    usleep(50000);

    // Grille_bateau
    send(client_sockets[0], Joueur_B.grille_bateau, sizeof(char) * DIMENSION * DIMENSION, 0);
    send(client_sockets[1], Joueur_A.grille_bateau, sizeof(char) * DIMENSION * DIMENSION, 0);

    usleep(50000);

    // Grille
    send(client_sockets[0], Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION, 0);
    send(client_sockets[1], Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION, 0);

    usleep(50000);

    // Bateaux
    for (int i = 0; i < NOMBRE_BATEAUX; i++){
        for (int j = 0; j < MAX_TAILLE; j++){
            s_envoyer(int_to_array(client_sockets[0]), 1, NULL, Joueur_B.liste_bateaux[i]->cases[j].ligneCase);
            s_envoyer(int_to_array(client_sockets[1]), 1, NULL, Joueur_A.liste_bateaux[i]->cases[j].ligneCase);

            usleep(50000);

            s_envoyer(int_to_array(client_sockets[0]), 1, NULL, Joueur_B.liste_bateaux[i]->cases[j].colonneCase);
            s_envoyer(int_to_array(client_sockets[1]), 1, NULL, Joueur_A.liste_bateaux[i]->cases[j].colonneCase);

            usleep(50000);
            // 9 fois
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////
    int max_tour = 75, nombre_tour, nbr_inutilisable, fin_de_partie = 0;

    while(fin_de_partie == 0){
        FD_ZERO(&readfds); // Initialisation de l'ensemble des sockets à surveiller
        max_sd = 0;

        // Ajout de sockets actives à l'ensemble
        for (int i = 0; i < NOMBRE_CLIENT; ++i) {
            // Vérifie si le socket est valide
            if (client_sockets[i] != -1) {
                FD_SET(client_sockets[i], &readfds); // Ajout du socket à l'ensemble
                
                if (client_sockets[i] > max_sd) {
                    max_sd = client_sockets[i]; // Mise à jour du plus grand descripteur
                }
            }
        }

        // Attente d'activité sur l'un des sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("Une erreur est survenu lors de la sélection de client");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < NOMBRE_CLIENT; ++i) {
            if (FD_ISSET(client_sockets[i], &readfds)) {
                // Le socket i a des données à lire
                nombre_tour = recevoir_int(client_sockets[i]);

                s_envoyer(int_to_array(client_sockets[i]), 1, NULL, 1);   // Le premier client à avoir send le nombre, on souhaite lui afficher un message disant de patienter
                s_envoyer(int_to_array(client_sockets[2 - 1 - i]), 1, NULL, 0);   // Tandis que l'autre non
                s_envoyer(int_to_array(client_sockets[i]), 1, "[Serveur] - En attente de l'autre joueur...", 0);  // On envoie le message à lui personnellement

                nbr_inutilisable = recevoir_int(client_sockets[2 - 1 - i]);    // On attend la réponse du deuxième client même si elle ne nous sert à rien
                break;
            }
        }
        ///////////////////////////////////////////////////////////////////////////
        int bateaux_detruite[2], fin_de_partie = 0;

        for (int i = 0; i < NOMBRE_CLIENT; i++){
            bateaux_detruite[i] = recevoir_int(client_sockets[i]);
            if (bateaux_detruite[i] == 1){
                fin_de_partie = 1;
                s_envoyer(client_sockets, NOMBRE_CLIENT, NULL, fin_de_partie);
                usleep(50000);
                char message_complet[100] = "";
                char* message_fin = " a gagner cette partie !";
                strcat(message_complet, liste_joueurs[i]->nom);
                strcat(message_complet, message_fin);
                s_envoyer(client_sockets, NOMBRE_CLIENT, message_complet, 0);
                break;
            }
        }
        if (nombre_tour > max_tour && bateaux_detruite[0] == 0 && bateaux_detruite[1] == 0){
            fin_de_partie = 1;
            s_envoyer(client_sockets, NOMBRE_CLIENT, NULL, fin_de_partie);
            usleep(50000);
            s_envoyer(client_sockets, NOMBRE_CLIENT, "MAX TOUR atteint, personne n'a gagné durant cette partie !", 0);
        }
        if (bateaux_detruite[0] == 0 && bateaux_detruite[1] == 0 && fin_de_partie == 0){
            s_envoyer(client_sockets, NOMBRE_CLIENT, NULL, fin_de_partie);
            usleep(50000);
            s_envoyer(client_sockets, NOMBRE_CLIENT, "TOUR SUIVANTE..", 0);
        }

        // Grille (RECEVOIR)
        int read_grille_A = read(client_sockets[0], Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION);
        int read_grille_B = read(client_sockets[1], Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION);

        // Grille (send)
        send(client_sockets[0], Joueur_B.grille, sizeof(char) * DIMENSION * DIMENSION, 0);
        send(client_sockets[1], Joueur_A.grille, sizeof(char) * DIMENSION * DIMENSION, 0);
    }
    sleep(5);

    printf("\nFIN SERVEUR\n");
    free(nom_A);
    free(nom_B);
    close(client_sockets[0]);
    close(client_sockets[1]);
    close(serveur_socket);

    return 0;
}