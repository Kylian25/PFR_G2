#ifndef CONFIG_H  // éviter les erreurs lors de la conpilation 
#define CONFIG_H


// valeurs par defaut 

#define DEF_LANGUE "FR"
#define DEF_TYPE_REQUETE "txt"
#define DEF_DIST_DEP 50
#define DEF_TPS_DEP 1
#define DEF_DIST_DET 1
#define DEF_ANGLE 90

// valeurs max/min

#define MAX_DIST_DEP 200
#define MIN_DIST_DEP 10
#define MAX_TPS_DEP 20
#define MIN_TPS_DEP 1
#define MAX_DIST_DET 50
#define MIN_DIST_DET 1
#define MAX_ANGLE 360
#define MIN_ANGLE 20

typedef struct {

    char langue[15];
    char type_requete[15];
    int dist_dep;
    int tps_dep;
    int dist_det;
    int angle;

}CONFIG_ROBOT;

typedef struct{

    char date_cour[50];
    char chemin_log[150];

}LOG;

extern CONFIG_ROBOT config;
extern LOG f_log;

void init_config(CONFIG_ROBOT * config);
void init_log();
void recup_config(CONFIG_ROBOT * config);
void afficher_fichier_config();
void sauvegarder_config();
void modifier_config();
void choix_langue();



#endif