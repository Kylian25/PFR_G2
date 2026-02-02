/*
Module : outils
Rôle : Ce fichier contient les prototypes des fonctions utilisables dans tous les 
modules du projet, comme l'ajout d'information dans le fichier de log ou encore l'affichage
de messages dans la console en focntion de la langue présente dans la configuration au moment
de l'affichage du message.

Auteur : Kylian
Date : 29/01/2026
*/

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

void affiche_logs(){

    char commande[200];

    sprintf(commande, "cat %s", f_log.chemin_log);

    system(commande);

}

void message_console(char * message_FR, char * message_EN){

    if (!strcmp(config.langue, "EN")) printf("%s",message_EN);  // strcmp renvoie 0 si chaines identiques
    else printf("%s", message_FR);
}

void lancer_simu(){  

    system("python3 src/simu/simu.py temp/resultat.jpg");
}
