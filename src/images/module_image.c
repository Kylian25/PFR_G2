#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "module_image.h"
#include "../outils/outils.h"

// créer l'image
Image* creer_image(int largeur, int hauteur, int nombre_de_canaux) {
    Image *image = (Image*)malloc(sizeof(Image));
    if (image == NULL){
        printf("erreur lors de la création de l'image\n");
        return NULL;
    }
    image->largeur=largeur;
    image->hauteur=hauteur;
    image->nombre_de_canaux=nombre_de_canaux;
    image->data= (Pixel*)malloc(largeur*hauteur*sizeof(Pixel));
    if (image->data == NULL){
        printf("erreur lors de l'atribution des pixels à l'image\n");
        free(image);
        return NULL;
    }
    return image;  
}

// liberer l'image 
void liberer_image(Image *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data); 
        }
        free(img); 
        printf("suppression de l'image réussie\n");
    }
}

// récuper le pixel
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
    printf("Format détecté : %dx%dx%d\n", largeur, hauteur, nombre_de_canaux);
    
    Image *image = creer_image(largeur, hauteur, nombre_de_canaux); 
    
    if (nombre_de_canaux != 3) {
        printf("Erreur : format non RGB (%d canaux)\n", nombre_de_canaux);
        fclose(f);
        liberer_image(image);
        return NULL;
    }

     int total_pixels = largeur * hauteur;
     int valeur;

    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            printf("Erreur de lecture du Rouge au pixel %d\n", i);
            break; 
        }
        image->data[i].r = (unsigned char)valeur;
    }
    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            printf("Erreur de lecture du Vert au pixel %d\n", i);
            break; 
        }
        image->data[i].g = (unsigned char)valeur;
    }
    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            printf("Erreur de lecture du Bleu au pixel %d\n", i);
            break; 
        }
        image->data[i].b = (unsigned char)valeur;
    }

    fclose(f);
    printf("L'image %s a été chargée avec succès.\n", chemin_fichier_texte);
    
    return image;
    
}

//tester couleur
int est_couleur_cible(Pixel p, CouleurCible cible) {
    switch (cible) {
        case CIBLE_ROUGE:
            return (p.r > p.g + 50 && p.r > p.b + 50 && p.r > 120 && p.g < 80 && p.b < 80);
        
        case CIBLE_JAUNE:
            return (abs(p.r - p.g) < 30 && p.r > p.b + 40 && p.g > p.b + 40 && p.r > 120 && p.g > 120);

        case CIBLE_BLEU:
            return (p.b > p.r + 40 && p.b > p.g + 40 && p.b > 120 && p.r < 100 && p.g < 100);

        default:
            return 0;
    }
}


ObjetDetecte* trouver_positions(const char* image_de_entre) {
    Image* img = charger_image(image_de_entre);
    if (img == NULL) {
        printf("Annulation de la détection : Image introuvable.\n");
        return NULL;
    }

    ObjetDetecte *objets = (ObjetDetecte*)malloc(3 * sizeof(ObjetDetecte));
    if (objets == NULL) {
        liberer_image(img);
        return NULL;
    }

    CouleurCible liste_couleurs[] = {CIBLE_ROUGE, CIBLE_JAUNE, CIBLE_BLEU};
    const char *noms[] = {"ROUGE", "JAUNE", "BLEU"};
    
    for (int i = 0; i < 3; i++) {
        CouleurCible couleur_actuelle = liste_couleurs[i];
        objets[i].type_couleur = couleur_actuelle;
        objets[i].x_min = img->largeur;
        objets[i].y_min = img->hauteur;
        objets[i].x_max = 0;
        objets[i].y_max = 0;
        objets[i].surface = 0;

        for (int y = 0; y < img->hauteur; y++) {
            for (int x = 0; x < img->largeur; x++) {
                
                Pixel p = get_pixel(img, x, y);

                if (est_couleur_cible(p, couleur_actuelle)) {
                    if (x < objets[i].x_min) objets[i].x_min = x;
                    if (x > objets[i].x_max) objets[i].x_max = x;
                    if (y < objets[i].y_min) objets[i].y_min = y;
                    if (y > objets[i].y_max) objets[i].y_max = y;
                    objets[i].surface++;
                }
            }
        } 
        if(objets[i].surface == 0){
            printf("%s : Non détecté.\n", noms[i]);
        } else {
            printf("%s détecté\n", noms[i]);
            printf("Position : X[%d-%d] Y[%d-%d]\n", objets[i].x_min, objets[i].x_max, objets[i].y_min, objets[i].y_max);
            printf("Surface  : %ld pixels\n", objets[i].surface);
            
        }
    }

    liberer_image(img);
    return objets; 
}

//encadrer les objets
void afficher_resultats(const char* fichier_entree,const char* image_jpeg) {
    ObjetDetecte *objets = trouver_positions(fichier_entree);
    if (!objets) return;

    FILE *f = fopen("temp/bboxes.txt", "w");
    if (!f) return;

    for (int i = 0; i < 3; i++) {
        if (objets[i].surface > 40) {
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
    sprintf(cmd, "python3 src/images/tracer_rectangle.py %s temp/bboxes.txt", image_jpeg);
    system(cmd);

}


//tester la forme
const char* detecter_forme(ObjetDetecte obj) {
    long largeur = (obj.x_max - obj.x_min) + 1;
    long hauteur = (obj.y_max - obj.y_min) + 1;
    long aire_box = largeur * hauteur;
    if (aire_box <= 0) return "INCONNU";
    float ratio = (float)obj.surface / (float)aire_box;
    if (ratio > 0.85) {
        return "CUBE";
    } else {
        return "BALLE";
    }
}

//renvoyer les coordonné de l'objet détecté avec sa forme et sa couleur
void detecter_forme_et_couleur(const char* fichier_image) {

    ObjetDetecte *obj = trouver_positions(fichier_image);
    
    if (obj == NULL) {
        printf("Erreur : Impossible d'analyser l'image.\n");
        return;
    }
    FILE *f = fopen("temp/forme_couleur.txt", "w");
    if (f == NULL) {
        printf("Erreur : Impossible de créer le fichier");
        free(obj);
        return;
    }
    const char *noms_couleurs[] = {"ROUGE", "JAUNE", "BLEU"};
    for (int i = 0; i < 3; i++) {

        if (obj[i].surface > 40) {
            const char* forme_trouvee = detecter_forme(obj[i]);
            fprintf(f, "%s %s %d %d %d %d\n", 
                    forme_trouvee, 
                    noms_couleurs[i], 
                    obj[i].x_min, obj[i].x_max, 
                    obj[i].y_min, obj[i].y_max);

            printf("Sauvegardé : %s %s \n", forme_trouvee, noms_couleurs[i]);
        }
    }
    fclose(f);
    free(obj);
    printf("Les résultats ont été écrits dans '%s'.\n", "temp/forme_couleur.txt");
}


// fonction principale du module image

void traitement_image() {
    char image_1[]="donnees/image_balles.txt";
    char image_2[]="donnees/image_balles.jpeg";
    afficher_resultats(image_1,image_2);
    detecter_forme_et_couleur(image_1);
}
