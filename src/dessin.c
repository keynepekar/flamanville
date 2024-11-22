#include <math.h>
#include <stdlib.h>
#include "constantes.h"
#include "structures.h"
#include "dessin.h"
#include "transformations.h"
#include "file_manager.h"

/*
 * Remplissage par balayage du buffer avec une teinte unie.
*/
void fond(RVB **t, RVB teinte) {
    for (int i = 0; i < H_FHD; ++i) {
        for (int j = 0; j < W_FHD; ++j) {
            t[i][j] = teinte;
        }
    }
}

/*
 * Remplissage par balayage de la zone rectangulaire délimitée par les coordonnées p1 et p2.
 * p1 : point haut gauche
 * p2 : point bas droit
 */
void dessiner_rectangle(RVB **t, COORD p1, COORD p2, RVB teinte) {
    for (int i = p1.y; i <= p2.y; ++i) {
        for (int j = p1.x; j <= p2.x; ++j) {
            if (i >= 0 && i < H_FHD && j >= 0 && j < W_FHD) {
                t[i][j] = teinte;
            }
        }
    }
}

/*
 * Dessine un segment entre p1 et p2 en utilisant l'algorithme de Bresenham.
 * teinte : couleur du segment
 * edgeList : liste des arêtes pour le remplissage (scanline)
 */
void dessiner_segment(RVB **t, COORD p1, COORD p2, RVB teinte, EdgeList *edgeList) {
    // Gestion de la liste des arêtes
    if (edgeList->count >= edgeList->capacity) {
        edgeList->capacity *= 2;
        edgeList->edges = realloc(edgeList->edges, edgeList->capacity * sizeof(Edge));
        if (edgeList->edges == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire lors du redimensionnement de edgeList.edges.\n");
            exit(EXIT_FAILURE);
        }
    }
    edgeList->edges[edgeList->count].x0 = p1.x;
    edgeList->edges[edgeList->count].y0 = p1.y;
    edgeList->edges[edgeList->count].x1 = p2.x;
    edgeList->edges[edgeList->count].y1 = p2.y;
    edgeList->count++;

    // Algorithme de Bresenham pour le tracé du segment
    int x0 = (int)p1.x;
    int y0 = (int)p1.y;
    int x1 = (int)p2.x;
    int y1 = (int)p2.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < W_FHD && y0 >= 0 && y0 < H_FHD) {
            t[y0][x0] = teinte;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

/*
 * Dessine une courbe de Bézier cubique définie par les points de contrôle p0 à p3.
 * Utilise l'algorithme de De Casteljau pour le calcul.
 * teinte : couleur de la courbe
 * edgeList : liste des arêtes pour le remplissage (scanline)
 */
void dessiner_bezier_cubique(RVB **t, COORD p0, COORD p1, COORD p2, COORD p3, RVB teinte, EdgeList *edgeList) {
    int pas = 1000;
    COORD prevPoint = p0;
    for (int i = 1; i <= pas; ++i) {
        double t_param = (double)i / pas;
        double x = pow(1 - t_param, 3) * p0.x +
                   3 * pow(1 - t_param, 2) * t_param * p1.x +
                   3 * (1 - t_param) * pow(t_param, 2) * p2.x +
                   pow(t_param, 3) * p3.x;
        double y = pow(1 - t_param, 3) * p0.y +
                   3 * pow(1 - t_param, 2) * t_param * p1.y +
                   3 * (1 - t_param) * pow(t_param, 2) * p2.y +
                   pow(t_param, 3) * p3.y;

        COORD currentPoint = {x, y};

        if (edgeList->count >= edgeList->capacity) {
            edgeList->capacity *= 2;
            edgeList->edges = realloc(edgeList->edges, edgeList->capacity * sizeof(Edge));
        }
        edgeList->edges[edgeList->count].x0 = prevPoint.x;
        edgeList->edges[edgeList->count].y0 = prevPoint.y;
        edgeList->edges[edgeList->count].x1 = currentPoint.x;
        edgeList->edges[edgeList->count].y1 = currentPoint.y;
        edgeList->count++;

        prevPoint = currentPoint;
    }
}

/*
 * Dessine un tracé à partir des données de segments et courbes.
 * Applique des transformations d'échelle et de translation.
 * donneesTrace : données du tracé (segments, courbes)
 * offsetX, offsetY : décalages pour la translation
 * edgeList : liste des arêtes pour le remplissage (scanline)
 */
void dessiner_trace(RVB **t, DonneesTrace donneesTrace, RVB teinte, double offsetX, double offsetY, EdgeList *edgeList) {
    COORD pointCourant = {0, 0};

    double scaleX = W_FHD / 1920.0;
    double scaleY = H_FHD / 1080.0;

    for (int i = 0; i < donneesTrace.nb; ++i) {
        SegmentsTrace segment = donneesTrace.segments[i];
        switch (segment.type) {
            case MOVE:
                pointCourant = segment.points[1];
                pointCourant = mise_echelle_point(pointCourant, scaleX, scaleY);
                pointCourant = translation_point(pointCourant, offsetX, offsetY);
                break;
            case LINE: {
                COORD p1 = pointCourant;
                COORD p2 = segment.points[1];
                p2 = mise_echelle_point(p2, scaleX, scaleY);
                p2 = translation_point(p2, offsetX, offsetY);
                dessiner_segment(t, pointCourant, p2, teinte, edgeList);
                pointCourant = p2;
                break;
            }
            case CUBIC_BEZIER: {
                COORD p0 = pointCourant;
                COORD p1 = segment.points[1];
                COORD p2 = segment.points[2];
                COORD p3 = segment.points[3];
                p1 = mise_echelle_point(p1, scaleX, scaleY);
                p1 = translation_point(p1, offsetX, offsetY);
                p2 = mise_echelle_point(p2, scaleX, scaleY);
                p2 = translation_point(p2, offsetX, offsetY);
                p3 = mise_echelle_point(p3, scaleX, scaleY);
                p3 = translation_point(p3, offsetX, offsetY);
                dessiner_bezier_cubique(t, p0, p1, p2, p3, teinte, edgeList);
                pointCourant = p3;
                break;
            }
            case CLOSE: {
                COORD p1 = pointCourant;
                COORD p2 = segment.points[1];
                p2 = mise_echelle_point(p2, scaleX, scaleY);
                p2 = translation_point(p2, offsetX, offsetY);
                dessiner_segment(t, pointCourant, p2, teinte, edgeList);
                pointCourant = p2;
                break;
            }
            default:
                break;
        }
    }
}

/*
 * Remplit une forme fermée en utilisant l'algorithme de scanline.
 * edgeList : liste des arêtes de la forme
 * couleurRemplissage : couleur pour remplir la forme
 */
void remplir_scanline(RVB **t, EdgeList *edgeList, RVB couleurRemplissage) {
    int yMin = H_FHD - 1, yMax = 0;
    for (int i = 0; i < edgeList->count; ++i) {
        int y0 = (int)round(edgeList->edges[i].y0);
        int y1 = (int)round(edgeList->edges[i].y1);
        if (y0 < yMin) yMin = y0;
        if (y1 < yMin) yMin = y1;
        if (y0 > yMax) yMax = y0;
        if (y1 > yMax) yMax = y1;
    }
    if (yMin < 0) yMin = 0;
    if (yMax >= H_FHD) yMax = H_FHD - 1;

    for (int y = yMin; y <= yMax; ++y) {
        double *intersections = malloc(edgeList->count * sizeof(double));
        int nbIntersections = 0;

        for (int i = 0; i < edgeList->count; ++i) {
            double x0 = edgeList->edges[i].x0;
            double y0 = edgeList->edges[i].y0;
            double x1 = edgeList->edges[i].x1;
            double y1 = edgeList->edges[i].y1;

            // Ignorer les arêtes horizontales
            if (y0 == y1) continue;

            // Trouver les intersections avec la scanline
            if ((y >= y0 && y < y1) || (y >= y1 && y < y0)) {
                double xIntersect = x0 + ((y - y0) * (x1 - x0)) / (y1 - y0);
                intersections[nbIntersections++] = xIntersect;
            }
        }

        // Trier les intersections
        for (int i = 0; i < nbIntersections - 1; ++i) {
            for (int j = i + 1; j < nbIntersections; ++j) {
                if (intersections[i] > intersections[j]) {
                    double temp = intersections[i];
                    intersections[i] = intersections[j];
                    intersections[j] = temp;
                }
            }
        }

        // Remplir les pixels entre les paires d'intersections
        for (int i = 0; i < nbIntersections - 1; i += 2) {
            int xStart = (int)ceil(intersections[i]);
            int xEnd = (int)floor(intersections[i + 1]);

            if (xStart < 0) xStart = 0;
            if (xEnd >= W_FHD) xEnd = W_FHD - 1;

            for (int x = xStart; x <= xEnd; ++x) {
                if (x >= 0 && x < W_FHD && y >= 0 && y < H_FHD) {
                    t[y][x] = couleurRemplissage;
                }
            }
        }

        free(intersections);
    }
}

/*
 * Remplit un rectangle avec un dégradé vertical entre couleurHaut et couleurBas.
 * p1 : point haut gauche
 * p2 : point bas droit
 * couleurHaut : couleur en haut du rectangle
 * couleurBas : couleur en bas du rectangle
 */
void fond_degrade_rectangle(RVB **t, COORD p1, COORD p2, RVB couleurHaut, RVB couleurBas) {
    int yStart = (int)p1.y;
    int yEnd = (int)p2.y;
    int xStart = (int)p1.x;
    int xEnd = (int)p2.x;

    for (int i = yStart; i <= yEnd; ++i) {
        double ratio = (double)(i - yStart) / (yEnd - yStart);
        RVB couleurActuelle;
        couleurActuelle.R = (int)((1 - ratio) * couleurHaut.R + ratio * couleurBas.R);
        couleurActuelle.V = (int)((1 - ratio) * couleurHaut.V + ratio * couleurBas.V);
        couleurActuelle.B = (int)((1 - ratio) * couleurHaut.B + ratio * couleurBas.B);

        for (int j = xStart; j <= xEnd; ++j) {
            if (i >= 0 && i < H_FHD && j >= 0 && j < W_FHD) {
                t[i][j] = couleurActuelle;
            }
        }
    }
}

/*
 * Dessine et remplit une forme à partir d'un fichier de données SVG.
 * nomFichier : chemin du fichier contenant les données de la forme
 * couleurContour : couleur du contour (utilisée pour le tracé)
 * couleurRemplissage : couleur pour remplir la forme
 * offsetX, offsetY : décalages pour la position de la forme
 */
void dessiner_et_remplir_forme(RVB **im, const char *nomFichier, RVB couleurContour, RVB couleurRemplissage, double offsetX, double offsetY) {
    DonneesTrace donneesTrace = lire_svg(nomFichier);

    couleurContour = couleurRemplissage; // changement de DA et ça nous facilite la vie !

    EdgeList edgeList;
    edgeList.count = 0;
    edgeList.capacity = 1000;
    edgeList.edges = malloc(edgeList.capacity * sizeof(Edge));
    if (edgeList.edges == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour edgeList.edges.\n");
        exit(EXIT_FAILURE);
    }

    dessiner_trace(im, donneesTrace, couleurContour, offsetX, offsetY, &edgeList);

    remplir_scanline(im, &edgeList, couleurRemplissage);

    free(donneesTrace.segments);
    free(edgeList.edges);
}

/*
 * Dessine un soleil avec un effet de halo (dégradé radial).
 * rayonInterieur : rayon du disque central plein
 * rayonExterieur : rayon du halo extérieur
 * couleurSoleil : couleur du soleil
 */
void dessiner_soleil(RVB **t, COORD centre, double rayonInterieur, double rayonExterieur, RVB couleurSoleil) {
    int xMin = (int)(centre.x - rayonExterieur);
    int xMax = (int)(centre.x + rayonExterieur);
    int yMin = (int)(centre.y - rayonExterieur);
    int yMax = (int)(centre.y + rayonExterieur);

    if (xMin < 0) xMin = 0;
    if (xMax >= W_FHD) xMax = W_FHD - 1;
    if (yMin < 0) yMin = 0;
    if (yMax >= H_FHD) yMax = H_FHD - 1;

    for (int y = yMin; y <= yMax; ++y) {
        for (int x = xMin; x <= xMax; ++x) {
            double dx = x - centre.x;
            double dy = y - centre.y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= rayonInterieur) {
                t[y][x] = couleurSoleil;
            } else if (distance <= rayonExterieur) {
                double ratio = (distance - rayonInterieur) / (rayonExterieur - rayonInterieur);

                RVB couleurFond = t[y][x];

                RVB couleurMelangee;
                couleurMelangee.R = (int)((1 - ratio) * couleurSoleil.R + ratio * couleurFond.R);
                couleurMelangee.V = (int)((1 - ratio) * couleurSoleil.V + ratio * couleurFond.V);
                couleurMelangee.B = (int)((1 - ratio) * couleurSoleil.B + ratio * couleurFond.B);

                t[y][x] = couleurMelangee;
            }
        }
    }
}

/*
 * Dessin à balayage de ligne pour un trifolium avec rotation sur son centre.
 * Rappel de l'équation implicite : f(x,y) = (x^2 + y^2)^2 - a x (x^2 - 3 y^2)
 * a : longueur des pétales
 * angle : en radians
 */
void trifolium(RVB **im, RVB teinte, double a, COORD centre, double angle) {
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);

    double dx = centre.x;
    double dy = centre.y;

    for (int i = 0; i < H_FHD; ++i) {
        for (int j = 0; j < W_FHD; ++j) {
            double x = j;
            double y = i;

            // translation
            double xt = x - dx;
            double yt = y - dy;

            // rotation
            double x2 = cosTheta * xt + sinTheta * yt;
            double y2 = -sinTheta * xt + cosTheta * yt;

            double f = pow(x2 * x2 + y2 * y2, 2) - a * x2 * (x2 * x2 - 3 * y2 * y2);

            if (f < 0) {
                im[i][j] = teinte;
            }
        }
    }
}

/*
 * Dessine un quadrifolium avec rotation sur son centre.
 * Rappel de l'équation implicite : f(x,y) = (x^2 + y^2)^3 - a^2 (x^4 - 6 x^2 y^2 + y^4)
 * a : longueur des pétales
 * angle : en radians
 */
void quadrifolium(RVB **im, RVB teinte, double a, COORD centre, double angle) {
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);

    double dx = centre.x;
    double dy = centre.y;

    for (int i = 0; i < H_FHD; ++i) {
        for (int j = 0; j < W_FHD; ++j) {
            double x = j;
            double y = i;

            // translation
            double xt = x - dx;
            double yt = y - dy;

            // rotation
            double x2 = cosTheta * xt + sinTheta * yt;
            double y2 = -sinTheta * xt + cosTheta * yt;

            // 4 feuilles svp chef
            double f = pow(x2 * x2 + y2 * y2, 3) - a * a * (pow(x2, 4) - 6 * x2 * x2 * y2 * y2 + pow(y2, 4));

            if (f < 0) {
                im[i][j] = teinte;
            }
        }
    }
}
