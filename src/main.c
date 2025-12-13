#include <stdio.h>
#include "menu/menu.h"
#include "outils/outils.h"
#include "config/config.h"



int main(void){

  // INITIALISATION 

  init_config();

  reset_logs(); // résinitialise le fichier de logs

  // FIN INITIALISATION 
  // tests 
  afficher_fichier_config();
  printf("\nmodifs\n");
  sauvergarder_config();
  afficher_fichier_config();
  // fin tests

  printf("Debut du programme..\n");
  ajout_log("Lancement du programme \n");

  demarrer_menu();

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");

  return 0;
}
