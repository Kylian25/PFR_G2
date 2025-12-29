#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "menu.h"
#include "../outils/outils.h"
#include "../config/config.h"

// debut du menu
void demarrer_menu() {

    system("clear"); // reset le terminal
    int choix;
    const char * options[] = {   // tableau de pointeurs qui pointent vers une chaine de caracteres (constantes)
        "1 : Mode Utilisateur",
        "2 : Mode Administrateur",
        "3 : Quitter"
    };
    int nb_options = 3;
    int mdp_valide;

    system("clear");
    do {

        afficher_menu("Menu Principal - Choix du Mode", options, nb_options);

        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear");
                printf("\n=====> Entree en Mode Utilisateur...\n");
                ajout_log("==> Entree en mode Utilisateur");

                menu_mode_utilisateur();
                break;
            case 2:
                mdp_valide = verif_mdp();
                if (mdp_valide){

                    system("clear");
                    printf("\n=====> Entree en Mode Administrateur...\n");
                    ajout_log("Mot de passe Valide ");
                    ajout_log("==> Entree en Mode Administrateur");
                    system("chmod ug+rw configuration/parametres"); // modifie les droits du fichier parametres pour modification
                    menu_mode_administrateur();
                    break;
                }
                else {
                    printf("Mauvais mot de passe\n");
                    ajout_log("Mauvais mot de passe rentré");
                    break;
                }
            case 3:
                system("clear");
                system("chmod ug+w configuration/parametres");
                printf("\nArret du programme..\n");
                ajout_log("Quitter le programme");
                return; 
            default:
                system("clear"); // reset le terminal 

                printf("\nChoix invalide. Réessayez.\n");
                ajout_log("Choix invalide (menu principal)");
                break;
        }
    } while (choix != 3);
}

// ########################################################################

// menu du mode utilisateur : choix du type de requete et de la langue.
void menu_mode_utilisateur() {

    int choix;

    system("chmod ug-w configuration/parametres");  // supprimer les droits de modif des parametres
    const char *options[] = {
        "1 : Mode Requete Textuelle",
        "2 : Mode Requete Vocale",
        "3 : Choisir la Langue (defaut: Francais)",
        "4 : Retour au Menu Principal"
    };

    int nb_options = 4;

    do {
        afficher_menu("   Mode Utilisateur", options, nb_options);
        
        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear"); // reset le terminal

                printf("\n[UTILISATEUR] Mode Textuel\n");
                ajout_log("[UTILISATEUR] Choix du mode Textuel");
                strcpy(config.type_requete, "texte");
                //sauvegarder_config();
                // appel fonction de gestion des commandes texte
                break;
            case 2:
                system("clear"); // reset le terminal 

                printf("\n[UTILISATEUR] Mode Vocal\n");
                ajout_log("[UTILISATEUR] Choix du mode Vocal");
                strcpy(config.type_requete, "vocale");
                //sauvegarder_config();
                // appel fonction de gestion des commandes vocales
                break;
            case 3:
                system("clear"); // reset le terminal 

                printf("\n[UTILISATEUR] Changement de langue\n");
                ajout_log("[UTILISATEUR] Choix de la langue");
                choix_langue();
                break;
            case 4:
                system("clear"); // reset le terminal 

                printf("\nRetour au Menu Principal.\n");
                ajout_log("[UTILISATEUR] Retour au menu principal");
                system("clear");
                return; 
            default:
                system("clear"); // reset le terminal 

                printf("\nChoix invalide.\n");
                ajout_log("[UTILISATEUR] Choix invalide");
                break;
        }
    } while (choix != 4);
}

// #####################################################################

// Menu modif config 

void menu_modifier_config(){

    int nb_choix = 3;
    int choix;

    const char * options[] ={
        "1 : Afficher parametres",
        "2 : Modifier parametres",
        "3 : Revenir au menu amdinistrateur"
    };

    do{

        afficher_menu("Mode administrateur - modification configuration", options, nb_choix);

        choix = obtenir_entier_utilisateur(nb_choix);

        switch (choix){

            case 1:
                system("clear");

                printf("\n[ADMIN] Affichage des parametres\n");
                ajout_log("[ADMIN] Affichage des parametres");
                
                afficher_fichier_config();
                break;

            case 2:
                system("clear");
                afficher_fichier_config();
                printf("[ADMIN] Modification des parametres");
                ajout_log("[ADMIN] Modification des parametres");
                modifier_config();
                break;
            case 3:
                system("clear"); // reset le terminal 

                printf("\nRetour au Menu administrateur\n");
                ajout_log("Retour au menu administrateur");
                return; 
            default:
                system("clear"); // reset le terminal 

                printf("\nChoix invalide\n");
                ajout_log("[ADMIN] Choix invalide");
                break;
        }

    }while (choix!=3);

}



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
        afficher_menu("   Mode Administrateur", options, nb_options);
        
        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear");
                printf("\n[ADMIN] Lancement de l'editeur de configuration\n");
                ajout_log("[ADMIN] Lancement de l'editeur de configuration");
                menu_modifier_config();
                
                break;
            case 2:
                system("clear");
                printf("\n[ADMIN] ------------ Affichage des logs -------------\n\n");
                ajout_log("[ADMIN] Affichage des logs");
                affiche_logs();
                break;
            case 3:
                system("clear");
                printf("\nRetour au Menu Principal\n");
                ajout_log("Retour au menu principal");
                return; 
            default:
                system("clear"); // reset le terminal 
                printf("\nChoix invalide\n");
                ajout_log("[ADMIN] Choix invalide");
                break;
        }
    } while (choix != 3);
}




// ---- autres fonctions ---

// affiche un menu avec un titre et des options.
void afficher_menu(const char *titre, const char *options[], int nb_options) {

    printf("\n##############################################################\n");
    printf("                   %s\n", titre);
    printf("###############################################################\n");

    for (int i = 0; i < nb_options; i++) {
        printf("%s\n", options[i]);
    }

    printf("--------------------------------------------\n");
    printf("Entrez votre choix : ");
}


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

/*
 Lit l'entree utilisateur et s'assure qu'elle est valide.
 Choix_maximum = Le nombre maximum d'options valides.
 Retourne : le choix valide de l'utilisateur (ou 0 si invalide).
 */
int obtenir_entier_utilisateur(int nombre_max){

    int choix;
    //int resultat_lecture;

    // lit l'entier
    if (scanf("%d",&choix) ==1){
        if (choix <= nombre_max) return choix;
    }
    else{

        printf("\nErreur de saisie\n");
        printf("Entrez votre choix :");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // vider le buffer

        return obtenir_entier_utilisateur(nombre_max);
    }

}


int verif_mdp(){

    const int mdp = 123;
    int valeur;
    printf("Entrer le mot de passe : \n");
    scanf("%d", &valeur);

    if (valeur == mdp) return 1;
    else return 0;

}