#ifndef DATA_SET_H
#define DATA_SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<stdbool.h>
#include <time.h>
#include <limits.h>

typedef struct{
    
    int id;
    char* date;
    double data;

    double x;
    double y;

    double d_x;
    double d_y;

    double min;
    double max;

    int nmb_data;
    double sum;
    double average;

    int nmb_data_1;
    double sum_1;
    double average_1;

    int tab_incr;
    int pass;
    
}Data_Set;

Data_Set* create_set();

Data_Set* create_data_set(int id , char* date , double data , double d_x , double d_y , double min , double max , double x , double y);

void print_data_set_tab(Data_Set** tab_data_set , int size , int r , FILE* output_file);

int compare_max(const void* a , const void* b);

int compare_id(const void* a , const void* b);

#endif