#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images.h"
#include "../outils/outils.h"

COULEUR BLEU = {0,0,255};
COULEUR JAUNE = {255,230,0};
COULEUR ROUGE = {255,0,0};

IMAGE *lire_image(const char * nom_fichier){

    FILE *fichier;
    char chemin[50];
    IMAGE *image = malloc(sizeof(IMAGE)); // reserver l'espace
    if(!image) return NULL;


    sprintf(chemin,"donnees/%s", nom_fichier);  // créer le chemin vers l'image

    fichier= fopen(chemin,"r");

    if (fichier==NULL){
        printf("Erreur de lecture du fichier\n");
        ajout_log("Erreur de lecture du fichier");
        free(image);
        return NULL;
    
    }

    if (fscanf(fichier, "%d %d %d", &image->hauteur, &image->largeur, &image->nb_canaux) !=3){  // récup hauteur,largeur et le nombre de canaux de l'image
        fclose(fichier);
        ajout_log("Erreur dans la récupération des parametres de l'image");
        free(image);
        return NULL;
    }  
    if (H != image->hauteur || L != image->largeur){

        ajout_log("Erreur de lecture des parametres");
        fclose(fichier);
        free(image);
        return NULL;
    }

    // Récupération des pixels

    int T = image->hauteur * image->largeur;  // taille de l'image 


    // rouge 
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%hhu", &(image->matrice[y][x].r));
        }
    }
    // vert
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%hhu", &(image->matrice[y][x].v));
        }
    }
    // bleu
    for (int y=0; y<H ; y++ ){
        for (int x=0 ; x<L; x++ ){

            fscanf(fichier, "%hhu", &(image->matrice[y][x].b));
        }
    }

    fclose(fichier);
    return image;
}

IMAGE * filtrer_bleu(IMAGE * image_source, unsigned char seuil, unsigned char delta){

    IMAGE * img = image_source;
    img = malloc(sizeof(IMAGE));
    for (int y=0;y<H;y++){
        for (int x=0;x<L;x++){

            PIXEL * px_src = &image_source->matrice[y][x];  // pixel de l'image source
            PIXEL * px = &img->matrice[y][x];   // pointeur pour modifier directement la valeur du pixel (évite les copies)

            if(px_src->b > seuil && px_src->b > px_src->r + delta && px_src->b > px_src->v + delta){

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

IMAGE * filtrer_jaune(IMAGE * image_source){

    IMAGE * img = image_source;
    img = malloc(sizeof(IMAGE));
    for (int y=0;y<H;y++){
        for (int x=0;x<L;x++){

            PIXEL * px_src = &image_source->matrice[y][x];  // pixel de l'image source
            PIXEL * px = &img->matrice[y][x];   // pointeur pour modifier directement la valeur du pixel (évite les copies)

            if( px_src->b < MAX_JAUNE_bleu && px_src->r > px_src->b + DELTA && px_src->v > px_src->b + DELTA && abs(px_src->r - px_src->v) < 70){

                px->b = 0;
                px->r = 255;
                px->v = 230;
            }
            else{

                px->b = 0;
                px->r = 0;
                px->v = 0;
            }

        }
    }
    sauvegarder_image("filtre_jaune", img);
    return img;  

}

IMAGE * filtrer_rouge(IMAGE * image_source, unsigned char seuil, unsigned char delta){

    IMAGE *img = image_source;
    img = malloc(sizeof(IMAGE));
    for (int y=0;y<H;y++){
        for (int x=0;x<L;x++){

            PIXEL * px_src = &image_source->matrice[y][x];  // pixel de l'image source
            PIXEL * px = &img->matrice[y][x];   // pointeur pour modifier directement la valeur du pixel (évite les copies)

            if(px_src->r > seuil && px_src->r > px_src->b + delta && px_src->r > 1.5*(px_src->v)){

                px->b = 0;
                px->r = 255;
                px->v = 0;
            }
            else{

                px->b = 0;
                px->r = 0;
                px->v = 0;
            }

        }
    }
    sauvegarder_image("filtre_rouge", img);
    return img;  

}

IMAGE * filtrer_image(const char * nom_fichier){

    IMAGE * image =  lire_image(nom_fichier);  // a liberer a la fin 

    int T = H*L;

    IMAGE * img_bleu = filtrer_bleu(image,SEUIL_BLEU, DELTA);  // a liberer a la fin
    IMAGE *img_jaune = filtrer_jaune(image);
    IMAGE *img_rouge = filtrer_rouge(image, SEUIL_ROUGE, DELTA);

    IMAGE * img_filtre = malloc(sizeof(IMAGE));

    for (int y=0; y<H; y++){
        for (int x=0 ; x<L; x++){

            PIXEL *  px_bleu = &img_bleu->matrice[y][x];
            PIXEL * px_jaune = &img_jaune->matrice[y][x];
            PIXEL * px_rouge = &img_rouge->matrice[y][x];

            if(px_bleu->b == BLEU.b){
                img_filtre->matrice[y][x].b = BLEU.b;
                img_filtre->matrice[y][x].r = BLEU.r;
                img_filtre->matrice[y][x].v = BLEU.v;
            } 
            else if (px_jaune->r==JAUNE.r && px_jaune->v==JAUNE.v){
                img_filtre->matrice[y][x].b = JAUNE.b;
                img_filtre->matrice[y][x].r = JAUNE.r;
                img_filtre->matrice[y][x].v = JAUNE.v;
            }
            else if (px_rouge->r == ROUGE.r){
                img_filtre->matrice[y][x].b = ROUGE.b;
                img_filtre->matrice[y][x].r = ROUGE.r;
                img_filtre->matrice[y][x].v = ROUGE.v;
            }
            else {
                img_filtre->matrice[y][x].b = 0;
                img_filtre->matrice[y][x].r = 0;
                img_filtre->matrice[y][x].v = 0;
            }
        }
    }

    // liberer l'espace 

    free(image);
    free(img_bleu);
    free(img_jaune);
    free(img_rouge);


    sauvegarder_image("image_filtree", img_filtre);
    return img_filtre;

}

void sauvegarder_image(const char * nom_fichier, IMAGE * img){

    char chemin[100];
    sprintf(chemin, "resultats/%s",nom_fichier);

    FILE *f = fopen(chemin,"w");
    for(int y=0; y<H; y++)
        for(int x=0; x<L; x++)
            fprintf(f,"%hhu %hhu %hhu ", img->matrice[y][x].r, img->matrice[y][x].v, img->matrice[y][x].b);

    fclose(f);
}
/*
void traiter_image(){

    char chemin_image[150];
    char chemin_image_originale[150];

    char * nom_fichier = lire_chaine();

    printf("%s", nom_fichier);

    
}
*/



