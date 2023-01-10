#ifndef COORDINATES_H
#define COORDINATES_H

#include <stdlib.h>

typedef struct{

    double x;
    double y;
    
}Coordinates;

Coordinates* create_coordinates(double x , double y);

#endif