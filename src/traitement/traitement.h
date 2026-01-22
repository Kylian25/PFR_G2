#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#define MAX_WORDS 100
#define MAX_LINE 256

// Définition des structures

typedef struct s_synonyme {
    char principal[50];
    char synonyme[50];
} Synonyme;

typedef struct s_typemot{
    char mot[50];
    char type[20];
} TypeMot;

typedef struct s_fonction{
    char mot[50];
    char fonction[50];
    char noms_param[5][20];
    int param_count;
} Fonction;

// Fonctions de chargement des fichiers de lexique

void charger_synonymes(const char *nom_fichier);
void charger_types(const char *nom_fichier);
void charger_fonctions(const char *nom_fichier);

// Fonctions utilitaires

void nettoyer_mot(char *mot);
char *trouver_principal(char *mot);
char *trouver_type(char *mot);
Fonction *trouver_fonction(char *mot);

// Traitement de texte

void traiter_texte(char *texte);
void gestion_requetes(char *mode);

// Déclaration des variables de lexique

Synonyme synonymes[MAX_WORDS];
TypeMot types[MAX_WORDS];
Fonction fonctions[MAX_WORDS];

#endif