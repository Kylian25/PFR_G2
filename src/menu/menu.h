#ifndef MENU_H // si menu_h non definie avant , si faux, le compilateur ignore jusqu'au endif
#define MENU_H  // si condition de dessus vraie

// fonction principale du menu
void demarrer_menu(void);

// fonctions des sous-menus
void menu_mode_utilisateur(void);
void menu_mode_administrateur(void);

// autres fonctions
void afficher_menu(const char *titre, const char *options[], int nombre_options);
int obtenir_choix_utilisateur(int choix_maximum);

#endif // fin de ifndef 