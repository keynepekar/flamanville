#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "structures.h"

COORD rotation_point(COORD origine, COORD point, double deg_rot);
COORD mise_echelle_point(COORD point, double sx, double sy);
COORD translation_point(COORD point, double dx, double dy);

#endif /* TRANSFORMATIONS_H */