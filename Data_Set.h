#ifndef DATA_SET_H
#define DATA_SET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<stdbool.h>
#include <limits.h>

typedef struct{
    
    int id;
    char* date;
    double data;

    double min;
    double max;

    int nmb_data;
    double sum;
    double average;

    double x;
    double y;

    int tab_incr;
    
}Data_Set;

Data_Set* create_set();

Data_Set* create_data_set(int id , char* date , double data , bool data_ok , double min , double max , double x , double y);

Data_Set* update_data_set(Data_Set* data_set , int id , char* date , double data , double min , double max , double x , double y);

void print_data_set_tab(Data_Set** tab_data_set , int size , int r , FILE* output_file);

int compare_max(const void* a , const void* b);

int compare_id(const void* a , const void* b);

Data_Set* update_data_set_id(Data_Set* d , int id);

Data_Set* update_data_set_data(Data_Set* d , double data);

#endif