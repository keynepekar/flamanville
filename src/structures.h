#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct {
    double x, y;
} COORD;

typedef struct {
    int R, V, B;
} RVB;

typedef enum {
    MOVE,
    LINE,
    CUBIC_BEZIER,
    CLOSE
} TypeSegment;

typedef struct {
    TypeSegment type;
    COORD points[4];
} SegmentsTrace;

typedef struct {
    SegmentsTrace *segments;
    int nb;
} DonneesTrace;

typedef struct {
    double x0, y0;
    double x1, y1;
} Edge;

typedef struct {
    Edge *edges;
    int count;
    int capacity;
} EdgeList;

#endif /* STRUCTURES_H */
