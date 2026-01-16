#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "traitement.h"

Lexique charger_lexique(const char *nom_fichier) {
    Lexique lexique = {NULL, 0};
    FILE *file = fopen(nom_fichier, "r");
    if (!file) return lexique;

    char buffer[128];

    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;

        lexique.mots = realloc(lexique.mots, sizeof(char *) * (lexique.indice + 1));
        lexique.mots[lexique.indice] = strdup(buffer);
        lexique.indice ++;
    }

    fclose(file);
    return lexique;
}

int mot_dans_lexique(const char *mot, const Lexique *lexique){
    for (int i = 0; i < lexique->indice; i ++){
       if (strcasecmp(mot, lexique->mots[i]) == 0){
        return 1;
       }
    }
    return 0;
}

int est_nombre(const char *mot){
    if (mot == NULL || *mot == '\0') return 0;

    int i = 0;
    while(mot[i] !='\0'){
        if (!isdigit((unsigned char) mot[i]))return 0;
        i++;
    }
    return 1;
}

Instruction text_to_instruction(const char *texte, const Lexique *lexique){
    Instruction instr = {NULL, 0};

    char *copie = strdup(texte);
    if (!copie) return instr;

    char *token = strtok(copie, " ");

    while (token){
        if (mot_dans_lexique(token, lexique) || est_nombre(token)){
            instr.mots = realloc(instr.mots, sizeof(char *) * (instr.indice + 1));
            instr.mots[instr.indice] = strdup(token);
            instr.indice ++;
        }
        token = strtok(NULL, " ");
    }

    free(copie);
    return instr;
}

void afficher_Instruction(Instruction instr){
    printf("[ ");
    for (int i = 0; i < instr.indice; i++){
        printf("%s ",instr.mots[i]);
    }
    printf("] \n");
}

void libere_lexique(Lexique *lexique){
    for (int i = 0; i < lexique->indice; i ++){
        free(lexique->mots[i]);
    }
    free(lexique->mots);
}

void libere_instruction(Instruction *instr){
    for (int i = 0; i < instr->indice; i ++){
        free(instr->mots[i]);
    }
    free(instr->mots);
}