/*
Module : config
Rôle : Ce fichier permet la gestion des paramètres du robot (langue, distances, angle...) via un fichier texte.
Il assure également la création dynamique d'un fichier de log à chaque lancement, 
nommé selon la date et l'heure via un appel shell Unix.

Auteur : Kylian
Date : 29/01/2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "../menu/menu.h"
#include "../outils/outils.h"


CONFIG_ROBOT config = {0};
LOG f_log = {0};

void init_config(CONFIG_ROBOT * config){

    strcpy(config->langue,DEF_LANGUE);
    strcpy(config->type_requete,DEF_TYPE_REQUETE);
    config->dist_dep=DEF_DIST_DEP;
    config->tps_dep=DEF_TPS_DEP;
    config->dist_det=DEF_DIST_DET;
    config->angle=DEF_ANGLE;

}

void init_log(LOG *l){

    char date[50];
    FILE *f;

    f = popen("date +%d_%m__%H_%M_%S","r");

    if (f!=NULL){  // si fichier existe

        if (fgets(date, sizeof(date), f) != NULL) {
            date[strcspn(date, "\n")] = '\0'; // supprime le retour à la ligne \n ajouté par la commande
        }
        pclose(f);
    }
    

    strcpy(l->date_cour,date); // enregistrement de la date
    sprintf(l->chemin_log, "logs/log_%s.txt",date);  // création du chemin vers le fichier de log

    // créer le fichier de log 
    FILE *fl = fopen(l->chemin_log, "w");
        if (fl != NULL) {
            fclose(fl); // Le fichier est maintenant créé et vide
        }
}

void recup_config(CONFIG_ROBOT * config){

    
    char cle[30];
    char valeur[30];

    system("chmod ug+rw configuration/parametres");
    FILE *f;
    f = fopen("configuration/parametres", "r");

    if (f == NULL) {
        message_console("Erreur : impossible d'ouvrir le fichier de configuration\n", "Error: unable to open configuration file\n");
        message_console("Vérifiez les droits d'accès du fichier configuration/parametres\n", "Check the access rights for the file : configuration/parametres.\n");
        ajout_log("ERREUR : Impossible d'ouvrir configuration/parametres");
        
        // Initialiser avec les valeurs par défaut
        init_config(config);
        return;
    }

    while (fscanf(f, "%29s %29s" , cle , valeur) == 2){  // == 2 -> si les deux chaines ont bien été lues
        
        // langue
        if (strcmp(cle,"langue") == 0){ // si les deux chaines sont identiques renvoie 0

            if (strcmp(valeur,"FR") == 0) {
                strcpy(config->langue, "FR");
            }
            else if (strcmp(valeur, "EN") ==0){
                strcpy(config->langue, "EN");
            }
            else {
                strcpy(config->langue, DEF_LANGUE);
            }
        }
        // type requete
        else if (strcmp(cle,"type_requete") == 0){ 

            if (strcmp(valeur,"voc") == 0) {
                strcpy(config->type_requete, "voc");
            }
            else if (strcmp(valeur, "txt") ==0){
                strcpy(config->type_requete, "txt");
            }
            else {
                strcpy(config->type_requete, DEF_TYPE_REQUETE);
            }
        }
        // distance deplacement
        else if (strcmp(cle, "dist_dep") == 0){

            char * fin;
            long val;
            val = strtol(valeur,&fin, 10 );
            if (*fin == '\0' && val > MIN_DIST_DEP && val < MAX_DIST_DEP){
                config->dist_dep=(int)val;
            }
            else{
                config->dist_dep=DEF_DIST_DEP;
            }
        }
        // temps déplacement
        else if (strcmp(cle, "tps_dep") ==0){

            char * fin;
            long val;
            val = strtol(valeur,&fin, 10 );
            if (*fin == '\0' && val > MIN_TPS_DEP && val < MAX_TPS_DEP){
                config->tps_dep=(int)val;
            }
            else{
                config->tps_dep=DEF_TPS_DEP;
            }
        }
        // distance detection
         else if (strcmp(cle, "dist_det") ==0){

            char * fin;
            long val;
            val = strtol(valeur,&fin, 10 );
            if (*fin == '\0' && val > MIN_DIST_DET && val < MAX_DIST_DET){
                config->dist_det=(int)val;
            }
            else{
                config->dist_det=DEF_DIST_DET;
            }
        }
        // angle 
         else if (strcmp(cle, "angle") ==0){

            char * fin;
            long val;
            val = strtol(valeur,&fin, 10 );
            if (*fin == '\0' && val > MIN_ANGLE && val < MAX_ANGLE){
                config->angle=(int)val;
            }
            else{
                config->angle=DEF_ANGLE;
            }
        }
        else{
            ajout_log("erreur dans le chargement des parametres");
        }
    }
    sauvegarder_config();
}


void afficher_fichier_config(){

    printf("\n");
    system("cat configuration/parametres");
    printf("--------------------------------------------");
    printf("\n");
    

}

void sauvegarder_config(){


    const char * chemin_fichier_config = "configuration/parametres";

    FILE * fichier;
    fichier = fopen(chemin_fichier_config, "w");   // efface le contenu du fichier 

    // ecriture 
    fprintf(fichier, "langue %s\n", config.langue);
    fprintf(fichier, "type_requete %s\n", config.type_requete);
    fprintf(fichier, "dist_dep %d\n", config.dist_dep);
    fprintf(fichier, "tps_dep %d\n", config.tps_dep);
    fprintf(fichier, "dist_det %d\n",config.dist_det );
    fprintf(fichier, "angle %d\n",config.angle);

    fclose(fichier);


}

void modifier_config(){

    afficher_fichier_config(); //affichage des paramètres
    
    message_console("\n\n1 : Modifier la distance de déplacement\n","\n\n1 : Change the travel distance\n");
    message_console("2 : Modifier le temps de déplacement \n", "2 : Change travel time \n");
    message_console("3 : Modifier la distance de détection\n", "3 : Change the detection distance\n");
    message_console("4 : Modifier l'angle de rotation\n", "4 : Change rotation angle\n");
    message_console("5 : Revenir au menu précédent\n", "5 : Return to previous menu\n");

    const int nb_choix = 5;
    int choix;
    int valeur;
    
    message_console("\nEntrez votre choix : ", "\nEnter your choice : ");
    choix = obtenir_entier_utilisateur(nb_choix);
    
        switch (choix){

            case 1:
                message_console("Entrez une valeur entre ", "Enter a value between ");
                printf("[%d,%d] : ", MIN_DIST_DEP, MAX_DIST_DEP);
                valeur = obtenir_entier_utilisateur(MAX_DIST_DEP);
                if (valeur > MIN_DIST_DEP && valeur <= MAX_DIST_DEP){
                    config.dist_dep = valeur;
                    sauvegarder_config();
                }
                else message_console("valeur incorrecte", "Incorrect value");
                ajout_log("distance de déplacement par defaut modifiée");

                system("clear");
                modifier_config();   // relancer la fonction pour éviter de revenir au menu précédent si les modifs ne sont pas encore finies
                break;
            case 2:
                message_console("Entrez une valeur entre ", "Enter a value between ");
                printf("[%d,%d] : ", MIN_TPS_DEP,MAX_TPS_DEP);
                valeur = obtenir_entier_utilisateur(MAX_TPS_DEP);
                if (valeur > MIN_TPS_DEP && valeur < MAX_TPS_DEP){
                    config.tps_dep = valeur;
                    sauvegarder_config();
                }
                else message_console("valeur incorrecte", "Incorrect value");
                ajout_log("temps de déplacement par defaut modifiée");
                system("clear");
                modifier_config();
                break;
            case 3: 
                message_console("Entrez une valeur entre ", "Enter a value between ");
                printf("[%d,%d] : ", MIN_DIST_DET, MAX_DIST_DET);
                valeur = obtenir_entier_utilisateur(MAX_DIST_DET);
                if (valeur > MIN_DIST_DET && valeur < MAX_DIST_DET){
                    config.dist_det = valeur;
                    sauvegarder_config();
                }
                else message_console("valeur incorrecte", "Incorrect value");
                ajout_log("distance de détection par defaut modifiée");
                system("clear");
                modifier_config(); 
                break;
            case 4:
                message_console("Entrez une valeur entre ", "Enter a value between ");
                printf("[%d,%d] : ", MIN_ANGLE, MAX_ANGLE);
                valeur = obtenir_entier_utilisateur(MAX_ANGLE);
                if (valeur > MIN_ANGLE && valeur < MAX_ANGLE){
                    config.angle = valeur;
                    sauvegarder_config();
                }
                else message_console("valeur incorrecte", "Incorrect value");
                ajout_log("angle de rotation par defaut modifiée");
                system("clear");
                modifier_config();
                break;
            case 5:
                system("clear");

                message_console("\nRetour au menu précédent\n","\nReturn to previous menu\n");
                ajout_log("Retour au menu précédent");
                return; 
            default:
                system("clear");

                message_console("\nChoix invalide\n","\nInvalid choice\n");
                ajout_log("[ADMIN] Choix invalide");
                break;
        }

}

void choix_langue(){

    int choix;
    int nb_choix = 3;

    message_console("\n1 : Francais\n", "\n1 : French\n");
    message_console("2 : Anglais\n", "2 : English\n");
    message_console("Votre choix : ", "Your choice : ");

    choix=obtenir_entier_utilisateur(nb_choix);

    system("chmod ug+w configuration/parametres");

    if (choix==1){
        message_console("\nLangue : Francais\n", "\nLanguage : French\n");
        strcpy(config.langue,"FR");
        sauvegarder_config();
        ajout_log("Langue choisie : Francais");
    }
    else if (choix==2){
        message_console("\nLangue choisie : Anglais", "\nChosen language : English");
        strcpy(config.langue,"EN");
        sauvegarder_config();
        ajout_log("Langue choisie : Anglais");
    }
    else message_console("Choix invalide, retour au menu précédent", "Invalid choice, returning to previous menu");
    sauvegarder_config();
    system("chmod ug-w configuration/parametres");

}




