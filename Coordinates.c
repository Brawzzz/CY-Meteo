#include <stdio.h>
#include "Coordinates.h"

Coordinates* create_coordinates(double x , double y) {

    Coordinates* coordinates = malloc(sizeof(Coordinates));
    if (coordinates == NULL){
        exit(4);
    }

    coordinates -> x = x;
    coordinates -> y = y;

    return coordinates;
}