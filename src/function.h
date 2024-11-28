#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "organisation.h"

// Déclarations des fonctions
extern void clear_text(int starty, int startx, int endy, int endx);
extern void enter_name(Joueur *Joueur_A);
extern void print_grid(char uneGrille[][DIMENSION]);
extern void copy_grid();
extern void ship_positioning(Joueur unJoueur, Bateau *unBateau, char grille_bateau[][DIMENSION]);
extern void add_ship(char grille_bateau[][DIMENSION], Bateau *unBateau);
extern void print_positioning_function();
extern int* int_to_array(int entier);
extern void envoyer(int client_socket, const char *message, int entier);
extern char* recevoir_str(int client_socket);
extern int recevoir_int(int client_socket);

#endif /* FUNCTIONS_H */
