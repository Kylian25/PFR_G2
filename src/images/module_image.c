#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "module_image.h"
#include "../outils/outils.h"

/**
 * créer l'image
 * - creer_image: alloue de la mémoire pour une nouvelle image avec les dimensions et le nombre de canaux spécifiés.
 * - Elle initialise également les données des pixels.
 */
Image* creer_image(int largeur, int hauteur, int nombre_de_canaux) {
    Image *image = (Image*)malloc(sizeof(Image));
    if (image == NULL){
        ajout_log("erreur lors de la création de l'image dans creer_image");
        return NULL;
    }
    image->largeur=largeur;
    image->hauteur=hauteur;
    image->nombre_de_canaux=nombre_de_canaux;
    image->data= (Pixel*)malloc(largeur*hauteur*sizeof(Pixel));
    if (image->data == NULL){
        ajout_log("erreur lors de l'atribution des pixels à l'image dans creer_image");
        free(image);
        return NULL;
    }
    return image;  
}

/*
 * - liberer_image: libère la mémoire allouée pour une image.
 */
void liberer_image(Image *img) {
    if (img != NULL) {
        if (img->data != NULL) {
            free(img->data); 
        }
        free(img); 
        ajout_log("libération de l'image réussie dans liberer_image");
    }
}

/**
 * - get_pixel: récupère un pixel à une position donnée dans une image.
 */ 
Pixel get_pixel(Image *image, int x, int y) {
    if (x < 0 || x >= image->largeur || y < 0 || y >= image->hauteur) {
        Pixel vide = {0, 0, 0}; 
        ajout_log("Coordonnées de pixel invalides dans get_pixel");
        return vide; 
        
    }
    
    return image->data[y * image->largeur + x];
    ajout_log("Récupération du pixel réussie dans get_pixel");
    
}

/**
 * charger_image: charge une image à partir d'un fichier texte.
 * - Le fichier texte doit contenir les dimensions de l'image suivies des valeurs RGB des pixels.
 */
Image* charger_image(const char* chemin_fichier_texte) {
    int largeur, hauteur, nombre_de_canaux;
    FILE *f = fopen(chemin_fichier_texte, "r");

    if (f == NULL) {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"Erreur : Impossible d'ouvrir le fichier %s\n", chemin_fichier_texte);
        sprintf(message_2,"Error: Unable to open file %s\n", chemin_fichier_texte);
        message_console(message_1, message_2);
        ajout_log("Erreur : Impossible d'ouvrir le fichier dans charger_image");
        return NULL;
    }

    ajout_log("Ouverture du fichier texte réussie\n");
    fscanf(f, "%d %d %d", &hauteur, &largeur, &nombre_de_canaux);
    ajout_log("Lecture des dimensions de l'image réussie dans charger_image");
    
    Image *image = creer_image(largeur, hauteur, nombre_de_canaux); 
    
    if (nombre_de_canaux != 3) {
        fclose(f);
        liberer_image(image);
        ajout_log("Erreur : format non RGB dans charger_image");
        return NULL;
    }

     int total_pixels = largeur * hauteur;
     int valeur;

    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            ajout_log("Erreur de lecture du Rouge dans charger_image");
            break; 
        }
        image->data[i].r = (unsigned char)valeur;
    }
    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            ajout_log("Erreur de lecture du Vert dans charger_image");
            break; 
        }
        image->data[i].g = (unsigned char)valeur;
    }
    for(int i = 0; i < total_pixels; i++) {
        if (fscanf(f, "%d", &valeur) != 1) {
            ajout_log("Erreur de lecture du Bleu dans charger_image");
            break; 
        }
        image->data[i].b = (unsigned char)valeur;
    }

    fclose(f);
    ajout_log("Image chargée avec succès dans charger_image");
    
    return image;
    
}

/** 
 * est_couleur_cible: vérifie si un pixel correspond à une couleur cible spécifiée.
 */
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

/** 
 * @trouver les positions des objets détectés dans l'image
 * - trouver_positions: analyse une image pour détecter des objets de couleurs spécifiques (rouge, jaune, bleu).
 * - Elle renvoie un tableau d'objets détectés avec leurs positions et surfaces.
 */
ObjetDetecte* trouver_positions(const char* image_de_entre) {
    Image* img = charger_image(image_de_entre);
    if (img == NULL) {
        ajout_log("Annulation de la détection : Image introuvable dans trouver_positions");
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
            char message_1[1024];
            char message_2[1024];
            sprintf(message_1,"%s : Non détecté.\n", noms[i]);
            sprintf(message_2,"%s : Not detected.\n", noms[i]);
            message_console(message_1, message_2);
            ajout_log("Objet non détecté dans trouver_positions");
        } else {
            char message_1[1024];
            char message_2[1024];
            sprintf(message_1,"%s détecté\nPosition : X[%d-%d] Y[%d-%d]\nSurface  : %ld pixels\n",
                    noms[i], objets[i].x_min, objets[i].x_max, 
                    objets[i].y_min, objets[i].y_max, objets[i].surface);
            sprintf(message_2,"%s detected\nPosition : X[%d-%d] Y[%d-%d]\nSurface  : %ld pixels\n",
                    noms[i], objets[i].x_min, objets[i].x_max, 
                    objets[i].y_min, objets[i].y_max, objets[i].surface);
            message_console(message_1, message_2);
            ajout_log("Objet détecté avec succès dans trouver_positions");
        }
    }

    liberer_image(img);
    return objets; 
}

/**
 * afficher_resultats: affiche les résultats de la détection en traçant des rectangles autour des objets détectés.
 * - Elle utilise un script Python externe pour dessiner les rectangles sur l'image JPEG.
 */
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
    ajout_log("Affichage des résultats avec rectangles dans afficher_resultats");

}


/**
 * detecter_forme: détermine la forme d'un objet détecté en fonction de son ratio surface/boîte englobante.
 */
const char* detecter_forme(ObjetDetecte obj) {
    long largeur = (obj.x_max - obj.x_min) + 1;
    long hauteur = (obj.y_max - obj.y_min) + 1;
    long aire_box = largeur * hauteur;
    if (aire_box <= 0) return "INCONNU";
    float ratio = (float)obj.surface / (float)aire_box;
    if (ratio > 0.85) {
        ajout_log("Détection de la forme réussie dans detecter_forme");
        return "cube";
    } else {
        ajout_log("Détection de la forme réussie dans detecter_forme");
        return "balle";
    }
    
}

/**
 * detecter_forme_et_couleur: détecte les formes et couleurs des objets dans une image et enregistre les résultats dans un fichier.
 */
void detecter_forme_et_couleur(const char* fichier_image) {

    ObjetDetecte *obj = trouver_positions(fichier_image);
    
    if (obj == NULL) {
        ajout_log("Erreur : Impossible d'analyser l'image dans detecter_forme_et_couleur");
        return;
    }
    FILE *f = fopen("temp/forme_couleur.txt", "w");
    if (f == NULL) {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"Erreur : Impossible de créer le fichier\n");
        sprintf(message_2,"Error: Unable to create file\n");
        message_console(message_1, message_2);
        ajout_log("Erreur : Impossible de créer le fichier dans detecter_forme_et_couleur");
        free(obj);
        return;
    }
    const char *noms_couleurs[] = {"rouge", "jaune", "bleu"};
    for (int i = 0; i < 3; i++) {

        if (obj[i].surface > 40) {
            const char* forme_trouvee = detecter_forme(obj[i]);
            fprintf(f, "%s %s %d %d %d %d\n", 
                    forme_trouvee, 
                    noms_couleurs[i], 
                    obj[i].x_min, obj[i].x_max, 
                    obj[i].y_min, obj[i].y_max);
            
            char message_1[1024];
            char message_2[1024];
            sprintf(message_1,"Sauvegardé : %s %s \n", forme_trouvee, noms_couleurs[i]);
            sprintf(message_2,"Saved : %s %s \n", forme_trouvee, noms_couleurs[i]);
            message_console(message_1, message_2);
            ajout_log("Sauvegarde des résultats réussie dans detecter_forme_et_couleur");
        }
    }
    fclose(f);
    free(obj);
    ajout_log("Les résultats ont été écrits dans 'temp/forme_couleur.txt'.");
    ajout_log("Détection de forme et couleur terminée dans detecter_forme_et_couleur");
}

/**
 * reconnaissance_forme_couleur: reconnaît une forme et une couleur spécifiques dans une image.
 * - Elle enregistre les résultats dans un fichier si la forme et la couleur sont trouvées.
 */
void reconnaissance_forme_couleur(char* fichier_image, char* couleur, char* forme) {

    ObjetDetecte *objets = trouver_positions(fichier_image);
    if (objets == NULL) return;
    int index_cible = -1;

    if (strcmp(couleur, "rouge") == 0) {
        index_cible = 0;
    } else if (strcmp(couleur, "jaune") == 0) {
        index_cible = 1;
    } else if (strcmp(couleur, "bleu") == 0) {
        index_cible = 2;
    } else {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"Erreur : La couleur '%s' n'est pas gérée (ROUGE, JAUNE, BLEU).\n", couleur);
        sprintf(message_2,"Error: The color '%s' is not handled (RED, YELLOW, BLUE).\n", couleur);
        message_console(message_1, message_2);
        free(objets);
        return;
    }
    ObjetDetecte obj = objets[index_cible];
    if (obj.surface < 40) {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"Recherche échouée : Aucun objet %s trouvé dans l'image.\n", couleur);
        sprintf(message_2,"Search failed: No %s object found in the image.\n", couleur);
        message_console(message_1, message_2);
        free(objets);
        return;
    }

    const char* forme_detectee = detecter_forme(obj);
    if (strcmp(forme_detectee, forme) == 0) {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"%s %s trouvé \nPosition: X[%d  %d] Y[%d  %d]\n", 
               forme, couleur, 
               obj.x_min, obj.x_max, 
               obj.y_min, obj.y_max);
        sprintf(message_2,"%s %s found \nPosition: X[%d  %d] Y[%d  %d]\n", 
               forme, couleur, 
               obj.x_min, obj.x_max, 
               obj.y_min, obj.y_max);
        message_console(message_1, message_2);
        FILE *f = fopen("temp/reconnaissance.txt", "w");
        if (f == NULL) {
            message_console("Erreur : Impossible de créer le fichier",
                            "Error: Unable to create file");
             free(objets);
             return;
            }
        fprintf(f, "['%s', '%s', '%d', '%d', '%d', '%d']\n", 
                forme_detectee, 
                couleur, 
                obj.x_min, obj.x_max, 
                obj.y_min, obj.y_max);
        fclose(f);
        ajout_log("Reconnaissance de forme et couleur réussie dans reconnaissance_forme_couleur");
    } else {
        char message_1[1024];
        char message_2[1024];
        sprintf(message_1,"la forme %s n'a pas été trouvée pour la couleur %s (trouvé: %s).\n", 
               forme, couleur, forme_detectee);
        sprintf(message_2,"the shape %s was not found for the color %s (found: %s).\n", 
               forme, couleur, forme_detectee);
        message_console(message_1, message_2);  
        ajout_log("Échec de la reconnaissance de forme et couleur dans reconnaissance_forme_couleur");
    }
    free(objets);
}

/**
 * traitement_image: effectue le traitement complet d'une image en détectant les objets et en affichant les résultats.
 */

void traitement_image(char * fichier_texte, char * fichier_jpeg) {
    
    afficher_resultats(fichier_texte,fichier_jpeg);
    detecter_forme_et_couleur(fichier_texte);
    
}
