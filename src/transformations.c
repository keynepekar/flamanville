#include <math.h>
#include "transformations.h"

/*
 * Effectue une rotation d'un point autour d'une origine donnée.
 * origine : centre de rotation
 * point : point à faire tourner
 * deg_rot : angle de rotation en degrés
 * Retourne le nouveau point après rotation.
 */
COORD rotation_point(COORD origine, COORD point, double deg_rot) {
    double rad = deg_rot * M_PI / 180.0;
    COORD nv_pt;
    nv_pt.x = (int)(cos(rad) * (point.x - origine.x) - sin(rad) * (point.y - origine.y) + origine.x);
    nv_pt.y = (int)(sin(rad) * (point.x - origine.x) + cos(rad) * (point.y - origine.y) + origine.y);
    return nv_pt;
}

/*
 * Applique une mise à l'échelle à un point.
 * sx, sy : facteurs d'échelle selon les axes X et Y
 * Retourne le point après mise à l'échelle.
 */
COORD mise_echelle_point(COORD point, double sx, double sy) {
    COORD nv_pt;
    nv_pt.x = point.x * sx;
    nv_pt.y = point.y * sy;
    return nv_pt;
}

/*
 * Applique une translation à un point.
 * dx, dy : décalages selon les axes X et Y
 * Retourne le point après translation.
 */
COORD translation_point(COORD point, double dx, double dy) {
    COORD nv_pt;
    nv_pt.x = point.x + dx;
    nv_pt.y = point.y + dy;
    return nv_pt;
}