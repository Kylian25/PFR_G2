#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images.h"
#include "../menu/menu.h"
#include "../outils/outils.h"

IMAGE * lire_image(const char * nom_fichier){

    FILE *f;
    char chemin[50];

    IMAGE * image = malloc(sizeof(IMAGE));
     if (image == NULL) {
        fclose(f);
        return NULL;
    }


    sprintf(chemin,"donnees/%s", nom_fichier);  // créer le chemin vers l'image

    f= fopen(chemin,"r");

    if (f==NULL){
        printf("Erreur de lecture du fichier");
        ajout_log("Erreur de lecture du fichier");
        return NULL;
    }

    if (fscanf(f, "%d %d %d", &(image->H), &(image->L), &(image->nb_canaux)) !=3){  // récup hauteur,largeur et le nombre de canaux de l'image
        fclose(f);
        free(image);
        ajout_log("Erreur dans la récupération des parametres de l'image");
        return NULL;
    }  

    image->donnees = malloc(image->H * image->L * sizeof(PIXEL)); 
    if (image->donnees == NULL){
        fclose(f);
        free(image);
        ajout_log("Erreur dans l'allocation des données de l'image");
        return NULL;
    }

    // Récupération des pixels

    int T = image->H*image->L;  // taille de l'image 

    for (int i=0; i<T ; i++ ){
        
        fscanf(f, "%d", &(image->donnees[i].r));
    }
    for (int i=0; i<T;i++){
        fscanf(f, "%d", &(image->donnees[i].v));
    }
    for (int i =0; i<T;i++){
        fscanf(f, "%d", &(image->donnees[i].b));
    }

    fclose(f);
    return image;
}


void liberer_image(IMAGE *img)
{
    if (img) {
        free(img->donnees);
        free(img);
        ajout_log("image libérée");
    }
}


