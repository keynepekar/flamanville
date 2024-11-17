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
    ecrire_corps(im, df);

    liberer_buffer(im);
    fermer_image(df);

    return 0;
}
