#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "outils.h"

void ajout_log(const char *message) {

    char commande[256];


    // Attention : double %% pour que sprintf n'essaie pas de les interpréter
    sprintf(commande, "echo \"$(date '+%%H:%%M:%%S') : %s\" >> %s", message, "configuration/logs");  // snprintf ppur eviter d'avoir des crashs a cause d'un message trop long si besoin

    system(commande);
}