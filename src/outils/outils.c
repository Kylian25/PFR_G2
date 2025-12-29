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

    system("cat configuration/logs");

}