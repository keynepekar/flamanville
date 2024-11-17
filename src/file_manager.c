#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "structures.h"
#include "file_manager.h"

#define LONGUEUR_LIGNE 256

RVB **allouer_buffer() {
    RVB **im = malloc(H * sizeof(RVB *));
    if (im == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < H; ++i) {
        im[i] = malloc(W * sizeof(RVB));
        if (im[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire.\n");
            for (int j = 0; j < i; ++j) {
                free(im[j]);
            }
            free(im);
            exit(EXIT_FAILURE);
        }
    }
    return im;
}

void liberer_buffer(RVB **im) {
    for (int i = 0; i < H; ++i) {
        free(im[i]);
    }
    free(im);
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