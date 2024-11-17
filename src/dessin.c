#include <math.h>
#include <stdlib.h>
#include "constantes.h"
#include "structures.h"
#include "dessin.h"
#include "transformations.h"
#include "file_manager.h"

void fond(RVB **t, RVB teinte) {
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            t[i][j] = teinte;
        }
    }
}

void dessiner_rectangle(RVB **t, COORD p1, COORD p2, RVB teinte) {
    for (int i = p1.y; i <= p2.y; ++i) {
        for (int j = p1.x; j <= p2.x; ++j) {
            if (i >= 0 && i < H && j >= 0 && j < W) {
                t[i][j] = teinte;
            }
        }
    }
}

void dessiner_segment(RVB **t, COORD p1, COORD p2, RVB teinte, EdgeList *edgeList) {
    // cimer Jack E. Bresenham
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

    int x0 = (int)p1.x;
    int y0 = (int)p1.y;
    int x1 = (int)p2.x;
    int y1 = (int)p2.y;

    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < W && y0 >= 0 && y0 < H) {
            t[y0][x0] = teinte;
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void dessiner_bezier_cubique(RVB **t, COORD p0, COORD p1, COORD p2, COORD p3, RVB teinte, EdgeList *edgeList) {
    // on peut remercier Paul de Casteljau pour l'algorithme ! :)
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

void dessiner_trace(RVB **t, DonneesTrace donneesTrace, RVB teinte, double offsetX, double offsetY, EdgeList *edgeList) {
    COORD pointCourant = {0, 0};

    double scaleX = W / 1920.0;
    double scaleY = H / 1080.0;

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

void remplir_scanline(RVB **t, EdgeList *edgeList, RVB couleurRemplissage) {
    int yMin = H - 1, yMax = 0;
    for (int i = 0; i < edgeList->count; ++i) {
        int y0 = (int)round(edgeList->edges[i].y0);
        int y1 = (int)round(edgeList->edges[i].y1);
        if (y0 < yMin) yMin = y0;
        if (y1 < yMin) yMin = y1;
        if (y0 > yMax) yMax = y0;
        if (y1 > yMax) yMax = y1;
    }
    if (yMin < 0) yMin = 0;
    if (yMax >= H) yMax = H - 1;

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
            if (xEnd >= W) xEnd = W - 1;

            for (int x = xStart; x <= xEnd; ++x) {
                if (x >= 0 && x < W && y >= 0 && y < H) {
                    t[y][x] = couleurRemplissage;
                }
            }
        }

        free(intersections);
    }
}

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
            if (i >= 0 && i < H && j >= 0 && j < W) {
                t[i][j] = couleurActuelle;
            }
        }
    }
}

void dessiner_et_remplir_forme(RVB **im, const char *nomFichier, RVB couleurContour, RVB couleurRemplissage, double offsetX, double offsetY) {
    DonneesTrace donneesTrace = lire_svg(nomFichier);

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