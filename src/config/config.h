#ifndef CONFIG_H  // éviter les erreurs lors de la conpilation 
#define CONFIG_H

typedef struct {

    char langue[15];
    char type_requete[15];
    int distance_deplacement;
    int temps_deplacement;
    int distance_detection;
    int angle;

}CONFIG_ROBOT;

extern CONFIG_ROBOT config;  // pour l'utiliser dans les autres .c 


#endif