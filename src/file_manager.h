#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <stdio.h>
#include "structures.h"

/* Prototypes des fonctions */
RVB **allouer_buffer();
void liberer_buffer(RVB **im);
FILE *lire_image(const char *nomfichier);
void fermer_image(FILE *df);
void ecrire_header(FILE *df);
void ecrire_corps(RVB **t, FILE *df);
DonneesTrace lire_svg(const char *nomfichier);

#endif /* FILE_MANAGER_H */
