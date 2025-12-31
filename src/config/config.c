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

    if (f!=NULL){

        if (fgets(date, sizeof(date), f) != NULL) {
            date[strcspn(date, "\n")] = '\0'; // supprime le retour à la ligne \n ajoute par la commande
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
    FILE *f;
    f = fopen("configuration/parametres", "r");

    if (f == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de configuration\n");
        printf("Vérifiez les droits d'accès du fichier configuration/parametres\n");
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

    printf("\n\n1 : Modifier la distance de déplacement\n");
    printf("2 : Modifier le temps de déplacement \n");
    printf("3 : Modifier la distance de détection\n");
    printf("4 : Modifier l'angle de rotation\n");
    printf("5 : Revenir au menu précédent\n");

    const int nb_choix = 5;
    int choix;
    int valeur;
    
    printf("\nEntrez votre choix : ");
    choix = obtenir_entier_utilisateur(nb_choix);
    
        switch (choix){

            case 1:
                printf("Entrez la valeur :");
                valeur = obtenir_entier_utilisateur(MAX_DIST_DEP);
                if (valeur > MIN_DIST_DEP && valeur <= MAX_DIST_DEP){
                    config.dist_dep = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("distance de déplacement par defaut modifiée");

                system("clear");
                break;
            case 2:
                printf("Entrez la valeur :");
                valeur = obtenir_entier_utilisateur(MAX_TPS_DEP);
                if (valeur > MIN_TPS_DEP && valeur < MAX_TPS_DEP){
                    config.tps_dep = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("temps de déplacement par defaut modifiée");
                system("clear");
                break;
            case 3: 
                printf("Entrez la valeur :");
                valeur = obtenir_entier_utilisateur(MAX_DIST_DET);
                if (valeur > MIN_DIST_DET && valeur < MAX_DIST_DET){
                    config.dist_det = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("distance de détection par defaut modifiée");
                system("clear");
                break;
            case 4:
                printf("Entrez la valeur :");
                valeur = obtenir_entier_utilisateur(MAX_ANGLE);
                if (valeur > MIN_ANGLE && valeur < MAX_ANGLE){
                    config.angle = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("angle de rotation par defaut modifiée");
                system("clear");
                break;
            case 5:
                system("clear");

                printf("\nRetour au menu précédent\n");
                ajout_log("Retour au menu précédent");
                return; 
            default:
                system("clear");

                printf("\nChoix invalide\n");
                ajout_log("[ADMIN] Choix invalide");
                break;
        }

}

void choix_langue(){

    int choix;
    int nb_choix = 3;

    printf("\n1 : Francais\n");
    printf("2 : Anglais\n");
    printf("Votre choix : ");

    choix=obtenir_entier_utilisateur(nb_choix);

    system("chmod ug+w configuration/parametres");

    if (choix==1){
        printf("\nLangue : Francais\n");
        strcpy(config.langue,"FR");
        sauvegarder_config();
        ajout_log("Langue choisie : Francais");
    }
    else if (choix==2){
        printf("\nLangue choisie : Anglais");
        strcpy(config.langue,"EN");
        sauvegarder_config();
        ajout_log("Langue choisie : Anglais");
    }
    else printf("Choix invalide, retour au menu précédent");
    system("chmod ug-w configuration/parametres");

}




