#ifndef IMAGES_H  
#define IMAGES_H

#define DELTA 50
//bleu
#define SEUIL_BLEU 40
// rouge
#define SEUIL_ROUGE 60

// jaune 
#define MAX_JAUNE_bleu 90

typedef struct{
    unsigned char r;
    unsigned char v;
    unsigned char b;
}PIXEL;

typedef struct{
    int nb_canaux, hauteur,largeur;
    PIXEL **matrice;   // pas de tableau fixe (taille pas connue a la compilation)
}IMAGE;   

typedef struct {
    int forme, couleur;
    unsigned char coin_HD[2];
    unsigned char coin_HG[2];
}OBJET;

typedef struct{
    unsigned char r,v,b;
}COULEUR;


// FONCTIONS

// images

IMAGE * creer_image(int h, int l);
void liberer_image(IMAGE * img);

IMAGE * lire_image(const char * nom_fichier);
void sauvegarder_image(const char * nom_fichier, IMAGE *img);
void afficher_image(const char *nom_image_filtree, const char * nom_image_originale);

// couleur

IMAGE * filtrer_bleu(IMAGE * image, unsigned char seuil,unsigned char delta);
IMAGE * filtrer_jaune(IMAGE * image_source);
IMAGE * filtrer_rouge(IMAGE * image_source, unsigned char seuil, unsigned char delta);

// traitement 

IMAGE * filtrer_image(const char * nom_fichier);
void traiter_image();
int nb_pixel_couleur(IMAGE * image, COULEUR * c);



#endif