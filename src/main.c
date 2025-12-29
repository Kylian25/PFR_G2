#include <stdio.h>
#include <string.h>
#include "menu/menu.h"
#include "outils/outils.h"
#include "config/config.h"



int main(void){

  // INITIALISATION 

  init_config();

  init_log(&f_log);

  reset_logs(); // résinitialise le fichier de logs

  // FIN INITIALISATION 

  printf("Debut du programme..\n");
  ajout_log("Lancement du programme \n");

  demarrer_menu();

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");

  return 0;
}
