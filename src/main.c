#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "menu/menu.h"
#include "outils/outils.h"
#include "config/config.h"
#include "images/module_image.h"



int main(void){

  // INITIALISATION 
  
  recup_config(&config);

  init_log(&f_log);

  // FIN INITIALISATION 

  message_console("Debut du programme..\n", "The program is starting\n");
  ajout_log("Lancement du programme \n");

  demarrer_menu();

  message_console("Programme arrêté \n", "Program finished\n");
  ajout_log("Programme arrêté \n");

  return 0;
}
