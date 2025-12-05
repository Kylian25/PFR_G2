#include <stdio.h>
#include <stdlib.h> 
#include "menu.h"
#include "../outils/outils.h"

// debut du menu
void demarrer_menu() {
    int choix;
    const char *options[] = {   // tableau de pointeurs qui pointent vers une chaine de caracteres (constantes)
        "1 : Mode Utilisateur",
        "2 : Mode Administrateur",
        "3 : Quitter"
    };
    int nb_options = 3;

    do {

        afficher_menu("Menu Principal - Choix du Mode", options, nb_options);

        choix = obtenir_choix_utilisateur(nb_options);

        switch (choix) {
            case 1:
                printf("\n=====> Entree en Mode Utilisateur...\n");
                menu_mode_utilisateur();
                break;
            case 2:
                printf("\n=====> Entree en Mode Administrateur...\n");
                menu_mode_administrateur();
                break;
            case 3:
                printf("\nArret du programme..\n");
                return; 
            default:
                printf("\nChoix invalide. Réessayez.\n");
                break;
        }
    } while (choix != 3);
}

// ########################################################################

// menu du mode utilisateur : choix du type de requete et de la langue.
void menu_mode_utilisateur() {

    int choix;

    const char *options[] = {
        "1 : Mode Requete Textuelle",
        "2 : Mode Requete Vocale",
        "3 : Choisir la Langue",
        "4 : Retour au Menu Principal"
    };

    int nb_options = 4;

    do {
        afficher_menu("Mode Utilisateur", options, nb_options);
        
        choix = obtenir_choix_utilisateur(nb_options);

        switch (choix) {
            case 1:
                printf("\n[UTILISATEUR] Activation du Mode Textuel\n");
                // Appeler ici la fonction de gestion des commandes texte (e.g., communication/gerer_texte.c)
                break;
            case 2:
                printf("\n[UTILISATEUR] Activation du Mode Vocal\n");
                // Appeler ici la fonction de gestion des commandes vocales
                break;
            case 3:
                printf("\n[UTILISATEUR] Changement de langue (Defaut: Francais)\n");
                // Appeler ici le sous-menu de choix de langue
                break;
            case 4:
                printf("\nRetour au Menu Principal.\n");
                return; 
            default:
                printf("\nChoix invalide.\n");
                break;
        }
    } while (choix != 4);
}

// #####################################################################

//  Menu du mode Administrateur : modification de la configuration et logs.
void menu_mode_administrateur() {

    int choix;

    const char *options[] = {
        "1 : Modifier le Fichier de Configuration",
        "2 : Afficher les Logs Systemes",
        "3 : Retour au Menu Principal"
    };

    int nb_options = 3;

    do {
        afficher_menu("Mode Administrateur", options, nb_options);
        
        choix = obtenir_choix_utilisateur(nb_options);

        switch (choix) {
            case 1:
                printf("\n[ADMIN] Lancement de l'editeur de configuration\n");
                // Appeler ici la fonction de gestion de la configuration (e.g., gestionnaire_config_menu())
                break;
            case 2:
                printf("\n[ADMIN] Affichage des logs\n");
                // Appeler ici la fonction pour lire et afficher le fichier de logs
                break;
            case 3:
                printf("\nRetour au Menu Principal\n");
                return; 
            default:
                printf("\nChoix invalide\n");
                break;
        }
    } while (choix != 3);
}


// ---- autres fonctions ---

// affiche un menu avec un titre et des options.
void afficher_menu(const char *titre, const char *options[], int nb_options) {

    printf("\n#############################################\n");
    printf("        %s\n", titre);
    printf("#############################################\n");

    for (int i = 0; i < nb_options; i++) {
        printf("%s\n", options[i]);
    }

    printf("--------------------------------------------\n");
    printf("Entrez votre choix : ");
}

/*
 Lit l'entree utilisateur et s'assure qu'elle est valide.
 Choix_maximum = Le nombre maximum d'options valides.
 Retourne : le choix valide de l'utilisateur (ou 0 si invalide).
 */
int obtenir_choix_utilisateur(int choix_maximum) {

    int choix;
    int resultat_lecture;

    // lit l'entier
    resultat_lecture = scanf("%d", &choix);

    if (resultat_lecture != 1) {   // scanf retourne le nb d'éléments lus (icion veut 1 (entier)), 0 si elle n'a pas lu un entier et <0 si erreur
        int c;
        c=getchar();
        while (c != '\n' && c != EOF); // c =! du retour a la ligne et de la fin du fichier(EOF)
        return 0; // Invalide
    }
    
    int caractere;
    caractere = getchar();
    while (caractere != '\n' && caractere != EOF);

    if (choix < 1 || choix > choix_maximum) { // || = ou 
        return 0;
    }
    return choix;
}