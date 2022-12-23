#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Tree.h"

// verify if the argument file is .csv
int is_csv(char *s){ 
    char *extension;
    extension = strrchr(s,'.'); 
 
    if ((extension[0] == '.') && (extension[1] == 'c') && (extension[2] == 's') && (extension[3] == 'v')){
        return 1;
    }
    return 0;
}

// compare two float
int compare_float ( const void * first, const void * second ) {
    int first_float = * (const float *) first;
    int second_float = * (const float *) second;
    return (int)(first_float - second_float);
}

// print a table of float
void print_tab(float* tab , int size , FILE* file){
    for (int i = 0 ; i < size ; i++){
        fprintf(file , "%f\n" , tab[i]);
    }
}

//------------------ MAIN ---------------------//
int main(int argc, char *argv[]){
    
    FILE* input_file = NULL;
    FILE* output_file = NULL;

    char* input_filename = NULL;
    char* output_filename = "filtred_data.csv";
    
    clock_t start;
    clock_t end;
    
    int nmb_data = 100; // actualiser avec le script shell
    int error = 0;

    float data = 0;

    double elapsed = 0.f;
    
    char line[1024];

    //---------- TYPE SORT ----------//
    char* type_sort = NULL;
    char* avl = "--avl";
    char* abr = "--abr";
    char* tab = "--tab";
    //-------------------------------//

    //---------- NO FILE ----------//
    if (argc == 1){
        printf("Pas de fichier en argument\n");
        error = 1;
    }
    //---------- NO SORT OPTIONS ----------//
    else if(argc == 2){

        input_filename = argv[1];
        
        if(!(is_csv(input_filename))){
            printf("format de fichier non correct: .csv accepte seulement\n");
            error = 2;
            //exit(EXIT_FAILURE);
        }
        else{

            printf("Le fichier est bien un fichier csv\n\n");
            printf("%s , %s \n\n", input_filename , output_filename);

            p_tree data_tree = NULL; 

            input_file = fopen(input_filename , "r+");

            if(input_file == NULL){
                error = 2;
            }
            else{

                start = clock();
                while (fgets(line, sizeof(line) , input_file) != NULL) {
                    
                    if(strlen(line) > 1){
                        sscanf(line , "%f" , &data);
                        data_tree = insert_AVL(data_tree , data);
                    }                 
                }
                end = clock();

                fclose(input_file);

                output_file = fopen(output_filename , "w");

                if(output_file == NULL){
                    error = 3;
                }
                else{
                    in_fixe_search(data_tree , output_file);
                    fclose(output_file);

                    elapsed = (double)(end - start) / CLOCKS_PER_SEC;
                    printf("Temps écoulé : %f secondes\n", elapsed);
                }
            }
        }
    }
    //---------- WITH SORT OPTIONS ----------//
    else{
        
        input_filename = argv[1];
        type_sort = argv[2];
            
        if(!(is_csv(input_filename))){
            printf("format de fichier non correct: .csv accepte seulement\n");
            error = 2;
        }

        else{
            printf("Le fichier est bien un fichier csv\n\n");

            printf("%s , %s \n\n", input_filename , type_sort);
            
            //---------- *--avl OPTION* ----------//
            if (strcmp(type_sort , avl) == 0){

                p_tree data_tree = NULL; 

                input_file = fopen(input_filename , "r+");

                if(input_file == NULL){
                    error = 2;
                }
                else{

                    start = clock();
                    while (fgets(line, sizeof(line) , input_file) != NULL) {

                        if(strlen(line) > 1){
                            sscanf(line , "%f" , &data);
                            data_tree = insert_AVL(data_tree , data);
                        }
                    }
                    end = clock();

                    fclose(input_file);
                    
                    output_file = fopen(output_filename , "w");

                    if(output_file == NULL){
                        error = 3;
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

                p_tree data_tree = NULL; 

                input_file = fopen(input_filename , "r+");

                if(input_file == NULL){
                    error = 2;
                }
                else{
                    start = clock();
                    while (fgets(line, sizeof(line) , input_file) != NULL) {

                        if(strlen(line) > 1){
                            sscanf(line , "%f" , &data);
                            data_tree = insert_ABR(data_tree , data);
                        }
                    }
                    end = clock();

                    fclose(input_file);
                    
                    output_file = fopen(output_filename , "w");

                    if(output_file == NULL){
                        error = 3;
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
                float* data_tab = malloc(nmb_data*sizeof(float));

                if(data_tab == NULL){
                    printf("ERROR");
                    exit(4);
                } 

                input_file = fopen(input_filename , "r+");

                if(input_file == NULL){
                    error = 2;
                }
                else{
                    start = clock();
                    while (fgets(line, sizeof(line) , input_file) != NULL) {

                        if(strlen(line) > 1){
                            sscanf(line , "%f" , &data);
                            data_tab[i] = data;
                            i++;
                        } 
                    }

                    qsort(data_tab , nmb_data , sizeof(float) , compare_float);
                    end = clock();

                    fclose(input_file);

                    output_file = fopen(output_filename , "w");

                    if(output_file == NULL){
                        error = 3;
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
            }
        }
    }
    return error;
}
