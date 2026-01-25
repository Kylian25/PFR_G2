#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "traitement.h"
#include "../config/config.h"
#include "../outils/outils.h"


// Déclaration des variables de lexique et des compteurs

Synonyme synonymes[MAX_WORDS];
TypeMot types[MAX_WORDS];
Fonction fonctions[MAX_WORDS];

int syn_count = 0, type_count = 0, func_count = 0;


void charger_synonymes(const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    char ligne[MAX_LINE];
    while (fgets(ligne, MAX_LINE, f)) {
        char *token = strtok(ligne, ";");
        nettoyer_mot(token);
        char principal[50];
        strcpy(principal, token);

        while ((token = strtok(NULL, ";\n"))) {
            strcpy(synonymes[syn_count].principal, principal);
            strcpy(synonymes[syn_count].synonyme, token);
            syn_count++;
        }
    }
    fclose(f);
}

void charger_types(const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    char mot[50], type[20];
    while (fscanf(f, "%[^;];%s\n", mot, type) != EOF) {
        strcpy(types[type_count].mot, mot);
        strcpy(types[type_count].type, type);
        type_count++;
    }
    fclose(f);
}

void charger_fonctions(const char *nom_fichier) {
    FILE *f = fopen(nom_fichier, "r");
    char ligne[MAX_LINE];
    while (fgets(ligne, MAX_LINE, f)) {
        ligne[strcspn(ligne, "\n")] = 0;

        char *token = strtok(ligne, ";");
        char *mot = token;

        token = strtok(NULL, ";");
        char *parametres = token;

        strcpy(fonctions[func_count].mot, mot);
        fonctions[func_count].param_count = 0;

        char *p = strtok(parametres, ",");
        while (p) {
            strcpy(fonctions[func_count].noms_param[fonctions[func_count].param_count], p);
            fonctions[func_count].param_count++;
            p = strtok(NULL, ",");
        }

        func_count++;
    }
    fclose(f);
}


void nettoyer_mot(char *mot) {
    mot[strcspn(mot, "\n")] = 0;
}

int est_nombre(const char *mot){
    if (*mot == '\0') return 0;

    for(int i = 0; mot[i]; i ++){
        if (!isdigit((unsigned char) mot[i])) return 0;
    }
    return 1;
}

char* trouver_principal(char *mot) {
    for (int i = 0; i < syn_count; i++) {
        if (strcasecmp(mot, synonymes[i].synonyme) == 0) {
            return synonymes[i].principal;
        }
    }
    return mot;
}

char* trouver_type(char *mot) {
    if (est_nombre(mot)) return "NOMBRE";

    for (int i = 0; i < type_count; i++) {
        if (strcasecmp(mot, types[i].mot) == 0)
            return types[i].type;
    }
    return "INCONNU";
}

Fonction* trouver_fonction(char *mot) {
    for (int i = 0; i < func_count; i++) {
        if (strcasecmp(mot, fonctions[i].mot) == 0)
            return &fonctions[i];
    }
    return NULL;
}


void traiter_texte(char *texte) {
    FILE *file = fopen("temp/instructions.txt","w");
    if (file == NULL) ajout_log("Fichier introuvable");
    char *tokens[MAX_WORDS];
    int token_count = 0;

    char *token = strtok(texte, " ");
    while (token) {
        token[strcspn(token, "\n")] = 0;
        tokens[token_count++] = trouver_principal(token);
        token = strtok(NULL, " ");
    }

    int i = 0;
    while (i < token_count) {

        if (strcmp(trouver_type(tokens[i]), "COMMANDE") == 0) {
            Fonction *func = trouver_fonction(tokens[i]);

            if (func) {

                char val_param[5][50];
                for (int k = 0; k < func->param_count; k++)
                    strcpy(val_param[k], "");

                int rempli = 0;
                int j = i + 1;
                while (j < token_count && strcmp(trouver_type(tokens[j]), "SEPARATEUR") != 0) {

                    char *type_j = trouver_type(tokens[j]);

                    if (rempli < func->param_count) {
                        if (strcmp(type_j, "NOMBRE") == 0 && strcmp(func->noms_param[rempli], "dist") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "NOMBRE") == 0 && strcmp(func->noms_param[rempli], "angle") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "NOMBRE") == 0 && strcmp(func->noms_param[rempli], "pos") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "NOMBRE") == 0 && strcmp(func->noms_param[rempli], "rayon") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "OBJET") == 0 && strcmp(func->noms_param[rempli], "objet") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "COULEUR") == 0 && strcmp(func->noms_param[rempli], "couleur") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "FORME") == 0 && strcmp(func->noms_param[rempli], "forme") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        }
                    }

                    j++;
                }

                fprintf(file,"%s ", func->mot);
                for (int k = 0; k < func->param_count; k++) {
                    fprintf(file,"%s ", val_param[k]);
                }
                fprintf(file,"\n");
            }

            while (i < token_count && strcmp(trouver_type(tokens[i]), "SEPARATEUR") != 0) {
                i++;
            }
        }

        i++;
    }
    fclose(file);
}

void gestion_requetes(char *mode) {

    int c;
    while ((c = getchar()) != '\n' && c != EOF);


    charger_synonymes("configuration/syn_FR.txt");
    charger_types("configuration/type_FR.txt");
    charger_fonctions("configuration/fonction_FR.txt");

    char texte[MAX_LINE];
    int commande_valide = 0;
    char reponse[10];

    do{
        if (strcmp(mode,"txt") == 0){
            message_console("Entrez une commande : ","Write a command : ");
            fgets(texte,MAX_LINE,stdin);
        } else{
            // fonction commande vocale
        }

        traiter_texte(texte);
        system("cat temp/instructions.txt");
        message_console("\nValidez-vous cette commande ? (o/n) \n","\nConfirm the command ? (y/n) \n");
        fgets(reponse,10,stdin);
        reponse[strcspn(reponse, "\n")] = 0;

        if (strcasecmp(reponse,"o") == 0 || strcasecmp(reponse,"y") == 0){
            commande_valide = 1;
            message_console("Commande validée. \n","Command confirmed \n");
        } else{
            message_console("Commande annulée. \n","Command cancelled \n");
        }
    } while (!commande_valide);
}
