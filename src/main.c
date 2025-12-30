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

  printf("\nTests images\n");

  IMAGE img = lire_image("IMG_5389.txt");

  printf("Hauteur : %d\nLargeur : %d\nNombre de canaux : %d\n", img.hauteur, img.largeur, img.nb_canaux);

  printf("\nPixel (y,x) (35,10)\nR : %d\nV : %d\nB : %d\n", img.matrice[35][10].r, img.matrice[35][10].v, img.matrice[35][10].b);
  printf("\nPixel (y,x) (H,L)\nR : %d\nV : %d\nB : %d\n", img.matrice[H-1][L-1].r, img.matrice[H-1][L-1].v, img.matrice[H-1][L-1].b);

  IMAGE img1 = lire_image("IMG_5390.txt");
  IMAGE img1_bleu = filtrer_bleu(&img1, SEUIL_BLEU, DELTA);
  system("python3 src/simu/afficher_image.py resultats/filtre_bleu");

  printf("Programme arrêté \n");
  ajout_log("Programme arrêté \n");


  return 0;
}
