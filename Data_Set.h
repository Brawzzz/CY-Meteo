#ifndef DATA_SET_H
#define DATA_SET_H

#include "Coordinates.h"

typedef struct{
    
    int id;

    char* date;

    double min;
    double max;

    int nmb_data;
    double sum;
    double average;

    Coordinates coordinates;
    
}Data_Set;

Data_Set* create_data_set(int id , char* date , Coordinates* coordinates , double min , double max);

#endif