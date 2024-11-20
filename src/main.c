#include <stdlib.h>
#include "constantes.h"
#include "structures.h"
#include "file_manager.h"
#include "dessin.h"
#include "transformations.h"

int main() {
    RVB **im;
    FILE *df;

    int largeurHD = 1920;
    int hauteurHD = 1080;
    int largeurBD = 1280;
    int hauteurBD = 720;

    im = allouer_buffer(largeurHD, hauteurHD);

    df = lire_image("flamanville.ppm");
    ecrire_header(df);

    // Fond blanc
    fond(im, (RVB){255, 255, 255});

    // Ciel
    COORD p1 = {0, 0};
    COORD p2 = {W - 1, H / 2};
    RVB couleurHaut = {39, 158, 184};
    RVB couleurBas = {253, 208, 134};
    fond_degrade_rectangle(im, p1, p2, couleurHaut, couleurBas);

    // Soleil
    dessiner_soleil(im, (COORD){825, 450}, 40, 100, (RVB){250, 221, 186});

    // Sol
    fond_degrade_rectangle(im, (COORD){0, 700}, (COORD){W - 1, 975}, (RVB){185, 190, 154}, (RVB){120, 194, 204});
    dessiner_et_remplir_forme(im, "./assets/Sol.txt", (RVB){65, 86, 34}, (RVB){82, 122, 45}, 0, 40);
    dessiner_et_remplir_forme(im, "./assets/Terre.txt", (RVB){143, 59, 20}, (RVB){143, 59, 20}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Pelouse.txt", (RVB){95, 137, 46}, (RVB){95, 137, 46}, 0, 20);

    // Montagnes
    dessiner_et_remplir_forme(im, "./assets/Montagne3.txt", (RVB){125, 175, 179}, (RVB){125, 175, 179}, 0, 150);
    dessiner_et_remplir_forme(im, "./assets/Montagne2.txt", (RVB){99, 159, 167}, (RVB){99, 159, 167}, -1, 150);
    dessiner_et_remplir_forme(im, "./assets/Montagne1.txt", (RVB){67, 114, 119}, (RVB){67, 114, 119}, 0, 150);

    // Nuages
    dessiner_et_remplir_forme(im, "./assets/Nuage-Long1.txt", (RVB){177, 189, 153}, (RVB){177, 189, 153}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Long2.txt", (RVB){198, 195, 148}, (RVB){198, 195, 148}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond1.1.txt", (RVB){253, 208, 138}, (RVB){253, 208, 138}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond1.2.txt", (RVB){242, 191, 125}, (RVB){242, 191, 125}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond2.1.txt", (RVB){253, 208, 138}, (RVB){253, 208, 138}, 0, 275);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond2.2.txt", (RVB){242, 191, 125}, (RVB){242, 191, 125}, 0, 275);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond3.1.txt", (RVB){253, 208, 138}, (RVB){253, 208, 138}, 0, 175);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond3.2.txt", (RVB){242, 191, 125}, (RVB){242, 191, 125}, 0, 216);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond4.1.txt", (RVB){253, 208, 138}, (RVB){253, 208, 138}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Nuage-Rond4.2.txt", (RVB){242, 191, 125}, (RVB){242, 191, 125}, 0, 0);

    // Tente
    dessiner_et_remplir_forme(im, "./assets/Tente1.txt", (RVB){0, 0, 0}, (RVB){179, 112, 21}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente2.txt", (RVB){0, 0, 0}, (RVB){246, 159, 27}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente3.txt", (RVB){0, 0, 0}, (RVB){100, 65, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente4.txt", (RVB){0, 0, 0}, (RVB){100, 65, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente5.txt", (RVB){0, 0, 0}, (RVB){246, 159, 27}, 0, 0);

    // Arbre 1
    dessiner_et_remplir_forme(im, "./assets/Tronc.txt", (RVB){0, 0, 0}, (RVB){97, 43, 18}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille1.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille2.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille3.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille4.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille5.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille6.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille7.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille8.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, 0, 0);

    // Arbre 2
    dessiner_et_remplir_forme(im, "./assets/Tronc.txt", (RVB){0, 0, 0}, (RVB){97, 43, 18}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille1.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille2.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille3.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille4.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille5.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille6.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille7.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);
    dessiner_et_remplir_forme(im, "./assets/Feuille8.txt", (RVB){0, 0, 0}, (RVB){64, 91, 37}, -1575, -80);

    // Table pique-nique
    dessiner_et_remplir_forme(im, "./assets/Barre-ar.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Pied-ar-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Pied-arr-droit.txt", (RVB){0, 0, 0}, (RVB){98, 32, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Pied-av-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/siege-droit-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/siege-droit-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Pied-av-droit.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/barre-av.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Siege-gauche-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Siege-gauche-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/bout1-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){178, 66, 24}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/table-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/table-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/table-barre-dessous.txt", (RVB){0, 0, 0}, (RVB){154, 62, 22}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/Bordure-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);
    dessiner_et_remplir_forme(im, "./assets/bout2-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 20);

    // Flamanville
    dessiner_et_remplir_forme(im, "./assets/Flamanville1.txt", (RVB){0, 0, 0}, (RVB){164, 36, 52}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville2.txt", (RVB){0, 0, 0}, (RVB){202, 67, 101}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville3.txt", (RVB){0, 0, 0}, (RVB){202, 67, 101}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville4.txt", (RVB){0, 0, 0}, (RVB){175, 39, 66}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville5.txt", (RVB){0, 0, 0}, (RVB){26, 25, 26}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville6.txt", (RVB){0, 0, 0}, (RVB){193, 163, 175}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville7.txt", (RVB){0, 0, 0}, (RVB){199, 56, 84}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville8.txt", (RVB){0, 0, 0}, (RVB){193, 180, 186}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville9.txt", (RVB){0, 0, 0}, (RVB){11, 12, 12}, 0, 25);
    dessiner_et_remplir_forme(im, "./assets/Flamanville10.txt", (RVB){0, 0, 0}, (RVB){193, 180, 185}, 0, 25);

    ecrire_corps(im, df);

    // Anti-aliasing
    RVB **imBD = redimensionner_image(im, largeurHD, hauteurHD, largeurBD, hauteurBD);
    df = fopen("flamanville.ppm", "w");
    if (df == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier flamanville.ppm.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(df, "P3\n%d %d\n%d\n", largeurBD, hauteurBD, P);

    for (int i = 0; i < hauteurBD; ++i) {
        for (int j = 0; j < largeurBD; ++j) {
            fprintf(df, "%d %d %d ", imBD[i][j].R, imBD[i][j].V, imBD[i][j].B);
        }
        fprintf(df, "\n");
    }

    fclose(df);

    // Libérer la mémoire
    liberer_buffer(im, hauteurHD);
    liberer_buffer(imBD, hauteurBD);

    return 0;
}
