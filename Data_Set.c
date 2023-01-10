#include "Data_Set.h"

Data_Set* create_data_set(int id , char* date , Coordinates* coordinates , double min , double max){

    Data_Set* data_set = (Data_Set*)malloc(sizeof(Data_Set));

    data_set -> id = id;

    data_set -> date = strdup(date);

    data_set -> min = min;
    data_set -> max = max;

    data_set -> nmb_data = 0;
    data_set -> sum = 0;
    data_set -> average = 0;

    data_set -> coordinates = *coordinates;  

    return data_set;
}