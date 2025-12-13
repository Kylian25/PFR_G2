#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "../menu/menu.h"
#include "../outils/outils.h"


CONFIG_ROBOT config = {0};

void init_config(){

    strcpy(config.langue,DEF_LANGUE);
    strcpy(config.type_requete,DEF_TYPE_REQUETE);
    config.dist_dep=DEF_DIST_DEP;
    config.tps_dep=DEF_TPS_DEP;
    config.dist_det=DEF_DIST_DET;
    config.angle=DEF_ANGLE;

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
    fprintf(fichier, "langue=%s\n", config.langue);
    fprintf(fichier, "type_requete=%s\n", config.type_requete);
    fprintf(fichier, "dist_dep=%d\n", config.dist_dep);
    fprintf(fichier, "tps_dep=%d\n", config.tps_dep);
    fprintf(fichier, "dist_det=%d\n",config.dist_det );
    fprintf(fichier, "angle=%d\n",config.angle);

    fclose(fichier);


}

void modifier_config(){

    printf("\n\n1 : Modifier la distance de déplacement\n");
    printf("2 : Modifier le temps de déplacement \n");
    printf("3 : Modifier la distance de détection\n");
    printf("4 : Modifier l'angle de rotation\n");
    printf("5 : Revenir au menu précédent\n");

    const int nb_choix = 5;
    int max = 360;
    int choix;
    int valeur;
    printf("\nEntrez votre choix : ");
    choix = obtenir_choix_utilisateur(nb_choix);
    
        switch (choix){

            case 1:
                printf("Entrez la valeur :\n");
                valeur = obtenir_choix_utilisateur(max);
                if (valeur > 0 && valeur < 2){
                    config.dist_dep = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("distance de déplacement par defaut modifiée");
                break;
            case 2:
                printf("Entrez la valeur :\n");
                valeur = obtenir_choix_utilisateur(360);
                if (valeur > 0 && valeur < 30){
                    config.tps_dep = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("temps de déplacement par defaut modifiée");
                break;
            case 3: 
                printf("Entrez la valeur :\n");
                valeur = obtenir_choix_utilisateur(360);
                if (valeur > 5 && valeur < 50){
                    config.dist_det = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("distance de détection par defaut modifiée");
                break;
            case 4:
                printf("Entrez la valeur :\n");
                valeur = obtenir_choix_utilisateur(360);
                if (valeur > 20 && valeur < 360){
                    config.angle = valeur;
                    sauvegarder_config();
                }
                else printf("valeur incorrecte");
                ajout_log("angle de rotation par defaut modifiée");
                break;
            case 5:
                printf("\nRetour au menu précédent\n");
                ajout_log("Retour au menu précédent");
                return; 
            default:
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

    choix=obtenir_choix_utilisateur(nb_choix);

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

}




