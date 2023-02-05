#ifndef TOOLS_H
#define TOOLS_H

# include "Data_Set.h"

int is_csv(char *s);

int compare_double (const void * a, const void * b);

int compare_int (const void * a, const void * b);

bool binary_search_date(char* array[] , int size , char* value , int* index);

bool binary_search_id(int array[] , int size , int value , int* index);

bool binary_search_data_set_ID(Data_Set* array[] , int size , int value , int* index);

bool binary_search_data_set_date(Data_Set* array[] , int size , char* value , int* index);

bool binary_search_tab_date(Data_Set** array[] , int size , char* value , int* index);

void set(Data_Set** tab, int i , int j , int k , int nbi, int nbj, int nbk, Data_Set* ptr);

Data_Set* get(Data_Set** tab, int i , int j , int k , int nbi, int nbj, int nbk);

#endif