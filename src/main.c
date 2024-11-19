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

    // Soleil
    dessiner_soleil(im, (COORD){825, 450}, 40, 100, (RVB){250, 221, 186});

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

    ecrire_corps(im, df);

    liberer_buffer(im);
    fermer_image(df);

    return 0;
}
