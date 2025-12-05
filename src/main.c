#include <stdio.h>
#include "menu/menu.h"
#include "outils/outils.h"



int main(void){

  printf("Debut du programme..\n");
  ajout_log("Lancement du programme \n");

  demarrer_menu();

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");

  return 0;
}
