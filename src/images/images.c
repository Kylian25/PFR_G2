#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images.h"
#include "../outils/outils.h"

COULEUR BLEU = {0,0,255};
COULEUR JAUNE = {255,230,0};
COULEUR ROUGE = {255,0,0};

IMAGE * creer_image(int h, int l){

    IMAGE *img = malloc(sizeof(IMAGE));
    if (img == NULL){
        ajout_log("Erreur d'allocation (creer_image)");
        return NULL;
    }
    img->hauteur = h;
    img->largeur = l;
    img->nb_canaux = 3;

    // Allocation 

    img->matrice = malloc(h * sizeof(PIXEL *));  // tableau de pointeurs PIXEL* qui pointent vers un pixel (lignes)
    if (img->matrice == NULL){
        free(img);
        ajout_log("Erreur allocation mémoire dans creer_image");
        return NULL;
    }

    // Allocation ligne par ligne 

    for (int y=0;y<h;y++){
        img->matrice[y] = malloc(l * sizeof(PIXEL)); // on alloue le contenu des lignes 

        if (img->matrice[y] == NULL){   // liberer ce qui a deja été alloué
            ajout_log("Erreur allocation mémoire dans creer_image, ligne par ligne");
            for (int i=0 ; i < y;i++){
                free(img->matrice[i]);
            }
            free(img->matrice);
            free(img);
        }
    }
    ajout_log("Image crée dans créer_image");
    return img;

}

void liberer_image(IMAGE * img){

    if (img == NULL) return;

    if (img->matrice){
        for (int y=0; y < img->hauteur ; y++){
            free(img->matrice[y]);
        }
        free(img->matrice);
    }
    free(img);
    ajout_log("Image libérée");
}


IMAGE *lire_image(const char * nom_fichier){

    FILE *fichier;
    char chemin[50];
    int hauteur,largeur,nb_canaux;

    sprintf(chemin,"donnees/%s", nom_fichier);  // créer le chemin vers l'image

    fichier= fopen(chemin,"r");

    if (fichier==NULL){
        printf("Erreur de lecture du fichier\n");
        ajout_log("Erreur de lecture du fichier");
        return NULL;
    
    }

    if (fscanf(fichier, "%d %d %d", &hauteur, &largeur, &nb_canaux) !=3){  // récup hauteur,largeur et le nombre de canaux de l'image
        fclose(fichier);
        ajout_log("Erreur dans la récupération des parametres de l'image");
        return NULL;
    }  

    IMAGE * image = creer_image(hauteur, largeur);  // créer l'image
    if (image == NULL) {
        fclose(fichier);
        return NULL;
    }

    // Récupération des pixels
    int H = image->hauteur;
    int L = image->largeur;

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
    ajout_log("Fichier image lu, OK");
    return image;
}

IMAGE * filtrer_bleu(IMAGE * image_source, unsigned char seuil, unsigned char delta){

    int H = image_source->hauteur;
    int L = image_source->largeur;

    IMAGE * img = creer_image(H,L);
    if (img == NULL) { ajout_log("Erreur de création d'image dans filtrer_bleu");return NULL;}

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

    int H = image_source->hauteur;
    int L = image_source->largeur;

    IMAGE * img = creer_image(H,L);
    if (img == NULL) { ajout_log("Erreur de création d'image dans filtrer_jaune");return NULL;}

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

    int H = image_source->hauteur;
    int L = image_source->largeur;

    IMAGE * img = creer_image(H,L);
    if (img == NULL) { ajout_log("Erreur de création d'image dans filtrer_rouge");return NULL;}

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
    if (image == NULL){ajout_log("Erreur creation image dans filtrer_image"); return NULL;}

    int H = image->hauteur;
    int L = image->largeur;
    char trace[200];

    IMAGE * img_bleu = filtrer_bleu(image,SEUIL_BLEU, DELTA);  // a liberer a la fin
    IMAGE *img_jaune = filtrer_jaune(image);
    IMAGE *img_rouge = filtrer_rouge(image, SEUIL_ROUGE, DELTA);

    IMAGE * img_filtre = creer_image(H,L);

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

    liberer_image(image);
    liberer_image(img_bleu);
    liberer_image(img_jaune);
    liberer_image(img_rouge);

    //trace 
    sprintf(trace, "%s : filtrage effctué", nom_fichier);
    ajout_log(trace);


    sauvegarder_image("image_filtree", img_filtre);
    return img_filtre;

}

void sauvegarder_image(const char * nom_fichier, IMAGE * img){

    int H = img->hauteur;
    int L = img->largeur;
    char trace[200];

    char chemin[100];
    sprintf(chemin, "resultats/%s",nom_fichier);

    FILE *f = fopen(chemin,"w");

    for(int y=0; y<H; y++){
        for(int x=0; x<L; x++){
            fprintf(f,"%hhu %hhu %hhu ", img->matrice[y][x].r, img->matrice[y][x].v, img->matrice[y][x].b);
        }
    }
    fclose(f);
    sprintf(trace, "Image sauvegardée : %s", nom_fichier);
    ajout_log(trace);
    
}

void afficher_image(const char * nom_image_filtree, const char * nom_image_originale, IMAGE * img){

    char commande[256];
    int H = img->hauteur;
    int L = img->largeur;
    sprintf(commande, "python3 src/simu/afficher_image.py resultats/%s donnees/%s %d %d", nom_image_filtree, nom_image_originale, H,L);

    system(commande);
}

/*
void traiter_image(){

    char chemin_image[150];
    char chemin_image_originale[150];

    char * nom_fichier = lire_chaine();

    printf("%s", nom_fichier);

    
}
*/



