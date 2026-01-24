#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "menu.h"
#include "../outils/outils.h"
#include "../config/config.h"
#include "../traitement/traitement.h"


// debut du menu
void demarrer_menu() {

    system("chmod ug-w configuration/parametres");
    system("clear"); // reset le terminal
    int choix;


    const char * optionsFR[] = {   // tableau de pointeurs qui pointent vers une chaine de caracteres (constantes)
        "1 : Mode Utilisateur",
        "2 : Mode Administrateur",
        "3 : Quitter"
    };

    const char * optionsEN[] = {  
        "1 : User mode",
        "2 : Admin mode",
        "3 : Exit program"
    };

    const char ** options;  // pointeur vers un tableau de char
    if (!strcmp(config.langue, "FR")) options = optionsFR;
    else options = optionsEN;

    const char * nom_menu;
    if (!strcmp(config.langue, "FR")) nom_menu = "     MENU PRINCIPAL";  // la chaine "aaaaa" est une adresse donc le = fonctionne
    else nom_menu = "  MAIN MENU";

    int nb_options = 3;
    int mdp_valide;

    system("clear");
    do {

        afficher_menu(nom_menu, options, nb_options);

        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear");
                message_console("\n=====> Entree en mode Utilisateur...\n", "\n=====> Entering User Mode...\n");
                ajout_log("==> Entree en mode Utilisateur");
        
                menu_mode_utilisateur();
                break;
            case 2:
                mdp_valide = verif_mdp();
                if (mdp_valide){

                    system("clear");
                    message_console("\n=====> Entree en mode Administrateur...\n","\n=====> Entering Admin mode...\n");
                    ajout_log("Mot de passe Valide ");
                    ajout_log("==> Entree en Mode Administrateur");
                    system("chmod ug+rw configuration/parametres"); // modifie les droits du fichier parametres pour modification
                    menu_mode_administrateur();
                    break;
                }
                else {
                    message_console("Mauvais mot de passe\n","Wrong password\n");
                    ajout_log("Mauvais mot de passe rentré");
                    break;
                }
            case 3:
                system("clear");
                system("chmod ug+w configuration/parametres");
                message_console("\nArret du programme..\n", "\nStopping program..\n");
                ajout_log("Quitter le programme");
                return; 
            default:
                system("clear"); // reset le terminal 

                message_console("\nChoix invalide, Réessayez.\n","\nInvalid choice, try again\n");
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

    const char *optionsFR[] = {
        "1 : Mode Requete Textuelle",
        "2 : Mode Requete Vocale",
        "3 : Choisir la Langue (defaut: Francais)",
        "4 : Retour au Menu Principal"
    };

    const char *optionsEN[] = {
        "1 : Text request mode",
        "2 : Voice request mode",
        "3 : Choose language",
        "4 : Return to main menu"
    };

    const char ** options;  // pointeur vers un tableau de chaines
    if (!strcmp(config.langue, "FR")) options = optionsFR;
    else options = optionsEN;

    int nb_options = 4;

    const char * nom_menu;
    if (!strcmp(config.langue, "FR")) nom_menu = "  Mode Utilisateur";  
    else nom_menu = "  User mode";
    
    do {
        afficher_menu(nom_menu, options, nb_options);
        
        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear"); // reset le terminal

                message_console("\n[UTILISATEUR] Mode Textuel\n","\n[USER] Text mode\n");
                ajout_log("[UTILISATEUR] Choix du mode Textuel");
                strcpy(config.type_requete, "txt");
                //sauvegarder_config();
                // appel fonction de gestion des commandes texte
                gestion_requetes("txt");
                break;
            case 2:
                system("clear"); // reset le terminal 

                message_console("\n[UTILISATEUR] Mode Vocal\n","\n[USER] Voice mode\n");
                ajout_log("[UTILISATEUR] Choix du mode Vocal");
                strcpy(config.type_requete, "voc");
                //sauvegarder_config();
                // appel fonction de gestion des commandes vocales
                gestion_requetes("voc");
                break;
            case 3:
                system("clear"); // reset le terminal 

                message_console("\n[UTILISATEUR] Changement de langue\n","[USER] Changing language");
                ajout_log("[UTILISATEUR] Choix de la langue");
                choix_langue();
                break;
            case 4:
                system("clear"); // reset le terminal 

                message_console("\nRetour au Menu Principal\n","\nReturn to main menu\n");
                ajout_log("[UTILISATEUR] Retour au menu principal");
                system("clear");
                return; 
            default:
                system("clear"); // reset le terminal 

                message_console("\nChoix invalide.\n", "\nInvalid choice\n");
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

    const char * optionsFR[] ={         // tableau de chaines 
        "1 : Afficher parametres",
        "2 : Modifier parametres",
        "3 : Revenir au menu amdinistrateur"
    };

    const char * optionsEN[] ={
        "1 : Display settings",
        "2 : Modify settings",
        "3 : Return to admin menu"
    };

    const char ** options;  // pointeur vers un tableau de chaines
    if (!strcmp(config.langue, "FR")) options = optionsFR;
    else options = optionsEN;

    const char * nom_menu;
    if (!strcmp(config.langue, "FR")) nom_menu = "  Admin - modification des paramètres";
    else nom_menu = "  Admin - modify settings";

    do{

        afficher_menu(nom_menu, options, nb_choix);

        choix = obtenir_entier_utilisateur(nb_choix);

        switch (choix){

            case 1:
                system("clear");

                message_console("\n[ADMIN] Affichage des parametres\n","\n[ADMIN] Settings display\n");
                ajout_log("[ADMIN] Affichage des parametres");
                
                afficher_fichier_config();
                break;

            case 2:
                system("clear");
                message_console("\n[ADMIN] Modification des parametres\n", "\n[ADMIN] Settings modification\n");
                ajout_log("[ADMIN] Modification des parametres");
                modifier_config();
                break;
            case 3:
                system("clear"); // reset le terminal 

                message_console("\nRetour au Menu administrateur\n", "\nReturn to admin menu\n");
                ajout_log("Retour au menu administrateur");
                return; 
            default:
                system("clear"); // reset le terminal 

                message_console("\nChoix invalide\n", "\nInvalid choice\n");
                ajout_log("[ADMIN] Choix invalide");
                break;
        }

    }while (choix!=3);

}



//  Menu du mode administrateur : modification de la configuration et des logs
void menu_mode_administrateur() {

    int choix;

    const char *optionsFR[] = {
        "1 : Modifier le Fichier de Configuration",
        "2 : Afficher les Logs Systemes",
        "3 : Retour au Menu Principal"
    };

    const char *optionsEN[] = {
        "1 : Modify config file",
        "2 : Display system logs",
        "3 : Return to main menu"
    };

    const char ** options;  // pointeur vers un tableau de chaines
    if (!strcmp(config.langue, "FR")) options = optionsFR;
    else options = optionsEN;

    int nb_options = 3;

    const char * nom_menu;
    if (!strcmp(config.langue, "FR")) nom_menu = "  Mode Administrateur";  // la chaine "aaaaa" est une adresse donc le = fonctionne
    else nom_menu = "  Admin mode";
    

    do {
        afficher_menu(nom_menu, options, nb_options);
        
        choix = obtenir_entier_utilisateur(nb_options);

        switch (choix) {
            case 1:
                system("clear");
                message_console("\n[ADMIN] Lancement de l'editeur de configuration\n", "\n[ADMIN] Launching the configuration editor");
                ajout_log("[ADMIN] Lancement de l'editeur de configuration");
                menu_modifier_config();
                
                break;
            case 2:
                system("clear");
                message_console("\n[ADMIN] ------------ Affichage des logs -------------\n\n", "\n[ADMIN] ------------ Log display -------------\n\n");
                ajout_log("[ADMIN] Affichage des logs");
                affiche_logs();
                break;
            case 3:
                system("clear");
                message_console("\nRetour au Menu Principal\n","\nReturn to main menu\n");
                ajout_log("Retour au menu principal");
                return; 
            default:
                system("clear"); // reset le terminal 
                message_console("\nChoix invalide\n","\nInvalid choice\n");
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
    message_console("Entrez votre choix : ", "Enter your choice : ");
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

        message_console("\nErreur de saisie\n", "\nInput error\n");
        message_console("Entrez votre choix :","Enter your choice : ");

        int c;
        while ((c = getchar()) != '\n' && c != EOF);  // vider le buffer

        return obtenir_entier_utilisateur(nombre_max);
    }

}


int verif_mdp(){

    const int mdp = 123;
    int valeur;
    message_console("Entrer le mot de passe :  ", "Enter password :  ");
    scanf("%d", &valeur);

    if (valeur == mdp) return 1;
    else return 0;

}