#include <stdio.h>
#include <stdlib.h>
#include "module_image.h"

// créer l'image
Image* creer_image(int largeur, int hauteur){
    Image *image = (Image*)malloc(sizeof(Image));
    if (image == NULL){
        printf("erreur lors de la création de l'image\n");
        return NULL;
    }
    image->largeur=largeur;
    image->hauteur=hauteur;
    image->data= (Pixel*)malloc(largeur*hauteur*sizeof(Pixel));
    if (image->data == NULL){
        printf("erreur lors de l'atribution des pixels à l'image\n");
        free(image);
        return NULL;
    }
    if(image != NULL && image->data != NULL){
        printf("image créer avec succès\n");
    } 
    return image;  
}


//liberer l'image au cas ou
void liberer_image(Image *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data); 
        }
        free(img); 
        printf("suppression de l'image réussie\n");
    }
}

// récuper directement le pixel
Pixel get_pixel(Image *image, int x, int y) {
    
    if (x < 0 || x >= image->largeur || y < 0 || y >= image->hauteur) {
        Pixel vide = {0, 0, 0}; 
        return vide; 
    }
    return image->data[y * image->largeur + x];
}


// charger l'image
Image* charger_image(const char* chemin_fichier_texte) {
    int largeur, hauteur, nombre_de_canaux;
    FILE *f = fopen(chemin_fichier_texte, "r");

    if (f == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", chemin_fichier_texte);
        return NULL;
    }

    printf("Ouverture du fichier texte réussie\n");

    fscanf(f, "%d %d %d", &largeur, &hauteur, &nombre_de_canaux);
    
    printf("le fichier est du format : Taille %dx%dx%d\n",largeur, hauteur, nombre_de_canaux );

    
    Image *image = creer_image(largeur, hauteur); 

   
    int rouge, vert, bleu;

    for(int i = 0; i < largeur * hauteur; i++) {
        
        if (fscanf(f, "%d %d %d", &rouge, &vert, &bleu) != 3) {
            printf("Erreur ou fin de fichier prématurée au pixel %d\n", i);
            break; 
        }
        image->data[i].r = (unsigned char)rouge;
        image->data[i].g = (unsigned char)vert;
        image->data[i].b = (unsigned char)bleu;
    }

    fclose(f);
    printf("L'image %s a été chargée avec succès en mémoire.\n", chemin_fichier_texte);
    
    return image;
}



//fonction de detection de couleur 
int est_couleur_cible(Pixel p, CouleurCible cible) {
    int seuil = 50;
    switch (cible) {
        case CIBLE_ROUGE:
            return (p.r > seuil && p.r> p.g + seuil  && p.r > p.b + 30);
        
        case CIBLE_JAUNE:
            return (p.r > p.b + 30 && p.g > p.b + 30 && p.b < 90 && abs(p.r - p.g) < 30);

        case CIBLE_BLEU:
             return (p.b >p.r + 30  && p.b > p.g + 30 && p.b > seuil);

        default:
            return 0;
    }
}


//fonction trouver positions
ObjetDetecte* trouver_positions(const char* image_de_entre) {

    Image* img = charger_image(image_de_entre);

    ObjetDetecte *objets = (ObjetDetecte*)malloc(3 * sizeof(ObjetDetecte));
    if (objets == NULL) return NULL; // Sécurité

    CouleurCible liste_couleurs[] = {CIBLE_ROUGE, CIBLE_JAUNE, CIBLE_BLEU};
    const char *noms[] = {"ROUGE", "JAUNE", "BLEU"};
    
    
    for (int i = 0; i < 3; i++) {
        printf("Test de détection de la couleur %s\n", noms[i]);
        CouleurCible couleur_actuelle = liste_couleurs[i];
        
       
        objets[i].type_couleur = couleur_actuelle;
        
        // Initialisation inversée
        objets[i].x_min = img->largeur;
        objets[i].y_min = img->hauteur;
        objets[i].x_max = 0;
        objets[i].y_max = 0;
        objets[i].surface = 0;

        // 3. Scan de l'image
        for (int y = 0; y < img->hauteur; y++) {
            for (int x = 0; x < img->largeur; x++) {
                
                Pixel p = get_pixel(img, x, y);

                if (est_couleur_cible(p, couleur_actuelle)) {
                    // Mise à jour directe dans le tableau
                    if (x < objets[i].x_min) objets[i].x_min = x;
                    if (x > objets[i].x_max) objets[i].x_max = x;
                    
                    if (y < objets[i].y_min) objets[i].y_min = y;
                    if (y > objets[i].y_max) objets[i].y_max = y;
                    
                    objets[i].surface++;
                }
                
            }
        } 
        if(objets[i].surface == 0){
                    printf("Aucun objet %s n'a été détectée\n",noms[i]);
                }
        else{
            printf("Objet %s détectée\n",noms[i]);
            printf("Position: X[%d-%d] Y[%d-%d]\n", objets[i].x_min, objets[i].x_max, objets[i].y_min, objets[i].y_max);
            printf("Surface: %ld pixels\n", objets[i].surface);
        }
    }
    liberer_image(img);
    // On renvoie le tableau complet
    return objets;
}



void afficher_resultats(const char* fichier_entree,const char* image_jpeg) {
    ObjetDetecte *objets = trouver_positions(fichier_entree);
    if (!objets) return;

    FILE *f = fopen("bboxes.txt", "w");
    if (!f) return;

    for (int i = 0; i < 3; i++) {
        if (objets[i].surface > 50) {
            fprintf(f, "%d %d %d %d\n",
                objets[i].x_min,
                objets[i].x_max,
                objets[i].y_min,
                objets[i].y_max
            );
        }
    }

    fclose(f);
    free(objets);
    char cmd[256];
    sprintf(cmd, "python3 tracer_rectangle.py %s bboxes.txt", image_jpeg);
    system(cmd);

}

