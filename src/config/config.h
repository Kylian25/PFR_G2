#ifndef CONFIG_H  // éviter les erreurs lors de la conpilation 
#define CONFIG_H


// valeurs par defaut 

#define DEF_LANGUE "FR"
#define DEF_TYPE_REQUETE "texte"
#define DEF_DIST_DEP 1
#define DEF_TPS_DEP 1
#define DEF_DIST_DET 1
#define DEF_ANGLE 100

typedef struct {

    char langue[15];
    char type_requete[15];
    int dist_dep;
    int tps_dep;
    int dist_det;
    int angle;

}CONFIG_ROBOT;

extern CONFIG_ROBOT config;

void init_config();
void afficher_fichier_config();
void sauvegarder_config();
void modifier_config();
void choix_langue();

#endif