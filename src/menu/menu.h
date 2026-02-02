#ifndef MENU_H 
#define MENU_H  

/*
Module : menu
Rôle : Ce fichier contient les prototypes des fonctions utiles à l'affichage
et la gestion des menus.

Auteur : Kylian
Date : 29/01/2026
*/

// fonction principale du menu
void demarrer_menu(void);

// fonctions des sous-menus
void menu_mode_utilisateur(void);

void menu_mode_administrateur(void);

// autres fonctions
void afficher_menu(const char *titre, const char *options[], int nombre_options);


int obtenir_entier_utilisateur(int nombre_max);

int verif_mdp();

#endif // fin de ifndef 