#include <stdlib.h>
#include <string.h>
#include "constantes.h"
#include "structures.h"
#include "file_manager.h"

#define LONGUEUR_LIGNE 256

/*
 * Alloue dynamiquement un buffer pour une image de dimensions largeur x hauteur.
 * largeur : largeur de l'image (px)
 * hauteur : hauteur de l'image (px)
 */
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

/*
 * Libère la mémoire allouée pour le buffer d'image.
 * hauteur : nombre de lignes du buffer à libérer
 */
void liberer_buffer(RVB **t, int hauteur) {
    for (int i = 0; i < hauteur; ++i) {
        free(t[i]);
    }
    free(t);
}

/*
 * Ouvre un fichier en écriture pour y enregistrer l'image.
 * nomfichier : nom du fichier à ouvrir
 * Retourne un pointeur vers le fichier ouvert.
 */
FILE *lire_image(const char *nomfichier) {
    FILE *df = fopen(nomfichier, "w");
    if (df == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }
    return df;
}

/*
 * Ferme le fichier passé en paramètre.
 */
void fermer_image(FILE *df) {
    fclose(df);
}

/*
 * Écrit l'en-tête du fichier image au format PPM.
 */
void ecrire_header(FILE *df) {
    fprintf(df, "P3\n%d %d\n%d\n", W_FHD, H_FHD, P);
}

/*
 * Écrit le contenu de l'image dans le fichier.
 * t : buffer de l'image
 */
void ecrire_corps(RVB **t, FILE *df) {
    for (int i = 0; i < H_FHD; ++i) {
        for (int j = 0; j < W_FHD; ++j) {
            fprintf(df, "%d %d %d ", t[i][j].R, t[i][j].V, t[i][j].B);
        }
        fprintf(df, "\n");
    }
}

/*
 * Lit un fichier TXT d'un SVG déjà parsé et extrait les données de tracé.
 * nomfichier : nom du fichier TXT à lire
 * Retourne une structure DonneesTrace contenant les segments à tracer.
 */
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

/*
 * Redimensionne une image haute définition en une image basse définition en appliquant un anti-aliasing.
 * Utilise l'interpolation bilinéaire pour le filtrage.
 * largeurFHD, hauteurFHD : dimensions de l'image FHD
 * largeurHD, hauteurHD : dimensions de l'image HD (cible)
 * Retourne le buffer de l'image redimensionnée.
 */
RVB **redimensionner_image(RVB **imageFHD, int largeurFHD, int hauteurFHD, int largeurHD, int hauteurHD) {
    RVB **imageBD = malloc(hauteurHD * sizeof(RVB *));
    for (int i = 0; i < hauteurHD; ++i) {
        imageBD[i] = malloc(largeurHD * sizeof(RVB));
    }

    double ratioX = (double)largeurFHD / largeurHD;
    double ratioY = (double)hauteurFHD / hauteurHD;

    for (int yBD = 0; yBD < hauteurHD; ++yBD) {
        for (int xBD = 0; xBD < largeurHD; ++xBD) {
            double xHD = xBD * ratioX;
            double yHD = yBD * ratioY;

            int xHD_int = (int)xHD;
            int yHD_int = (int)yHD;

            double x_diff = xHD - xHD_int;
            double y_diff = yHD - yHD_int;

            RVB A = imageFHD[yHD_int][xHD_int];
            RVB B = imageFHD[yHD_int][xHD_int + 1 < largeurFHD ? xHD_int + 1 : xHD_int];
            RVB C = imageFHD[yHD_int + 1 < hauteurFHD ? yHD_int + 1 : yHD_int][xHD_int];
            RVB D = imageFHD[yHD_int + 1 < hauteurFHD ? yHD_int + 1 : yHD_int][xHD_int + 1 < largeurFHD ? xHD_int + 1 : xHD_int];

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