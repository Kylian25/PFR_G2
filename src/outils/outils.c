#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outils.h"

void ajout_log(const char *message) {

    char commande[256];


    // double %% pour que sprintf n'essaie pas de les interpréter (melange unix et c)
    sprintf(commande, "echo \"$(date '+%%H:%%M:%%S') : %s\" >> %s", message, "configuration/logs");  // snprintf ppur eviter d'avoir des crashs a cause d'un message trop long si besoin

    system(commande);
}

void reset_logs(){

    system(" > configuration/logs");

}

void affiche_logs(){

    system("cat configuration/logs");

}