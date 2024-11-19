#ifndef DESSIN_H
#define DESSIN_H

#include "structures.h"

void fond(RVB **t, RVB teinte);
void dessiner_rectangle(RVB **t, COORD p1, COORD p2, RVB teinte);
void dessiner_segment(RVB **t, COORD p1, COORD p2, RVB teinte, EdgeList *edgeList);
void dessiner_bezier_cubique(RVB **t, COORD p0, COORD p1, COORD p2, COORD p3, RVB teinte, EdgeList *edgeList);
void dessiner_trace(RVB **t, DonneesTrace donneesTrace, RVB teinte, double offsetX, double offsetY, EdgeList *edgeList);
void remplir_scanline(RVB **t, EdgeList *edgeList, RVB couleurRemplissage);
void fond_degrade_rectangle(RVB **t, COORD p1, COORD p2, RVB couleurHaut, RVB couleurBas);
void dessiner_et_remplir_forme(RVB **im, const char *nomFichier, RVB couleurContour, RVB couleurRemplissage, double offsetX, double offsetY);
void dessiner_soleil(RVB **t, COORD centre, double rayonInterieur, double rayonExterieur, RVB couleurSoleil);

#endif /* DESSIN_H */
