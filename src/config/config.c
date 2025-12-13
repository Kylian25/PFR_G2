#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

// atoi permet de transformer une chaine de caractère en entier 
// ex : atoi("90") -> 90


CONFIG_ROBOT config = {0};

void init_config(){

    strcpy(config.langue,DEF_LANGUE);
    strcpy(config.type_requete,DEF_TYPE_REQUETE);
    config.dist_dep=DEF_DIST_DEP;
    config.tps_dep=DEF_TPS_DEP;
    config.dist_det=DEF_DIST_DET;
    config.angle=DEF_ANGLE;

}

void charger_config(){

    
}

void afficher_fichier_config(){

    system("cat configuration/parametres");
    printf("\n");

}

void sauvergarder_config(){

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


}




