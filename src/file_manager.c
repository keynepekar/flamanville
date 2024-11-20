#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "structures.h"
#include "file_manager.h"

#define LONGUEUR_LIGNE 256

RVB **allouer_buffer(int largeur, int hauteur) {
    RVB **t = malloc(hauteur * sizeof(RVB *));
    if (t == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le buffer d'image.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < hauteur; ++i) {
        t[i] = malloc(largeur * sizeof(RVB));
        if (t[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour la ligne %d du buffer d'image.\n", i);
            exit(EXIT_FAILURE);
        }
    }
    return t;
}

void liberer_buffer(RVB **t, int hauteur) {
    for (int i = 0; i < hauteur; ++i) {
        free(t[i]);
    }
    free(t);
}

FILE *lire_image(const char *nomfichier) {
    FILE *df = fopen(nomfichier, "w");
    if (df == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }
    return df;
}

void fermer_image(FILE *df) {
    fclose(df);
}

void ecrire_header(FILE *df) {
    fprintf(df, "P3\n%d %d\n%d\n", W, H, P);
}

void ecrire_corps(RVB **t, FILE *df) {
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            fprintf(df, "%d %d %d ", t[i][j].R, t[i][j].V, t[i][j].B);
        }
        fprintf(df, "\n");
    }
}

DonneesTrace lire_svg(const char *nomfichier) {
    FILE *file = fopen(nomfichier, "r");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s.\n", nomfichier);
        exit(EXIT_FAILURE);
    }

    DonneesTrace donneesTrace;
    donneesTrace.segments = NULL;
    donneesTrace.nb = 0;

    char line[LONGUEUR_LIGNE];
    while (fgets(line, LONGUEUR_LIGNE, file) != NULL) {
        // Enlever le saut de ligne
        line[strcspn(line, "\n")] = 0;

        char *token = strtok(line, " ");
        if (token == NULL) continue;

        SegmentsTrace segment;
        if (strcmp(token, "Move") == 0) {
            segment.type = MOVE;
            for (int i = 0; i < 2; ++i) {
                char *coord = strtok(NULL, " ");
                if (coord == NULL) break;
                sscanf(coord, "%lf,%lf", &segment.points[i].x, &segment.points[i].y);
            }
        } else if (strcmp(token, "Line") == 0) {
            segment.type = LINE;
            for (int i = 0; i < 2; ++i) {
                char *coord = strtok(NULL, " ");
                if (coord == NULL) break;
                sscanf(coord, "%lf,%lf", &segment.points[i].x, &segment.points[i].y);
            }
        } else if (strcmp(token, "CubicBezier") == 0) {
            segment.type = CUBIC_BEZIER;
            for (int i = 0; i < 4; ++i) {
                char *coord = strtok(NULL, " ");
                if (coord == NULL) break;
                sscanf(coord, "%lf,%lf", &segment.points[i].x, &segment.points[i].y);
            }
        } else if (strcmp(token, "Close") == 0) {
            segment.type = CLOSE;
            for (int i = 0; i < 2; ++i) {
                char *coord = strtok(NULL, " ");
                if (coord == NULL) break;
                sscanf(coord, "%lf,%lf", &segment.points[i].x, &segment.points[i].y);
            }
        } else {
            // Type inconnu
            continue;
        }

        // Ajouter le segment à donneesTrace
        donneesTrace.nb++;
        donneesTrace.segments = realloc(donneesTrace.segments, donneesTrace.nb * sizeof(SegmentsTrace));
        if (donneesTrace.segments == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour les segments.\n");
            exit(EXIT_FAILURE);
        }
        donneesTrace.segments[donneesTrace.nb - 1] = segment;
    }

    fclose(file);
    return donneesTrace;
}

RVB **redimensionner_image(RVB **imageHD, int largeurHD, int hauteurHD, int largeurBD, int hauteurBD) {
    RVB **imageBD = malloc(hauteurBD * sizeof(RVB *));
    for (int i = 0; i < hauteurBD; ++i) {
        imageBD[i] = malloc(largeurBD * sizeof(RVB));
    }

    double ratioX = (double)largeurHD / largeurBD;
    double ratioY = (double)hauteurHD / hauteurBD;

    for (int yBD = 0; yBD < hauteurBD; ++yBD) {
        for (int xBD = 0; xBD < largeurBD; ++xBD) {
            double xHD = xBD * ratioX;
            double yHD = yBD * ratioY;

            int xHD_int = (int)xHD;
            int yHD_int = (int)yHD;

            double x_diff = xHD - xHD_int;
            double y_diff = yHD - yHD_int;

            RVB A = imageHD[yHD_int][xHD_int];
            RVB B = imageHD[yHD_int][xHD_int + 1 < largeurHD ? xHD_int + 1 : xHD_int];
            RVB C = imageHD[yHD_int + 1 < hauteurHD ? yHD_int + 1 : yHD_int][xHD_int];
            RVB D = imageHD[yHD_int + 1 < hauteurHD ? yHD_int + 1 : yHD_int][xHD_int + 1 < largeurHD ? xHD_int + 1 : xHD_int];

            RVB pixel;
            pixel.R = (int)(
                    A.R * (1 - x_diff) * (1 - y_diff) +
                    B.R * x_diff * (1 - y_diff) +
                    C.R * (1 - x_diff) * y_diff +
                    D.R * x_diff * y_diff
            );
            pixel.V = (int)(
                    A.V * (1 - x_diff) * (1 - y_diff) +
                    B.V * x_diff * (1 - y_diff) +
                    C.V * (1 - x_diff) * y_diff +
                    D.V * x_diff * y_diff
            );
            pixel.B = (int)(
                    A.B * (1 - x_diff) * (1 - y_diff) +
                    B.B * x_diff * (1 - y_diff) +
                    C.B * (1 - x_diff) * y_diff +
                    D.B * x_diff * y_diff
            );

            imageBD[yBD][xBD] = pixel;
        }
    }

    return imageBD;
}