#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "traitement.h"
//#include "../config/config.h"

int main(void){

    // Récupération du fichier de lexique

    //CONFIG_ROBOT config = {0};
    //recup_config(&config);
    //char * nom_fichier;

    //if (config.langue == "EN") nom_fichier = strdup("lexique_EN");
    //if (config.langue == "FR") nom_fichier = strdup("lexique_FR");

    Lexique lexique = charger_lexique("../../configuration/lexique_FR");

    char texte[100];

    //if (config.type_requete == "txt"){
        
        // Saisie d'une instruction à exécuter

        printf("Saisissez une instruction à exécuter. \n");
        fgets(texte, sizeof(texte), stdin);
        
        texte[strcspn(texte, "\n")] = '\0';
    //}
    //else{
        
        // Récupération de la commande vocale
    //}

    Instruction instr = text_to_instruction(texte, &lexique);
    afficher_Instruction(instr);

    // Exécution de l'instruction et affichage avec le module de simu

    libere_lexique(&lexique);
    libere_instruction(&instr);

}