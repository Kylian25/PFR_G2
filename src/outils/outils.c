#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outils.h"
#include "../config/config.h"

void ajout_log(const char *message) {

    char commande[256];
    char commande2[256];
    char commande3[256];

    sprintf(commande2, "chmod ug+w %s", f_log.chemin_log);
    system(commande2); // droits de modifs du fichier

    sprintf(commande, "echo \"$(date '+%%H:%%M:%%S') : %s\" >> %s", message,f_log.chemin_log);  // snprintf ppur eviter d'avoir des crashs a cause d'un message trop long si besoin
    system(commande);

    sprintf(commande3, "chmod ug-w %s", f_log.chemin_log);
    system(commande3);  // droits
}

void reset_logs(){

    system(" > configuration/logs");

}

void affiche_logs(){

    char commande[200];

    sprintf(commande, "cat %s", f_log.chemin_log);

    system(commande);

}
/*
char * lire_chaine(){
    
    char *nom_fichier = (char*)malloc(256 * sizeof(char));
    
    if (nom_fichier == NULL) {
        printf("Erreur, allocation mémoire échouée\n");
        return NULL;
    }
    
    printf("Entrez le nom du fichier : ");

    if (scanf("%255s", nom_fichier) != 1) {
        printf("Erreur, impossible de lire la chaine\n");
        free(nom_fichier);
        return lire_chaine();
    }
    
    if (strlen(nom_fichier) == 0) {
        printf("Erreur : chaîne vide\n");
        free(nom_fichier);
        return lire_chaine();
    }
    
    for (int i = 0; nom_fichier[i] != '\0'; i++) {
        if (nom_fichier[i] < 32 || nom_fichier[i] > 126) {
            printf("Erreur : caractère invalide\n");
            free(nom_fichier);
            return lire_chaine();
        }
    }
    
    return nom_fichier;
}
*/