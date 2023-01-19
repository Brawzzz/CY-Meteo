#include "Data_Set.h"

Data_Set* create_set(){

    Data_Set* data_set = (Data_Set*)malloc(sizeof(Data_Set));
    if (data_set == NULL){
        exit(4);
    }

    return data_set;
}

Data_Set* create_data_set(int id , char* date , double data , double d_x , double d_y , double min , double max , double x , double y){

    Data_Set* data_set = (Data_Set*)malloc(sizeof(Data_Set));
    if (data_set == NULL){
        exit(4);
    }

    data_set -> id = id;
    data_set -> date = strdup(date);
    data_set -> data = data;

    data_set -> d_x = d_x;
    data_set -> d_y = d_y;

    data_set -> min = min;
    data_set -> max = max;

    data_set -> nmb_data = 0;
    data_set -> sum = 0;
    data_set -> average = 0;

    data_set -> nmb_data_1 = 0;
    data_set -> sum_1 = 0;
    data_set -> average_1 = 0;

    data_set -> x = x;
    data_set -> y = y; 

    data_set -> tab_incr = 0;

    return data_set;
}

// print a table of Data_Set*
void print_data_set_tab(Data_Set** tab_data_set , int size , int r , FILE* output_file){
    if(r == 0){
        for(int i = 0 ; i < size ; i++){
            if(tab_data_set[i] -> pass == 1){
                fprintf(output_file , "%d,%s,%lf,%lf,%lf,%lf,%lf\n" , tab_data_set[i] -> id , tab_data_set[i] -> date , tab_data_set[i] -> min , tab_data_set[i] -> max , tab_data_set[i] -> average , tab_data_set[i] -> x , tab_data_set[i] -> y);
            }
        }
    }
    else{
        for(int i = size - 1 ; i >= 0 ; i--){
            if(tab_data_set[i] -> pass == 1){
                fprintf(output_file , "%d,%s,%lf,%lf,%lf,%lf,%lf\n" , tab_data_set[i] -> id , tab_data_set[i] -> date , tab_data_set[i] -> min , tab_data_set[i] -> max , tab_data_set[i] -> average , tab_data_set[i] -> x , tab_data_set[i] -> y);
            }
        }
    }
}

// compare two Data_Set's max
int compare_max(const void* a , const void* b){
    if ( (*(Data_Set**)a) -> max > (*(Data_Set**)b) -> max ){
        return 1;
    }
    else if ( (*(Data_Set**)a) -> max < (*(Data_Set**)b) -> max){
        return -1;
    }
    else{
        return 0; 
    }
}

// compare two Data_Set's id
int compare_id(const void* a , const void* b){
    if ( (*(Data_Set**)a) -> id > (*(Data_Set**)b) -> id ){
        return 1;
    }
    else if ( (*(Data_Set**)a) -> id < (*(Data_Set**)b) -> id){
        return -1;
    }
    else{
        return 0; 
    }
}