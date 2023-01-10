#define __USE_XOPEN
#define _GNU_SOURCE

#define NMB_STATIONS 62
#define NMB_DATES 4656

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include "Tree.h"

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

// compare two dates ISO 8601
int compare_dates(char* date1, char* date2) {

    struct tm tm1, tm2;

    strptime(date1, "%Y-%m-%dT%H:%M:%S%z", &tm1);
    strptime(date2, "%Y-%m-%dT%H:%M:%S%z", &tm2);

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

// convert date to time
void convert_date_to_time(char* date , double* time){

    struct tm tm1;
    strptime(date, "%Y-%m-%dT%H:%M:%S%z", &tm1);
    *time = mktime(&tm1);
}

// print a table of double
void print_tab(double* tab , int size , FILE* file){
    for (int i = 0 ; i < size ; i++){
        fprintf(file , "%lf\n" , tab[i]);
    }
}

// print a table of int
void print_int_tab(int* tab , int size){
    for (int i = 0 ; i < size ; i++){
        printf("%d\n" , tab[i]);
    }
}

// binary search algorithme for ID
bool binary_search_ID(Data_Set* array[] , int size , int value , int* index) {
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

// binary search algorithme for date
bool binary_search_date(Data_Set* array[] , int size , char* value , int* index) {
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

//------------------ MAIN ---------------------//
int main(int argc, char *argv[]){
    
    int error = 0;

    FILE* id_file = NULL;
    FILE* date_file = NULL;
    FILE* input_file = NULL;
    FILE* output_file = NULL;

    p_tree final_tree = NULL;

    char* id_filename = "id.csv";
    char* date_filename = "dates.csv";
    char* input_filename = NULL;;
    char* output_filename = NULL;
    
    time_t start;
    time_t end;

    int ID = 0;
    char complete_date[26];
    double data = 0.f;

    double x = 0;
    double y = 0;

    Coordinates* coordinates = NULL;

    char id_line[1024];
    char date_line[1024];
    char data_line[1024];

    double elapsed = 0.f;

    //---------- TYPE SORT ----------//
    char* type_sort = NULL;
    char* avl = "--avl";
    char* abr = "--abr";
    char* tab = "--tab";

    char* ID_option = "--id";
    char* date_option = "--date";
    char* data_option = "--data";

    char* r_option = "-r";

    int avl_state = 0;
    int abr_state = 0;
    int tab_state = 0;

    int ID_option_state = 0;
    int date_option_state = 0;
    int data_option_state = 0;

    int r_option_state = 0;
    //-------------------------------//
    
    //---------- FILES OPTIONS ----------//
    char* f_option = "-f";
    char* o_option = "-o";

    int f_option_state = 0;
    int o_option_state = 0;
    //-----------------------------------//
    
    //---------- OPERATIONS OPTIONS ----------//
    char* average_option = "--av";
    char* minimum_option = "--min";
    char* maximum_option = "--max";

    int average_option_state = 0;
    int minimum_option_state = 0;
    int maximum_option_state = 0;
    //----------------------------------------//

    //---------- NO FILES ----------//
    if (argc == 1){
        printf("No input file and no output file\n");
        error = 1;
        exit(error);
    }
    //---------- CHECK ARGUMENTS ----------//
    else{

        for (int i = 1 ; i < argc ; i++) {

            // Check for -f option
            if (strcmp(argv[i], "-f") == 0) {
                f_option_state = 1;
                if (i + 1 < argc) {
                    f_option = argv[i + 1];
                }
            }
            // Check for -o option
            if (strcmp(argv[i], "-o") == 0) {
                o_option_state = 1;
                if (i + 1 < argc) {
                    o_option = argv[i + 1];
                }
            }

            // Check for -av option
            if (strcmp(argv[i], average_option) == 0) {
                average_option_state = 1;
            }
            // Check for -min option
            if (strcmp(argv[i], minimum_option) == 0) {
                minimum_option_state = 1;
            }
            // Check for -max option
            if (strcmp(argv[i], maximum_option) == 0) {
                maximum_option_state = 1;
            }
            
            // Check for -r option
            if (strcmp(argv[i], r_option) == 0) {
                r_option_state = 1;
            }

            // Check for --id option
            if (strcmp(argv[i], ID_option) == 0) {
                ID_option_state = 1;
            }
            // Check for --date option
            else if (strcmp(argv[i], date_option) == 0) {
                date_option_state = 1;
            }
            // Check for --data option
            else if (strcmp(argv[i], data_option) == 0) {
                data_option_state = 1;
            }

            // Check for --avl option
            if (strcmp(argv[i], avl) == 0) {
                avl_state = 1;
                type_sort = argv[i];
            }
            // Check for -abr option
            else if (strcmp(argv[i], abr) == 0) {
                abr_state = 1;
                type_sort = argv[i];
            }
            // Check for -tab option
            else if (strcmp(argv[i], tab) == 0) {
                tab_state = 1;
                type_sort = argv[i];
            }
        }

        if (f_option_state == 0){
            printf("No input file options found\n");
            error = 2;
            exit(error);
        }
        if (o_option_state == 0){
            printf("No output file options found\n");
            error = 3;
            exit(error);
        }
        else if(f_option_state == 1 && o_option_state == 1){

            input_filename = f_option;
            output_filename = o_option;

            if(is_csv(input_filename) == 0 || is_csv(output_filename) == 0){
                printf("Format ERROR : .csv accepted only\n");
                error = 2;
                exit(error);
            }

            printf("Right exentension : .csv\n\n");
            printf("%s , %s \n\n", input_filename , output_filename);

            printf("id option : %d ; date option : %d , data option : %d\n", ID_option_state , date_option_state , data_option_state);
            printf("min option : %d ; max option : %d , average option : %d\n\n", minimum_option_state , maximum_option_state , average_option_state);

            //-------------------- NO SORT OPTIONS / *--avl OPTION BY DEFAULT --------------------//
            
            if(type_sort == NULL || avl_state == 1){

                //------------------- SORT BY ID -------------------//
                if(ID_option_state == 1){

                    Data_Set** tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set*));
                    if (tab_data_set == NULL){
                        error = 4;
                        exit(error);
                    }

                    // fill the complete ID tab
                    id_file = fopen(id_filename , "r");

                    if(id_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    int i = 0;
                    int index = 0;

                    start = time(NULL);
                    while (fgets(id_line, sizeof(id_line) , id_file) != NULL) {

                        sscanf(id_line , "%d;%lf,%lf" , &ID , &x , &y);

                        coordinates = create_coordinates(x , y);
                        tab_data_set[i] = create_data_set(ID , "\0" , coordinates , INT_MAX , INT_MIN);

                        i++;
                    }
                    fclose(id_file);
                    
                    i = 0;

                    input_file = fopen(input_filename , "r+");

                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    // -t1 , p1 
                    if(average_option_state == 1 && minimum_option_state == 1 && maximum_option_state == 1){
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf" , &ID , complete_date , &data);

                            if(binary_search_ID(tab_data_set , NMB_STATIONS , ID , &index)){
                                
                                // Compute the min
                                if(tab_data_set[index]->min > data){
                                    tab_data_set[index]->min = data;
                                } 

                                // Compute the max
                                if(tab_data_set[index]->max < data){
                                    tab_data_set[index]->max = data;
                                }

                                // Compute the average
                                tab_data_set[index]->nmb_data += 1;
                                tab_data_set[index]->sum += data; 
                                tab_data_set[index]->average = (tab_data_set[index]->sum) / (tab_data_set[index]->nmb_data);
                            }
                        }
                        fclose(input_file);
                    }
                    
                    // -w
                    else if (average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){

                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf" , &ID , complete_date , &data);

                            if(binary_search_ID(tab_data_set , NMB_STATIONS , ID , &index)){

                                // Compute the average
                                tab_data_set[index]->nmb_data += 1;
                                tab_data_set[index]->sum += data; 
                                tab_data_set[index]->average = (tab_data_set[index]->sum) / (tab_data_set[index]->nmb_data);
                            }
                        }
                        fclose(input_file);
                    }

                    for (int i = 0 ; i < NMB_STATIONS ; i++){
                        final_tree = insert_AVL_by_ID(final_tree , tab_data_set[i]);
                    }
                    end = time(NULL);

                    output_file = fopen(output_filename , "w");

                    if(r_option_state == 1){
                        reverse_in_fixe_search(final_tree , output_file);
                    }
                    else{
                        in_fixe_search(final_tree , output_file);
                    }
                  
                    fclose(output_file);

                    free(final_tree);
                    final_tree = NULL;
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);  
                }

                //------------------- SORT BY DATE -------------------// 
                else if(date_option_state == 1){

                    Data_Set** tab_data_set = malloc(NMB_DATES * sizeof(Data_Set*));
                    if (tab_data_set == NULL){
                        error = 4;
                        exit(error);
                    }

                    start = time(NULL);

                    // fill the complete date tab
                    date_file = fopen(date_filename , "r");
                    if(date_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    int i = 0;
                    while (fgets(date_line , sizeof(date_line) , date_file) != NULL) {

                        sscanf(date_line , "%s;%lf,%lf" , complete_date , &x , &y);

                        coordinates = create_coordinates(x , y);
                        tab_data_set[i] = create_data_set(0 , complete_date , coordinates , 0 , 0);

                        i++;
                    }
                    fclose(date_file);

                    input_file = fopen(input_filename , "r");
                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    // -t2 , p2 
                    int index = 0;
                    char* date;
                    char* rest;
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf" , &ID , complete_date , &data);
                            
                            date = strtok(complete_date , "T");
                            rest = strtok(NULL , "T");

                            if(binary_search_date(tab_data_set , NMB_DATES , date , &index)){

                                // Compute the average
                                tab_data_set[index] -> nmb_data += 1;
                                tab_data_set[index] -> sum += data; 
                                tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);
                            }
                        }
                        fclose(input_file);
                    }

                    for (int i = 0 ; i < NMB_DATES ; i++){
                        final_tree = insert_AVL_by_date(final_tree , tab_data_set[i]);
                    }
                    end = time(NULL);

                    output_file = fopen(output_filename , "w");
                    if(output_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    if(r_option_state == 1){
                        reverse_in_fixe_search(final_tree , output_file);
                    }
                    else{
                        in_fixe_search(final_tree , output_file);
                    }
                  
                    fclose(output_file);

                    free(final_tree);
                    final_tree = NULL;
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed); 
                }

            /*    
                //------------------- SORT BY DATA -------------------//
                else if(data_option_state == 1){

                    Data_Set* tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set));
                    if (tab_data_set == NULL){
                        error = 4;
                        exit(error);
                    }

                    // fill the complete ID tab
                    id_file = fopen(id_filename , "r");

                    if(id_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    int i = 0;
                    int index = 0;

                    start = time(NULL);
                    while (fgets(data_line, sizeof(data_line) , id_file) != NULL) {

                        sscanf(data_line , "%d" , &ID);

                        tab_data_set[i] = create_data_set(ID , "\0" , INT_MAX , INT_MIN);

                        i++;
                    }
                    fclose(id_file);
                    
                    i = 0;

                    input_file = fopen(input_filename , "r+");

                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    // -h  , -m
                    if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 1){
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf" , &ID , date , &data);

                            if(id_binary_search(tab_data_set , NMB_STATIONS , ID , &index)){
                                
                                // Compute the min
                                if(tab_data_set[index].min > data){
                                    tab_data_set[index].min = data;
                                } 

                                // Compute the max
                                if(tab_data_set[index].max < data){
                                    tab_data_set[index].max = data;
                                }

                                // Compute the average
                                tab_data_set[index].nmb_data += 1;
                                tab_data_set[index].sum += data; 
                                tab_data_set[index].average = (tab_data_set[index].sum) / (tab_data_set[index].nmb_data);
                            }
                        }
                        fclose(input_file);
                    }

                    for (int i = 0 ; i < NMB_STATIONS ; i++){
                        // printf("%lf\n" ,tab_data_set[i].average);
                        final_tree = insert_AVL_by_ID(final_tree , tab_data_set[i]);
                    }
                    end = time(NULL);
                    
                    // print_AVL(final_tree , 0);

                    output_file = fopen(output_filename , "w");

                    if(r_option_state == 1){
                        reverse_in_fixe_search(final_tree , output_file);
                    }
                    else{
                        in_fixe_search(final_tree , output_file);
                    }
                  
                    fclose(output_file);

                    free(final_tree);
                    final_tree = NULL;
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);  
                }
                */
            }

            //-------------------- *--abr OPTIONS --------------------//

            //-------------------- *--tab OPTIONS --------------------//
        }
    }
    return error;
}
