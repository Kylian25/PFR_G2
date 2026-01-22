#ifndef IMAGES_H  // éviter les erreurs lors de la conpilation 

#define IMAGES_H

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

typedef struct {
    int largeur;
    int hauteur;
    int nombre_de_canaux;
    Pixel *data;
} Image;

Image* creer_image(int largeur, int hauteur, int nombre_de_canaux);

void liberer_image(Image *img);

Pixel get_pixel(Image *image, int x, int y);

Image* charger_image(const char* chemin_fichier_texte);

typedef enum {
    CIBLE_ROUGE,
    CIBLE_JAUNE,
    CIBLE_BLEU
} CouleurCible;

int est_couleur_cible(Pixel p, CouleurCible cible);

typedef struct {
    CouleurCible type_couleur; 
    int x_min, y_min;
    int x_max, y_max;
    long surface;
} ObjetDetecte;

ObjetDetecte* trouver_positions(const char* image_de_entre);

void afficher_resultats(const char* fichier_entree,const char* image_jpeg);

void traitement_image();

#endif
