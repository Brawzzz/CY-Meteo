#define NMB_STATIONS 62
#define NMB_DATES 37227
#define M_PI 3.14159265358979323846

#include "Tree.h"
#include<math.h>

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

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);

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
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }
                    
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);  
                }

                //------------------------------------- SORT BY DATE -------------------------------------// 
                else if(date_option_state == 1){

                    //--------------------------- *-t2/*-p2 | min : 0 , max : 0 , average : 1 ---------------------------//
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
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
                            data_set = create_data_set(0 , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                            data_set_tree = insert_AVL_by_date(data_set_tree , data_set , &balance);
                        }
                        end = time(NULL);
                        fclose(input_file);

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);                        
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);                         
                        }
                        fclose(output_file);
                        free(data_set_tree);
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){

                        mode = -1;
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
                        // fill the tab with all the single dates
                        date_file = fopen(date_filename , "r");
                        if(date_file == NULL){
                            error = 2;
                            exit(error);
                        } 
                        while (fgets(date_line , sizeof(date_line) , date_file) != NULL) {
                            sscanf(date_line , "%s" , dates[i]);
                            i++;
                        }
                        fclose(date_file);

                        // allocates a tab of p_tree
                        tree_tab = malloc(NMB_DATES * sizeof(p_tree));
                        if (tree_tab == NULL){
                            error = 4;
                            exit(error);
                        }

                        // start reading the data file and update tree_tab
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(ID , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);

                            // resarch complete_date in the tab of p_tree 
                            if(binary_search_date(dates , NMB_DATES , complete_date , &index)){
                                tree_tab[index] = insert_AVL_by_ID(tree_tab[index] , data_set , &balance , mode); 
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);

                        mode = 0;

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
 
                        if(r_option_state == 1){
                            for (int i = 0 ; i < NMB_DATES ; i++){
                                reverse_in_fixe_search(tree_tab[i] , output_file , mode);                        
                            }
                        }
                        else{
                            for (int i = 0 ; i < NMB_DATES ; i++){
                                in_fixe_search(tree_tab[i] , output_file , mode , &count);                        
                            }   
                        }
                        fclose(output_file);
                        free(tree_tab);
                    }

                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);
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

                    // write the result in a file
                    output_file = fopen(output_filename , "w");
                    if(output_file == NULL){
                        error = 3;
                        exit(error);
                    }
                    mode = 1;
                    if(r_option_state == 1){
                        reverse_in_fixe_search(data_set_tree , output_file , mode);
                    }
                    else{
                        in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                    fclose(output_file);

                    free(data_set_tree);
                    data_set_tree = NULL;
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);
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

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);

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
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);
                        }
                        fclose(output_file);

                        free(data_set_tree);
                        data_set_tree = NULL;
                    }
                    
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);  
                }

                //------------------------------------- SORT BY DATE -------------------------------------// 
                else if(date_option_state == 1){

                    //--------------------------- *-t2/*-p2 | min : 0 , max : 0 , average : 1 ---------------------------//
                    if(average_option_state == 1 && minimum_option_state == 0 && maximum_option_state == 0){
                        
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
                            data_set = create_data_set(0 , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);
                            data_set_tree = insert_ABR_by_date(data_set_tree , data_set);
                        }
                        end = time(NULL);
                        fclose(input_file);

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            reverse_in_fixe_search(data_set_tree , output_file , mode);                        
                        }
                        else{
                            in_fixe_search(data_set_tree , output_file , mode , &count);                         
                        }
                        fclose(output_file);
                        free(data_set_tree);
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){

                        mode = -1;
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
                        // fill a tab with all the single dates
                        date_file = fopen(date_filename , "r");
                        if(date_file == NULL){
                            error = 2;
                            exit(error);
                        } 
                        while (fgets(date_line , sizeof(date_line) , date_file) != NULL) {
                            sscanf(date_line , "%s" , dates[i]);
                            i++;
                        }
                        fclose(date_file);

                        // allocates and fill a tab of p_tree with the single dates
                        tree_tab = malloc(NMB_DATES * sizeof(p_tree));
                        if (tree_tab == NULL){
                            error = 4;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            data_set = create_data_set(0 , dates[i] , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                            tree_tab[i] = create_tree(data_set); 
                        }

                        // start reading the data file and update tree_tab
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        }
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);
                            data_set = create_data_set(ID , complete_date , data , 0 , 0 , 0 , 0 , 0 , 0);

                            // resarch complete_date in the tab of p_tree 
                            if(binary_search_tree_date(tree_tab , NMB_DATES , complete_date , &index)){
                                tree_tab[index] = insert_ABR_by_ID(tree_tab[index] , data_set , mode);
                            }
                        }
                        end = time(NULL);
                        fclose(input_file);

                        mode = 0;
                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        if(r_option_state == 1){
                            for (int i = 0 ; i < NMB_DATES ; i++){
                                reverse_in_fixe_search(tree_tab[i] , output_file , mode);                        
                            }
                        }
                        else{
                            for (int i = 0 ; i < NMB_DATES ; i++){
                                in_fixe_search(tree_tab[i] , output_file , mode , &count);                        
                            }   
                        }
                        fclose(output_file);
                        free(tree_tab);
                    }

                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);
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

                    output_file = fopen(output_filename , "w");
                    if(output_file == NULL){
                        error = 3;
                        exit(error);
                    }

                    mode = 1;

                    if(r_option_state == 1){
                        reverse_in_fixe_search(data_set_tree , output_file , mode);
                    }
                    else{
                        in_fixe_search(data_set_tree , output_file , mode , &count);
                    }
                  
                    fclose(output_file);

                    free(data_set_tree);
                    data_set_tree = NULL;
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);
                }
            }

            //-------------------------------------- *--tab OPTION --------------------------------------//
            else if(strcmp(type_sort , tab) == 0){

                i = 0;
                index = 0;

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
                            error = 2;
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
                            else{
                                
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);
                        
                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_STATIONS , r_option_state , output_file);
                        fclose(output_file);

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
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_STATIONS ; i++){
                            if(tab_data_set[i] -> pass == 1){
                                fprintf(output_file , "%d,%lf,%lf,%lf,%lf\n" , tab_data_set[i] -> id , tab_data_set[i] -> average , tab_data_set[i] -> average_1 , tab_data_set[i] -> x , tab_data_set[i] -> y);
                            }
                        }
                        fclose(output_file);

                        free(tab_data_set);
                    } 
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);     
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
                            error = 2;
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
                            tab_data_set[i] = create_set();
                            tab_data_set[i] -> date = "\0";
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
                            
                            // search the index of date in dates and update min, 
                            // max, average of tab_data_set at the same index
                            if(binary_search_date(dates , NMB_DATES , date , &index)){

                                // create a new data_set if ther is no date at tab_data_set[index]
                                if(strcmp(tab_data_set[index] -> date , "\0") == 0){

                                    tab_data_set[index] = create_data_set(0 , date , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                                    // Compute the average
                                    tab_data_set[index] -> nmb_data += 1;
                                    tab_data_set[index] -> sum += data; 
                                    tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);
                                    
                                    tab_data_set[index] -> pass = 1;
                                }
                                else{
                                    // Compute the average
                                    tab_data_set[index] -> nmb_data += 1;
                                    tab_data_set[index] -> sum += data; 
                                    tab_data_set[index] -> average = (tab_data_set[index] -> sum) / (tab_data_set[index] -> nmb_data);
                                }
                            }
                        }
                        fclose(input_file);
                        end = time(NULL);

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_DATES , r_option_state , output_file);
                        fclose(output_file);
                    }

                    //--------------------------- *-t3/*-p3 | min : 0 , max : 0 , average : 0 ---------------------------//
                    else if (average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 0){

                        i = 0;
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
                        // fill dates with all the single dates
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

                        // allocates a tab of tab of data_set* and fill it with the single dates
                        Data_Set*** tab = malloc(NMB_DATES * sizeof(Data_Set**));
                        if (tab == NULL){
                            error = 4;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            tab_data_set = malloc(NMB_STATIONS * sizeof(Data_Set*));
                            if (tab_data_set == NULL){
                                error = 4;
                                exit(error);
                            }
                            for (int j = 0 ; j < NMB_STATIONS ; j++){
                                tab_data_set[j] = create_data_set(0 , dates[i] , 0 , 0 , 0 , 0 , 0 , 0 , 0);
                                tab_data_set[j] -> pass = 0;
                            }
                            tab[i] = tab_data_set;
                        }

                        i = 0;

                        // start reading the input file and update tab
                        input_file = fopen(input_filename , "r+");
                        if(input_file == NULL){
                            error = 2;
                            exit(error);
                        } 
                        while (fgets(data_line, sizeof(data_line) , input_file) != NULL) {

                            sscanf(data_line , "%d;%[^;];%lf,%lf;%lf" , &ID , complete_date , &x , &y , &data);

                            // search the complete_date in tab 
                            // and update the data if it's found
                            if(binary_search_tab_date(tab , NMB_DATES , complete_date , &index)){
                                i = tab[index][0] -> tab_incr;

                                (tab[index][i]) -> id = ID;
                                (tab[index][i]) -> data = data;
                                (tab[index][0] -> tab_incr)++;

                                (tab[index][i]) -> pass = 1;
                            }
                        }
                        fclose(input_file);

                        // sort all the Data_Set* tabs of tab by id
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            qsort(tab[i] , NMB_STATIONS , sizeof(Data_Set*) , compare_id);
                        }
                        end = time(NULL);

                        // // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        for (int i = 0 ; i < NMB_DATES ; i++){
                            for (int j = 0 ; j < NMB_STATIONS ; j++){
                                if(tab[i][j] -> pass == 1){
                                    fprintf(output_file , "%d,%s,%lf\n" , tab[i][j] -> id , tab[i][j] -> date , tab[i][j] -> data);
                                }
                            }
                        }
                        fclose(output_file);
                        free(tab);
                    } 
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);  
                }

                //------------------------------------- SORT BY DATA -------------------------------------//
                else if (data_option_state == 1){

                    //--------------------------- *-h/*-m | min : 0 , max : 1 , average : 0 ---------------------------//
                    if(average_option_state == 0 && minimum_option_state == 0 && maximum_option_state == 1){

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
                            error = 2;
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

                        qsort(tab_data_set , NMB_STATIONS , sizeof(Data_Set*) , compare_max);
                        end = time(NULL);

                        // write the result in a file
                        output_file = fopen(output_filename , "w");
                        if(output_file == NULL){
                            error = 3;
                            exit(error);
                        }
                        print_data_set_tab(tab_data_set , NMB_STATIONS , r_option_state , output_file);
                        
                        fclose(output_file);
                    }
                
                    elapsed = end - start;
                    printf("Temps écoulé : %f secondes\n", elapsed);
                }
            }
        }
    }
    return error;
}
