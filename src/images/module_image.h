//convrtir en ppm
void convertir_image_en_ppm(const char *image_jpeg,const char *image_ppm);

//convetir ppm en jpeg
void convertir_image_en_jpeg(const char *image_de_entre, const char *image_de_sortie);

//definition des pixel
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;


//mon image
typedef struct {
    int largeur;
    int hauteur;
    Pixel *data;
} Image;


Image* creer_image(int largeur, int hauteur);
void liberer_image(Image *img);
Pixel get_pixel(Image *image, int x, int y);
Image* charger_image(const char* image_de_entre);
void sauvegarder_image_ppm(Image *img, const char *nom_fichier);


typedef enum {
    CIBLE_ROUGE,
    CIBLE_JAUNE,
    CIBLE_BLEU
} CouleurCible;


typedef struct {
    CouleurCible type_couleur; 
    int x_min, y_min;
    int x_max, y_max;
    long surface;
} ObjetDetecte;

int est_couleur_cible(Pixel p, CouleurCible cible);
ObjetDetecte* trouver_positions(const char* image_de_entre) ;
void dessiner_rectangle(Image *img, ObjetDetecte obj, Pixel couleur);
void afficher_resultats(const char* fichier_entree, const char* fichier_sortie);
Image* charger_image(const char* chemin_fichier_texte);
ObjetDetecte* trouver_positions(const char* image_de_entre);