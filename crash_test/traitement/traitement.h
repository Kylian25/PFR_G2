#ifndef TRAITEMENT_H
#define TRAITEMENT_H

// Structures

typedef struct s_lexique{
    char **mots;
    int indice;
} Lexique;

typedef struct s_instruction{
    char **mots;
    int indice;
} Instruction;

// Fonctions

Lexique charger_lexique(const char *nom_fichier);

int mot_dans_lexique(const char *mot, const Lexique *lexique);
int est_nombre(const char *mot);

Instruction text_to_instruction(const char *texte, const Lexique *lexique);
void afficher_Instruction(Instruction instr);

void libere_lexique(Lexique *lexique);
void libere_instruction(Instruction *instr);


#endif