#define __USE_XOPEN
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Tree.h"

// verify if the argument file is .csv
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

// compare two double
int compare_double (const void * a, const void * b){
    if (*(double*)a > *(double*)b)
        return 1;
    else if (*(double*)a < *(double*)b)
        return -1;
    else
        return 0;  
}

// compare two ISO 8601 dates
int compare_dates(const void *a, const void *b) {
  const char *date1 = *(const char **)a;
  const char *date2 = *(const char **)b;

  struct tm tm1, tm2;

  strptime(date1, "%Y-%m-%dT%H:%M:%S%z", &tm1);
  strptime(date2, "%Y-%m-%dT%H:%M:%S%z", &tm2);

  time_t t1 = mktime(&tm1);
  time_t t2 = mktime(&tm2);

  return (int)(t1 - t2);
}

// print a table of double
void print_tab(double* tab , int size , FILE* file){
    for (int i = 0 ; i < size ; i++){
        fprintf(file , "%lf\n" , tab[i]);
    }
}

//------------------ MAIN ---------------------//
int main(int argc, char *argv[]){
    
    FILE* input_file = NULL;
    FILE* output_file = NULL;

    p_tree data_tree = NULL;

    char* input_filename = NULL;
    char* output_filename = NULL;
    
    clock_t start;
    clock_t end;
    
    int nmb_data = 100; // actualiser avec le script shell
    int error = 0;

    double data = 0;
    double elapsed = 0.f;
    
    char line[1024];

    //---------- TYPE SORT ----------//
    char* type_sort = NULL;
    char* avl = "--avl";
    char* abr = "--abr";
    char* tab = "--tab";

    int avl_state = 0;
    int abr_state = 0;
    int tab_state = 0;
    //-------------------------------//
    
    //---------- FILE OPTIONS ----------//
    char* f_option = NULL;
    char* o_option = NULL;
    char* r_option = NULL;

    int f_option_state = 0;
    int o_option_state = 0;
    int r_option_state = 0;
    //----------------------------------//
    
    //---------- NO FILES ----------//
    if (argc == 1){
        printf("No input file and no output file\n");
        error = 1;
        exit(error);
    }
    else{

        for (int i = 1; i < argc; i++) {

            if (strcmp(argv[i], "-f") == 0) {
                f_option_state = 1;
                if (i + 1 < argc) {
                    f_option = argv[i + 1];
                }
            }
            if (strcmp(argv[i], "-o") == 0) {
                o_option_state = 1;
                if (i + 1 < argc) {
                    o_option = argv[i + 1];
                }
            }
            if (strcmp(argv[i], "-r") == 0) {
                r_option_state = 1;
                if (i + 1 < argc) {
                    r_option = argv[i + 1];
                }
            }
            if (strcmp(argv[i], avl) == 0) {
                avl_state = 1;
                type_sort = argv[i];
            }
            else if (strcmp(argv[i], abr) == 0) {
                abr_state = 1;
                type_sort = argv[i];
            }
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
                printf("Erreur format : .csv accepte seulement\n");
                error = 2;
                exit(error);
            }

            //---------- NO SORT OPTIONS ----------//

            else if(type_sort == NULL){

                printf("Les fichiers sont des fichiers .csv\n\n");
                printf("%s , %s \n\n", input_filename , output_filename); 

                input_file = fopen(input_filename , "r+");

                if(input_file == NULL){
                    error = 2;
                    exit(error);
                }
                else{

                    start = clock();
                    while (fgets(line, sizeof(line) , input_file) != NULL) {
                        
                        if(strlen(line) > 1){
                            sscanf(line , "%lf" , &data);
                            data_tree = insert_AVL(data_tree , data);
                        }                 
                    }
                    end = clock();

                    fclose(input_file);

                    output_file = fopen(output_filename , "w");

                    if(output_file == NULL){
                        error = 3;
                        exit(error);
                    }
                    else{
                        in_fixe_search(data_tree , output_file);
                        fclose(output_file);

                        elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                        printf("Temps écoulé : %f secondes\n", elapsed);
                    }
                }
            }
        
            //---------- WITH SORT OPTIONS ----------//
            else{

                printf("Les fichiers sont des fichiers .csv\n\n");
                printf("%s , %s \n\n", input_filename , type_sort);
                
                //---------- *--avl OPTION* ----------//
                if (strcmp(type_sort , avl) == 0){

                    input_file = fopen(input_filename , "r+");

                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }
                    else{

                        start = clock();
                        while (fgets(line, sizeof(line) , input_file) != NULL) {

                            if(strlen(line) > 1){
                                sscanf(line , "%lf" , &data);
                                data_tree = insert_AVL(data_tree , data);
                            }
                        }
                        end = clock();

                        fclose(input_file);
                        
                        output_file = fopen(output_filename , "w");

                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        else{
                            in_fixe_search(data_tree , output_file);
                            fclose(output_file);

                            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                            printf("Temps écoulé : %f secondes\n", elapsed);
                        }
                    }
                }

                //---------- *--abr OPTION* ----------//
                else if (strcmp(type_sort , abr) == 0){ 

                    input_file = fopen(input_filename , "r+");

                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }
                    else{
                        start = clock();
                        while (fgets(line, sizeof(line) , input_file) != NULL) {

                            if(strlen(line) > 1){
                                sscanf(line , "%lf" , &data);
                                data_tree = insert_ABR(data_tree , data);
                            }
                        }
                        end = clock();

                        fclose(input_file);
                        
                        output_file = fopen(output_filename , "w");

                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        else{
                            in_fixe_search(data_tree , output_file);
                            fclose(output_file);

                            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                            printf("Temps écoulé : %f secondes\n", elapsed);
                        }
                    }
                }
                
                //---------- *--tab OPTION* ----------//
                else if (strcmp(type_sort , tab) == 0){

                    int i = 0;
                    double* data_tab = malloc(nmb_data*sizeof(double));

                    if(data_tab == NULL){
                        printf("ERROR");
                        error = 4,
                        exit(error);
                    } 

                    input_file = fopen(input_filename , "r+");

                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }
                    else{
                        start = clock();
                        while (fgets(line, sizeof(line) , input_file) != NULL) {

                            if(strlen(line) > 1){
                                sscanf(line , "%lf" , &data);
                                data_tab[i] = data;
                                i++;
                            } 
                        }

                        qsort(data_tab , nmb_data , sizeof(double) , compare_double);
                        end = clock();

                        fclose(input_file);

                        output_file = fopen(output_filename , "w");

                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        else{
                            print_tab(data_tab , nmb_data , output_file);
                            fclose(output_file);

                            elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                            printf("Temps écoulé : %f secondes\n", elapsed);
                        }
                    }
                }

                else{
                    printf("mode de tris inconnu\n");
                    error = 1;
                    exit(error);
                }
            }
        }
    }
    return error;
}
