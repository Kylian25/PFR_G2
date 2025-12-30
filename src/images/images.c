#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images.h"
#include "../outils/outils.h"

IMAGE lire_image(const char * nom_fichier){

    FILE *fichier;
    char chemin[50];
    IMAGE image;


    sprintf(chemin,"donnees/%s", nom_fichier);  // créer le chemin vers l'image

    fichier= fopen(chemin,"r");

    if (fichier==NULL){
        printf("Erreur de lecture du fichier\n");
        ajout_log("Erreur de lecture du fichier");
        return image;
    }

    if (fscanf(fichier, "%d %d %d", &image.hauteur, &image.largeur, &image.nb_canaux) !=3){  // récup hauteur,largeur et le nombre de canaux de l'image
        fclose(fichier);
        ajout_log("Erreur dans la récupération des parametres de l'image");
        return image;
    }  
    if (H != image.hauteur || L != image.largeur){

        ajout_log("Erreur de lecture des parametres");
        fclose(fichier);
        return image;
    }

    // Récupération des pixels

    int T = image.hauteur * image.largeur;  // taille de l'image 


    // rouge 
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%d", &(image.matrice[y][x].r));
        }
    }
    // vert
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%d", &(image.matrice[y][x].v));
        }
    }
    // bleu
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%d", &(image.matrice[y][x].b));
        }
    }

   
    return image;
}

IMAGE filtrer_bleu(IMAGE * image_source, int seuil, int delta){

    IMAGE img = *image_source;
    for (int y=0;y<H;y++){
        for (int x=0;x<L;x++){

            PIXEL * px = &img.matrice[y][x];   // pointeur pour modifier directement la valeur du pixel (évite les copies)

            if(px->b > seuil && px->b > px->r + delta && px->b > px->v + delta){

                px->b = 255;
                px->r = 0;
                px->v = 0;
            }
            else{

                px->b = 0;
                px->r = 0;
                px->v = 0;
            }

        }
    }
    sauvegarder_image("filtre_bleu", img);
    return img;

    

}

void sauvegarder_image(const char * nom_fichier, IMAGE img){

    char chemin[100];
    sprintf(chemin, "resultats/%s",nom_fichier);

    FILE *f = fopen(chemin,"w");
    for(int y=0; y<H; y++)
        for(int x=0; x<L; x++)
            fprintf(f,"%d %d %d ", img.matrice[y][x].r, img.matrice[y][x].v, img.matrice[y][x].b);

    fclose(f);
}



