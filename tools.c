#include "tools.h"

// verify if the argument file has .csv extension
int is_csv(char *s){ 
    char *extension;
    extension = strrchr(s,'.'); 

    if (extension == NULL){
        return 0;
    }
    
    if ((extension[0] == '.') && (extension[1] == 'c') && (extension[2] == 's') && (extension[3] == 'v')){
        return 1;
    }
}

// compare two doubles
int compare_double (const void * a, const void * b){
    if (*(double*)a > *(double*)b)
        return 1;
    else if (*(double*)a < *(double*)b)
        return -1;
    else
        return 0;  
}

// compare two int
int compare_int (const void * a, const void * b){
    if (*(int*)a > *(int*)b)
        return 1;
    else if (*(int*)a < *(int*)b)
        return -1;
    else
        return 0;  
}

// binary search algorithme for ID
bool binary_search_data_set_ID(Data_Set* array[] , int size , int value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (array[*index] -> id == value){
            return true;
        }
        else if (array[*index] -> id < value){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// binary search algorithme for date in a tab of Data_Set*
bool binary_search_data_set_date(Data_Set* array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp(array[*index] -> date , value) == 0){
            return true;
        }
        else if (strcmp(array[*index] -> date , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// binary search algorithme for date in a tab of tab of Data_Set*
bool binary_search_tab_date(Data_Set** array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp(array[*index][0] -> date , value) == 0){
            return true;
        }
        else if (strcmp(array[*index][0] -> date , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// 
bool binary_search_hour(Data_Set*** array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp(array[*index][0][0] -> date , value) == 0){
            return true;
        }
        else if (strcmp(array[*index][0][0] -> date , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// binary search algorithme for id in a tab of int
bool binary_search_id(int array[] , int size , int value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (array[*index] == value){
            return true;
        }
        else if (array[*index] < value){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// binary search algorithme for date in a tab of char*
bool binary_search_date(char* array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp(array[*index] , value) == 0){
            return true;
        }
        else if (strcmp(array[*index] , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

void set(PData_Set* tab, int i , int j , int k , int nbi, int nbj, int nbk, Data_Set* ptr) {
    if ( ( ( i >= 0 ) && ( i < nbi ) ) && ( ( j >= 0 ) && ( j < nbj ) ) && ( ( k >= 0 ) && ( k < nbk ) ) ) {
        tab[i + j*nbi + k*nbj*nbi] = ptr;
    }
    else {
        fprintf(stderr,"(set)#ERROR : index out of range\n");
        fflush(stderr);
    }
}

Data_Set* get(PData_Set* tab, int i , int j , int k , int nbi, int nbj, int nbk) {
    Data_Set* res = NULL;
    if ( ( ( i >= 0 ) && ( i < nbi ) ) && ( ( j >= 0 ) && ( j < nbj ) ) && ( ( k >= 0 ) && ( k < nbk ) ) ) {
        //fprintf(stderr,"(get)OK : %d,%d,%d\n",i,j,k);
        //fflush(stderr);
        res = tab[i + j*nbi + k*nbj*nbi];
    }
    else {
        fprintf(stderr,"(get)#ERROR : index out of range : %d,%d,%d\n",i,j,k);
        fflush(stderr);
    }
    return res;
}
    
