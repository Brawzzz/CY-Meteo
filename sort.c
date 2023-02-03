#define NMB_STATIONS 62
#define NMB_DATES 4656
#define NMB_HOURS 24
#define M_PI 3.14159265358979323846

#include "Tree.h"
#include <math.h>

//------------------------------------ MAIN ---------------------------------------//
int main(int argc, char *argv[]){
    
    int error = 0;

    FILE* id_file = NULL;
    FILE* date_file = NULL;
    FILE* input_file = NULL;
    FILE* output_file = NULL;

    Data_Set* data_set = NULL;
    Data_Set** tab_data_set = NULL;

    p_tree* tree_tab = NULL;
    p_tree data_set_tree = NULL;
    p_tree id_tree = NULL;

    char* id_filename = "id_coord.csv";
    char* date_filename = "dates.csv";
    char* input_filename = NULL;;
    char* output_filename = NULL;
    
    time_t start;
    time_t end;

    int ID = 0;
    char complete_date[26];
    char date[26];
    double data = 0.f;

    double x = 0.f;
    double y = 0.f;

    double dx = 0.f;
    double dy = 0.f;

    double wind_dir = 0.f;
    double wind_speed = 0.f;

    char id_line[1024];
    char date_line[1024];
    char data_line[1024];

    char** dates = NULL;
    char* dates_i = NULL;

    char* hours = NULL;

    int* id_s = NULL;

    double elapsed = 0.f;

    int nodes = 0;
    int mode = 0;
    int i = 0;
    int index = 0;
    int balance = 0;
    int count = 0;

    //---------------------------- TYPE SORT ----------------------------//
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
    //-------------------------------------------------------------------//
    
    //---------------------------- FILES OPTIONS ----------------------------//
    char* f_option = "-f";
    char* o_option = "-o";

    int f_option_state = 0;
    int o_option_state = 0;
    //-----------------------------------------------------------------------//
    
    //---------------------------- OPERATIONS OPTIONS ----------------------------//
    char* average_option = "--av";
    char* minimum_option = "--min";
    char* maximum_option = "--max";

    int average_option_state = 0;
    int minimum_option_state = 0;
    int maximum_option_state = 0;
    //----------------------------------------------------------------------------//

    //---------------------------- NO FILES ----------------------------//
    if (argc == 1){
        printf("No input file and no output file\n");
        error = 1;
        exit(error);
    }
    //---------------------------- CHECK ARGUMENTS ----------------------------//
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

        //---------------------------- CHECK MANDATORY ARGUMENTS ----------------------------//
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

            printf("Correct exentension : .csv\n\n");
            printf("%s , %s \n\n", input_filename , output_filename);

            printf("reverse option : %d\n" , r_option_state);

            printf("id option : %d ; date option : %d , data option : %d\n", ID_option_state , date_option_state , data_option_state);
            printf("min option : %d ; max option : %d , average option : %d\n\n", minimum_option_state , maximum_option_state , average_option_state);

            printf("Sorting in progress...\n");
            //-------------------------------------- NO SORT OPTIONS / *--avl BY DEFAULT --------------------------------------//
            
            if(type_sort == NULL || avl_state == 1){

                //------------------------------------- SORT BY ID -------------------------------------//
                if(ID_option_state == 1){

                    //--------------------------- *-t1/*-p1 | min : 1 , max : 1 , average : 1 ---------------------------// 
                    if(average_option_state == 1 && minimum_option_state == 1 && maximum_option_state == 1){
                        
                        mode = 1;
                        start = time(NULL);
                        
                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(ID , "\0" , data , 0 , 0 , INT_MAX , INT_MIN , x , y);
                            data_set_tree = insert_AVL_by_ID(data_set_tree , data_set , &balance , mode);
                        }
                        fclose(input_file);
                        end = time(NULL);

                        mode = 0;

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);
                        printf("DONE\n");

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }

                    //--------------------------- *-w | min : 0 , max : 0 , average : 1 ---------------------------// 
                    else if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        mode = 2;
                        start = time(NULL);
                        
                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf;%lf;%lf,%lf" , &ID , complete_date , &wind_dir , & wind_speed , &x , &y);

                            dx = wind_speed * cos((wind_dir * M_PI) / 180);
                            dy = wind_speed * sin((wind_dir * M_PI) / 180);

                            data_set = create_data_set(ID , "\0" , data , dx , dy , 0 , 0 , x , y);
                            data_set_tree = insert_AVL_by_ID(data_set_tree , data_set , &balance , mode);
                        }
                        fclose(input_file);
                        end = time(NULL);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);
                        printf("DONE\n");

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }
                    
                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);  
                }

                //------------------------------------- SORT BY DATE -------------------------------------// 
                else if(date_option_state == 1){

                    //--------------------------- *-t2/*-p2 | min : 0 , max : 0 , average : 1 ---------------------------//
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        mode = 0;
                        start = time(NULL); 

                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(0 , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                            data_set_tree = insert_AVL_by_date(data_set_tree , data_set , mode , &balance);
                        }
                        end = time(NULL);
                        fclose(input_file);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);                        
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);                         
                        }
                        fclose(output_file);
                        free(data_set_tree);

                        printf("DONE\n");
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){

                        mode = 1;
                        int index_2 = 0;
                        start = time(NULL);

                        // tab of all single hours
                        char* hours[NMB_HOURS] = {"00" , "01" , "02" , "03" , "04" , "05" , 
                                           "06" , "07" , "08" , "09" , "10" , "11" , 
                                           "12" , "13" , "14" , "15" , "16" , "17" , 
                                           "18" , "19" , "20" , "21" , "22" , "23" } ;

                        // tab of id allocation
                        id_s = (int*)malloc(NMB_STATIONS * sizeof(int));
                        if (id_s == NULL){
                            error = 4;
                            exit(error);
                        }
                        // fill the is tab with alll the single id
                        id_file = fopen(id_filename , "r");
                        if (id_file == NULL){
                            error = 4;
                            exit(error);
                        }
                        while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                            sscanf(id_line , "%d" , &id_s[i]);
                            i++;
                        }
                        fclose(id_file);
                        
                        // Allocates a tab of tab of temporary trees
                        p_tree** tmp_trees = malloc(NMB_HOURS * sizeof(p_tree*));
                        if (tmp_trees == NULL){
                            error = 4;
                            exit(error);
                        }

                        for (int j = 0 ; j < NMB_HOURS ; j++){
                            // allocates a tab of p_tree
                            tree_tab = malloc(NMB_STATIONS * sizeof(p_tree));
                            if (tree_tab == NULL){
                                error = 4;
                                exit(error);
                            }
                            for (int i = 0 ; i < NMB_STATIONS ; i++){
                                data_set = create_data_set(id_s[i] , "\0" , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                                data_set -> hour = hours[i]; 

                                tree_tab[i] = create_tree(data_set); 
                            }
                            tmp_trees[j] = tree_tab;
                        }
                        
                        // start reading the data file and update tmp_trees
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(ID , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                    
                            // resarch complete_date in the tab of p_tree 
                            if(binary_search_tree_hour(tmp_trees , 24 , data_set -> hour , &index)){
                                if(binary_search_tree_id(tmp_trees[index] , NMB_STATIONS , ID , &index_2)){
                                    tmp_trees[index][index_2] = insert_AVL_by_date(tmp_trees[index][index_2] , data_set , mode , &balance);
                                } 
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);
                        
                        mode = 0;

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            for (int i = 1 ; i < NMB_HOURS ; i++){
                                for (int j = 0 ; j < NMB_STATIONS ; j++){
                                    reverse_in_fixe_search(tmp_trees[i][j] , output_file , mode , &count);  
                                    fprintf(output_file , "\n"); 
                                    count += 1;   
                                }                  
                            }
                        }
                        else{
                            for (int i = 1 ; i < NMB_HOURS ; i++){
                                for (int j = 0 ; j < NMB_STATIONS ; j++){
                                    in_fixe_search(tmp_trees[i][j] , output_file , mode , &count);  
                                    fprintf(output_file , "\n"); 
                                    count += 1;   
                                }                  
                            }  
                        }
                        fclose(output_file);
                        free(tree_tab);
                        printf("DONE\n");
                    }

                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);
                }
  
                //------------------------------------- SORT BY DATA -------------------------------------//
                else if(data_option_state == 1){

                    input_file = fopen(input_filename , "r+");
                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    //--------------------------- *-h/*-m | min : 0 , max : 1 , average : 0 ---------------------------//
                    start = time(NULL);
                    if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 1){

                        mode = 0;

                        // start reading the data file
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf;" , &ID , complete_date ,  &x , &y , &data);
                            data_set = create_data_set(ID , "\0" , data , 0 , 0 , 0 , INT_MIN , x , y);

                            // insert data_set by id in order to have the max for each station
                            id_tree = insert_AVL_by_ID(id_tree , data_set , &balance , mode);
                        }
                        fclose(input_file);
                    }
                    // make a new tree sorted by max with id_tree
                    insert_AVL_by_max_from_id(id_tree , &data_set_tree);
                    end = time(NULL);

                    free(id_tree);
                    id_tree = NULL;

                    mode = 1;

                    // write the result in a file
                    printf("Writing the sorting result in a file...");
                    output_file = fopen(output_filename , "w");
                    if(output_file == NULL){
                        error = 3;
                        exit(error);
                    }
                    if(r_option_state == 1){
                        reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                    else{
                        in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                    fclose(output_file);

                    free(data_set_tree);
                    data_set_tree = NULL;
                    printf("DONE\n");
                
                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);
                }
            }

            //-------------------------------------- *--abr OPTION --------------------------------------//
            else if(strcmp(type_sort , abr) == 0){

                if(ID_option_state == 1){

                    //--------------------------- *-t1/*-p1 | min : 1 , max : 1 , average : 1 ---------------------------// 
                    if(average_option_state == 1 && minimum_option_state == 1 && maximum_option_state == 1){
                        
                        mode = 1;
                        start = time(NULL);
                        
                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line , sizeof(data_line) , input_file) != NULL) {
                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);

                            data_set = create_data_set(ID , "\0" , data , 0 , 0 , INT_MAX , INT_MIN , x , y);
                            data_set_tree = insert_ABR_by_ID(data_set_tree , data_set , mode);
                        }
                        fclose(input_file);
                        end = time(NULL);

                        mode = 0;

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);
                        printf("DONE\n");

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }

                    //--------------------------- *-w | min : 0 , max : 0 , average : 1 ---------------------------// 
                    else if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        mode = 2;
                        start = time(NULL);
                        
                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf;%lf;%lf,%lf" , &ID , complete_date , &wind_dir , & wind_speed , &x , &y);

                            dx = wind_speed * cos((wind_dir * M_PI) / 180);
                            dy = wind_speed * sin((wind_dir * M_PI) / 180);

                            data_set = create_data_set(ID , "\0" , data , dx , dy , 0 , 0 , x , y);
                            data_set_tree = insert_ABR_by_ID(data_set_tree , data_set , mode);
                        }
                        fclose(input_file);

                        end = time(NULL);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);
                        printf("DONE\n");

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }
                    
                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);  
                }

                //------------------------------------- SORT BY DATE -------------------------------------// 
                else if(date_option_state == 1){

                    //--------------------------- *-t2/*-p2 | min : 0 , max : 0 , average : 1 ---------------------------//
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        mode = 0;
                        start = time(NULL); 

                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(0 , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                            data_set_tree = insert_ABR_by_date(data_set_tree , data_set , mode);
                        }
                        end = time(NULL);
                        fclose(input_file);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode , &count);                        
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);                         
                        }
                        fclose(output_file);
                        free(data_set_tree);
                        printf("DONE\n");
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){

                        mode = 1;
                        int index_2 = 0;
                        start = time(NULL);

                        // tab of all single hours
                        char* hours[NMB_HOURS] = {"00" , "01" , "02" , "03" , "04" , "05" , 
                                           "06" , "07" , "08" , "09" , "10" , "11" , 
                                           "12" , "13" , "14" , "15" , "16" , "17" , 
                                           "18" , "19" , "20" , "21" , "22" , "23" } ;

                        // tab of id allocation
                        id_s = (int*)malloc(NMB_STATIONS * sizeof(int));
                        if (id_s == NULL){
                            error = 4;
                            exit(error);
                        }
                        // fill the is tab with alll the single id
                        id_file = fopen(id_filename , "r");
                        if (id_file == NULL){
                            error = 4;
                            exit(error);
                        }
                        while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                            sscanf(id_line , "%d" , &id_s[i]);
                            i++;
                        }
                        fclose(id_file);
                        
                        // Allocates a tab of temporary trees
                        p_tree** tmp_trees = malloc(NMB_HOURS * sizeof(p_tree*));
                        if (tmp_trees == NULL){
                            error = 4;
                            exit(error);
                        }

                        for (int j = 0 ; j < NMB_HOURS ; j++){
                            // allocates a tab of p_tree
                            tree_tab = malloc(NMB_STATIONS * sizeof(p_tree));
                            if (tree_tab == NULL){
                                error = 4;
                                exit(error);
                            }
                            for (int i = 0 ; i < NMB_STATIONS ; i++){
                                data_set = create_data_set(id_s[i] , "\0" , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                                data_set -> hour = hours[i]; 

                                tree_tab[i] = create_tree(data_set); 
                            }
                            tmp_trees[j] = tree_tab;
                        }
                        
                        // start reading the data file and update tmp_trees
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(ID , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                    
                            // resarch complete_date in the tab of p_tree 
                            if(binary_search_tree_hour(tmp_trees , 24 , data_set -> hour , &index)){
                                if(binary_search_tree_id(tmp_trees[index] , NMB_STATIONS , ID , &index_2)){
                                    tmp_trees[index][index_2] = insert_ABR_by_date(tmp_trees[index][index_2] , data_set , mode);
                                } 
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);
                        
                        mode = 0;

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            for (int i = 1 ; i < NMB_HOURS ; i++){
                                for (int j = 0 ; j < NMB_STATIONS ; j++){
                                    reverse_in_fixe_search(tmp_trees[i][j] , output_file , mode , &count);  
                                    fprintf(output_file , "\n"); 
                                    count += 1;   
                                }                  
                            }
                        }
                        else{
                            for (int i = 1 ; i < NMB_HOURS ; i++){
                                for (int j = 0 ; j < NMB_STATIONS ; j++){
                                    in_fixe_search(tmp_trees[i][j] , output_file , mode , &count);  
                                    fprintf(output_file , "\n"); 
                                    count += 1;   
                                }                  
                            }  
                        }
                        fclose(output_file);
                        free(tree_tab);
                        printf("DONE\n");
                    }

                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);
                }
  
                //------------------------------------- SORT BY DATA -------------------------------------//
                else if(data_option_state == 1){

                    input_file = fopen(input_filename , "r+");
                    if(input_file == NULL){
                        error = 2;
                        exit(error);
                    }

                    //--------------------------- *-h/*-m | min : 0 , max : 1 , average : 0 ---------------------------//
                    start = time(NULL);
                    if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 1){

                        mode = 0;

                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {
                            
                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf;" , &ID , complete_date ,  &x , &y , &data);
                            data_set = create_data_set(ID , "\0" , data , 0 , 0 , 0 , INT_MIN , x , y);
                            id_tree = insert_ABR_by_ID(id_tree , data_set , mode);
                        }
                        fclose(input_file);
                    }
                    insert_ABR_by_max_from_id(id_tree , &data_set_tree);
                    end = time(NULL);

                    free(id_tree);
                    id_tree = NULL;

                    mode = 1;

                    // write the result in a file
                    printf("Writing the sorting result in a file...");
                    output_file = fopen(output_filename , "w");
                    if(output_file == NULL){
                        error = 3;
                        exit(error);
                    }
                    if(r_option_state == 1){
                        reverse_in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                    else{
                        in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                    fclose(output_file);
                    printf("DONE\n");

                    free(data_set_tree);
                    data_set_tree = NULL;

                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);
                }
            }

            //-------------------------------------- *--tab OPTION --------------------------------------//
            else if(strcmp(type_sort , tab) == 0){

                i = 0;
                index = 0;

                count = 0;
                mode = 0;

                //------------------------------------- SORT BY ID -------------------------------------//
                if (ID_option_state == 1){
                    
                    //--------------------------- *-t1/*-p1 | min : 1 , max : 1 , average : 1 ---------------------------// 
                    if(average_option_state == 1 && minimum_option_state == 1 && maximum_option_state == 1){
                        
                        start = time(NULL);
                        
                        // tab of data_set allocation
                        tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set*));
                        if (tab_data_set == NULL){
                            error = 4;
                            exit(error);
                        }
                        // fill a tab with all the single id
                        id_file = fopen(id_filename , "r");
                        if(id_file == NULL){
                            error = 4;
                            exit(error);
                        }
                        while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                            sscanf(id_line , "%d;%lf,%lf" , &ID , &x , &y);
                            tab_data_set[i] = create_data_set(ID , "\0" , 0 , 0 , 0 , INT_MAX , INT_MIN , x , y);
                            tab_data_set[i] -> pass = 0;
                            i++;
                        }
                        fclose(id_file);
                        
                        // start reading the data file and update tab_data_set
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        } 
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , date , &x , &y , &data); 

                            // search ID in tab_data_set and update the min, max, average if it's found
                            if(binary_search_data_set_ID(tab_data_set , NMB_STATIONS , ID , &index)){
                                
                                // Compute the min
                                if((tab_data_set[index]) -> min > data){
                                    (tab_data_set[index]) -> min = data;
                                } 

                                // Compute the max
                                if((tab_data_set[index]) -> max < data){
                                    (tab_data_set[index]) -> max = data;
                                }

                                // Compute the average
                                tab_data_set[index] -> nmb_data += 1;
                                tab_data_set[index] -> sum += data; 
                                tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);

                                // update the status of tab_data_set[index]
                                tab_data_set[index] -> pass = 1;
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);
                        
                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_STATIONS , r_option_state , mode , &count , output_file);
                        fclose(output_file);
                        printf("DONE\n");

                        free(tab_data_set);
                    }

                    //--------------------------- *-w | min : 0 , max : 0 , average : 1 ---------------------------// 
                    else if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        start = time(NULL);

                        // tab of data_set allocation
                        tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set*));
                        if (tab_data_set == NULL){
                            error = 4;
                            exit(error);
                        }

                        // fill dates tab with all the single dates
                        id_file = fopen(id_filename , "r");
                        if(id_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                            sscanf(id_line , "%d;%lf,%lf" , &ID , &x , &y);
                            tab_data_set[i] = create_data_set(ID , "\0" , 0 , 0 , 0 , 0 , 0 , x , y);
                            tab_data_set[i] -> pass = 0;
                            i++;
                        }
                        fclose(id_file);
                        
                        // start reading the data file and update data_set_tree
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf;%lf;%lf,%lf" , &ID , complete_date , &wind_dir , & wind_speed , &x , &y);

                            dx = wind_speed * cos((wind_dir * M_PI) / 180);
                            dy = wind_speed * sin((wind_dir * M_PI) / 180);

                            if(binary_search_data_set_ID(tab_data_set , NMB_STATIONS , ID , &index)){
                                // Compute the average
                                tab_data_set[index] -> nmb_data += 1;
                                tab_data_set[index] -> sum += dx; 
                                tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);

                                // Compute the average_1
                                tab_data_set[index] -> nmb_data_1 += 1;
                                tab_data_set[index] -> sum_1 += dy; 
                                tab_data_set[index] -> average_1 = (tab_data_set[index] -> sum_1) / (tab_data_set[index] -> nmb_data_1);

                                // update the status of tab_data_set[index]
                                tab_data_set[index] -> pass = 1;
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_STATIONS , r_option_state , mode , &count , output_file);
                        fclose(output_file);

                        free(tab_data_set);
                        printf("DONE\n");
                    } 
                
                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);     
                }
                
                //------------------------------------- SORT BY DATE -------------------------------------//
                else if (date_option_state == 1){

                    //--------------------------- *-t2/*-p2 | min : 0 , max : 0 , average : 1---------------------------// 
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){

                        start = time(NULL);

                        // tab of dates allocation
                        char** dates = (char**)malloc(NMB_DATES * sizeof(char*));
                        if (dates == NULL){
                            error = 4;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            char* dates_i = (char*)malloc(26 * sizeof(char));
                            if (dates_i == NULL){
                                error = 4;
                                exit(error);
                            }
                            memset(dates_i , 0 , sizeof(dates_i));
                            dates[i] = dates_i;
                        }
                        // fill dates tab with all the single dates
                        date_file = fopen(date_filename , "r");
                        if(date_file == NULL){
                            error = 4;
                            exit(error);
                        }
                        while (fgets(date_line , sizeof(date_line) , date_file) != NULL) {
                            sscanf(date_line , "%s" , dates[i]);
                            i++;
                        }
                        fclose(date_file);

                        // tab of data_set allocation
                        tab_data_set = malloc(NMB_DATES * sizeof(Data_Set*));
                        if (tab_data_set == NULL){
                            error = 4;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            tab_data_set[i] = create_data_set(0 , dates[i] , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                            tab_data_set[i] -> pass = 0;
                        }
                        
                        i = 0;
                        
                        // start reading the data file and update tab_data_set
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , date , &x , &y , &data);
                            data_set = create_data_set(0 , date , data , 0 , 0 , 0 , 0 , 0 , 0);

                            // search the index of date in tab_data_set and update min, 
                            // max, average of tab_data_set at the same index
                            if(binary_search_data_set_date(tab_data_set , NMB_DATES , data_set -> date , &index)){

                                tab_data_set[index] -> nmb_data += 1;
                                tab_data_set[index] -> sum += data; 
                                tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);

                                tab_data_set[index] -> pass = 1;
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_DATES , r_option_state , mode , &count , output_file);
                        fclose(output_file);
                        printf("DONE\n");
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if (average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){
                        
                        // i = 0;
                        // int index_2 = 0;
                        // start = time(NULL);

                        // // tab of all single hours
                        // char* hours[NMB_HOURS] = {"00" , "01" , "02" , "03" , "04" , "05" , 
                        //                    "06" , "07" , "08" , "09" , "10" , "11" , 
                        //                    "12" , "13" , "14" , "15" , "16" , "17" , 
                        //                    "18" , "19" , "20" , "21" , "22" , "23" } ;

                        // // tab of id allocation
                        // id_s = (int*)malloc(NMB_STATIONS * sizeof(int));
                        // if (id_s == NULL){
                        //     error = 4;
                        //     exit(error);
                        // }
                        // // fill the is tab with alll the single id
                        // id_file = fopen(id_filename , "r");
                        // if (id_file == NULL){
                        //     error = 4;
                        //     exit(error);
                        // }
                        // while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                        //     sscanf(id_line , "%d" , &id_s[i]);
                        //     i++;
                        // }
                        // fclose(id_file);

                        // // allocates a tab of tab of data_set** and fill it with the single dates
                        // Data_Set**** tab = malloc(NMB_HOURS * sizeof(Data_Set***));
                        // if (tab == NULL){
                        //     error = 4;
                        //     exit(error);
                        // }
                        // for (int i = 0 ; i < NMB_HOURS ; i++){
                        //     Data_Set*** tab_i = malloc(NMB_STATIONS * sizeof(Data_Set**));
                        //     if (tab_i == NULL){
                        //         error = 4;
                        //         exit(error);
                        //     }
                        //     for (int j = 0 ; j < NMB_STATIONS ; j++){
                                
                        //         tab_data_set = malloc(NMB_DATES * sizeof(Data_Set*));
                        //         if (tab_data_set == NULL){
                        //             error = 4;
                        //             exit(error);
                        //         }
                        //         tab_i[j] = tab_data_set;
                        //     }
                        //     tab[i] = tab_i;
                        // }

                        // for (int i = 0; i < NMB_DATES; i++){
                        //     printf("%d\n",tab[1][i][1] -> id);
                        // }
                        

                        // i = 0;

                        // // start reading the input file and update tab
                        // input_file = fopen(input_filename , "r+");
                        // if(input_file == NULL){
                        //     error = 2;
                        //     exit(error);
                        // } 
                        // while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                        //     sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                        //     data_set = create_data_set(ID , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);

                        //     // search the complete_date in tab 
                        //     // and update the data if it's found
                        //     if(binary_search_tab_date(tab , NMB_HOURS , data_set -> hour , &index)){
                        //         if(binary_search_tab_date(tab , NMB_HOURS , data_set -> hour , &index)){
                        //         i = tab[index][0] -> tab_incr;

                        //         (tab[index][i]) -> id = ID;
                        //         (tab[index][i]) -> data = data;
                        //         (tab[index][0] -> tab_incr)++;

                        //         (tab[index][i]) -> pass = 1;
                        //     }
                        // }
                        // fclose(input_file);

                        // // sort all the Data_Set* tabs of tab by id
                        // for (int i = 0 ; i < NMB_DATES ; i++){
                        //     qsort(tab[i] , NMB_STATIONS , sizeof(Data_Set*) , compare_id);
                        // }
                        // end = time(NULL);

                    //     // // write the result in a file
                    //     printf("Writing the sorting result in a file...");
                    //     output_file = fopen(output_filename , "w");
                    //     if(output_file == NULL){
                    //         error = 3;
                    //         exit(error);
                    //     }
                    //     for (int i = 0 ; i < NMB_DATES ; i++){
                    //         print_data_set_tab(tab[i] , NMB_STATIONS , r_option_state , mode , &count , output_file);
                    //     }
                    //     fclose(output_file);
                    //     printf("DONE\n");
                    //     free(tab);
                    } 

                    // elapsed = end - start;
                    // printf("Sorting time : %f seconds\n", elapsed);  
                }

                //------------------------------------- SORT BY DATA -------------------------------------//
                else if (data_option_state == 1){

                    //--------------------------- *-h/*-m | min : 0 , max : 1 , average : 0 ---------------------------//
                    if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 1){
                        
                        mode = 1;
                        start = time(NULL);

                        // tab of data_set allocation
                        tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set*));
                        if (tab_data_set == NULL){
                            error = 4;
                            exit(error);
                        }
                        // fill tab_data_set with all the single ID
                        id_file = fopen(id_filename , "r");
                        if(id_file == NULL){
                            error = 4;
                            exit(error);
                        }
                        while (fgets(id_line , sizeof(id_line) , id_file) != NULL) {
                            sscanf(id_line , "%d;%lf,%lf" , &ID , &x , &y);
                            tab_data_set[i] = create_data_set(ID , "\0" , 0 , 0 , 0 , 0 , INT_MIN , x , y);
                            i++;
                        }
                        fclose(id_file);

                        // start reading the data file and update tab_data_set
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }   
                        i = 0;
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , date , &x , &y , &data);

                            if(binary_search_data_set_ID(tab_data_set , NMB_STATIONS , ID , &index)){
                                // Compute the max
                                if((tab_data_set[index]) -> max < data){
                                    (tab_data_set[index]) -> max = data;
                                }

                                tab_data_set[index] -> pass = 1;
                            }
                            i++;
                        }
                        fclose(input_file);

                        // Sort tab_data_set by max
                        qsort(tab_data_set , NMB_STATIONS , sizeof(Data_Set*) , compare_max);
                        end = time(NULL);

                        // write the result in a file
                        printf("Writing the sorting result in a file...");
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_STATIONS , r_option_state , mode , &count , output_file);
                        fclose(output_file);
                        printf("DONE\n");
                    }
                
                    elapsed = end - start;
                    printf("Sorting time : %f seconds\n", elapsed);
                }
            }
        }
    }
    return error;
}
