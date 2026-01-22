#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "traitement_test.h"

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
        char *fonction = token;

        token = strtok(NULL, ";");
        char *parametres = token;

        strcpy(fonctions[func_count].mot, mot);
        strcpy(fonctions[func_count].fonction, fonction);
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

char* trouver_principal(char *mot) {
    for (int i = 0; i < syn_count; i++) {
        if (strcmp(mot, synonymes[i].synonyme) == 0) {
            return synonymes[i].principal;
        }
    }
    return mot;
}

char* trouver_type(char *mot) {
    for (int i = 0; i < type_count; i++) {
        if (strcmp(mot, types[i].mot) == 0)
            return types[i].type;
    }
    return "INCONNU";
}

Fonction* trouver_fonction(char *mot) {
    for (int i = 0; i < func_count; i++) {
        if (strcmp(mot, fonctions[i].mot) == 0)
            return &fonctions[i];
    }
    return NULL;
}


void traiter_texte(char *texte) {
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
                        } else if (strcmp(type_j, "OBJET") == 0 && strcmp(func->noms_param[rempli], "object") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        } else if (strcmp(type_j, "COULEUR") == 0 && strcmp(func->noms_param[rempli], "color") == 0) {
                            strcpy(val_param[rempli], tokens[j]);
                            rempli++;
                        }
                    }

                    j++;
                }

                printf("%s(", func->fonction);
                for (int k = 0; k < func->param_count; k++) {
                    printf("%s=%s", func->noms_param[k], val_param[k]);
                    if (k < func->param_count - 1)
                        printf(", ");
                }
                printf(")\n");
            }

            while (i < token_count && strcmp(trouver_type(tokens[i]), "SEPARATEUR") != 0) {
                i++;
            }
        }

        i++;
    }
}

int main() {
    charger_synonymes("../../configuration/syn_FR.txt");
    charger_types("../../configuration/type_FR.txt");
    charger_fonctions("../../configuration/fonction_FR.txt");

    char texte[MAX_LINE];
    printf("Entrez une phrase : ");
    fgets(texte, MAX_LINE, stdin);

    traiter_texte(texte);
    return 0;
}
