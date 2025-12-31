#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "images/images.h"
#include "menu/menu.h"
#include "outils/outils.h"
#include "config/config.h"



int main(void){

  // INITIALISATION 

  recup_config(&config);

  init_log(&f_log);

  // FIN INITIALISATION 

  printf("Debut du programme..\n");
  ajout_log("Lancement du programme \n");

  demarrer_menu();


  
  // tests images
  IMAGE * image_test;
  image_test = filtrer_image("IMG_5390.txt");
  system("python3 src/simu/afficher_image.py resultats/image_filtree donnees/IMG_5390.jpeg");
  free(image_test);
  

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");


  return 0;
}
