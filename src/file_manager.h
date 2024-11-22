#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include "structures.h"

/* Prototypes des fonctions */
RVB **allouer_buffer(int largeur, int hauteur);
void liberer_buffer(RVB **t, int hauteur);
FILE *lire_image(const char *nomfichier);
void fermer_image(FILE *df);
void ecrire_header(FILE *df);
void ecrire_corps(RVB **t, FILE *df);
DonneesTrace lire_svg(const char *nomfichier);
RVB **redimensionner_image(RVB **imageFHD, int largeurFHD, int hauteurFHD, int largeurHD, int hauteurHD);

#endif /* FILE_MANAGER_H */
