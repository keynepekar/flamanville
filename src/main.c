#include <stdlib.h>
#include "constantes.h"
#include "structures.h"
#include "file_manager.h"
#include "dessin.h"
#include "transformations.h"

int main() {
    RVB **im;
    FILE *df;

    im = allouer_buffer();

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

    // Montagnes
    dessiner_et_remplir_forme(im, "./assets/Montagne3.txt", (RVB){0, 0, 0}, (RVB){125, 175, 179}, 0, 120);
    dessiner_et_remplir_forme(im, "./assets/Montagne2.txt", (RVB){0, 0, 0}, (RVB){99, 159, 167}, 0, 120);
    dessiner_et_remplir_forme(im, "./assets/Montagne1.txt", (RVB){0, 0, 0}, (RVB){67, 114, 119}, 0, 120);

    // Sol
    dessiner_et_remplir_forme(im, "./assets/Sol.txt", (RVB){0, 0, 0}, (RVB){65, 86, 34}, 0, 40);

    // Tente
    dessiner_et_remplir_forme(im, "./assets/Tente1.txt", (RVB){0, 0, 0}, (RVB){179, 112, 21}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente2.txt", (RVB){0, 0, 0}, (RVB){246, 159, 27}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente3.txt", (RVB){0, 0, 0}, (RVB){100, 65, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente4.txt", (RVB){0, 0, 0}, (RVB){100, 65, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Tente5.txt", (RVB){0, 0, 0}, (RVB){246, 159, 27}, 0, 0);

    // Arbre
    dessiner_et_remplir_forme(im, "./assets/Tronc.txt", (RVB){0, 0, 0}, (RVB){97, 43, 18}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille1.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille2.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille3.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille4.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille5.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille6.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille7.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Feuille8.txt", (RVB){0, 0, 0}, (RVB){90, 132, 46}, 0, 0);

    // Table pique-nique
    dessiner_et_remplir_forme(im, "./assets/Barre-ar.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Pied-ar-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Pied-arr-droit.txt", (RVB){0, 0, 0}, (RVB){98, 32, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Pied-av-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/siege-droit-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/siege-droit-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Pied-av-droit.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/barre-av.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Siege-gauche-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Siege-gauche-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout1-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){178, 66, 24}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/table-contour.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/table-dessus.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/table-barre-dessous.txt", (RVB){0, 0, 0}, (RVB){154, 62, 22}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/Bordure-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout2-siege-gauche.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);

    // Buche
    dessiner_et_remplir_forme(im, "./assets/tronc2.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/rainure1.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/rainure2.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/rainure3.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/rainure4.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/rainure5.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout1.txt", (RVB){0, 0, 0}, (RVB){124, 54, 17}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout2.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout3.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout4.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout5.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);
    dessiner_et_remplir_forme(im, "./assets/bout6.txt", (RVB){0, 0, 0}, (RVB){99, 42, 16}, 0, 0);

    ecrire_corps(im, df);

    liberer_buffer(im);
    fermer_image(df);

    return 0;
}
