#ifndef IMAGES_H  
#define IMAGES_H


#define H 300
#define L 300

#define DELTA 50
#define SEUIL_BLEU 40
#define SEUIL_ROUGE 40
#define SEUIL_JAUNE 40

typedef struct{
    int r;
    int v;
    int b;
}PIXEL;

typedef struct{
    int nb_canaux, hauteur,largeur;
    PIXEL matrice[H][L];   
}IMAGE;                 

IMAGE lire_image(const char * chemin);

IMAGE filtrer_bleu(IMAGE * image, int seuil,int delta);

void sauvegarder_image(const char * nom_fichier, IMAGE img);

#endif