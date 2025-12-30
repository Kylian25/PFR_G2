#ifndef IMAGES_H  
#define IMAGES_H

typedef struct{
    int r;
    int v;
    int b;
}PIXEL;

typedef struct{
    int H;
    int L;
    int nb_canaux;
    PIXEL * donnees;   // permettra de creer un tableau de pixels 
}IMAGE;                // en fonction de H et L avec un malloc 


IMAGE * lire_image(const char * chemin);

void liberer_image(IMAGE *img);


#endif