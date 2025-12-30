#include <stdio.h>
#include <string.h>

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

  printf("\nTests images\n");

  IMAGE * img = lire_image("IMG_5389.txt");

  printf("Hauteur : %d\nLargeur : %d\nNombre de canaux : %d\n", img->H, img->L, img->nb_canaux);

  printf("Premier pixel  : R=%d G=%d B=%d\n",
  img->donnees[0].r,
  img->donnees[0].v,
  img->donnees[0].b);

  int last = img->H * img->L - 1;
  printf("Dernier pixel  : R=%d G=%d B=%d\n",
  img->donnees[last].r,
  img->donnees[last].v,
  img->donnees[last].b);

  liberer_image(img);

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");


  return 0;
}
