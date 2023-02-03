#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#include "Tree.h"

int max(int a , int b){
    return (a < b ? b : a);
}

int min(int a , int b){
    return (a < b ? a : b);
}

int Max(int a , int b , int c){
    return ( max(max(a , b) , max(b , c)) );
}

int Min(int a , int b , int c){
    return ( min(min(a , b) , min(b , c)) );
}

//******************************************************//
//***********************   TREE   *********************//
//******************************************************//

// Create a Tree*
p_tree create_tree(Data_Set* new){

    p_tree t = malloc(sizeof(Tree));
    if (t == NULL){
        exit(4);
    }

    t -> element = new;
    t -> left_son = NULL;
    t -> right_son = NULL;
    t -> equilibre = 0;

    return t;
}

// Return true if a node is empty
bool is_empty(p_tree t){  
    if (t == NULL){
        return true;
    }
    else{
        return false;
    }
}

// Return true if a nodes is a leaf
bool is_leaf(p_tree t){

    if(is_empty(t)){
        printf("L'arbre est vide\n");
        exit(1);
    }  
    else if(t -> left_son == NULL && t -> right_son == NULL){
        return true;
    }
    else{
        return false;
    }
}

// Return the element of a node
Data_Set* get_element(p_tree t){
    return (t -> element);
}

// Return the number of nodes in a tree
int nmb_nodes(p_tree t){

    if(is_empty(t)){
        return 0;
    }
    else if(is_leaf(t)){
        return 1;
    } 
    else{
        return 1 + nmb_nodes(t -> left_son) + nmb_nodes(t -> right_son); 
    }
}

// Return true if a node has a left son
bool has_left_son(p_tree t){
    
    if(is_empty(t)){
        return false;
    }  
    else if (t -> left_son == NULL){
        return false;
    }
    else{
        return true;
    }
}

// Return true if a node has a right son
bool has_right_son(p_tree t){

    if(is_empty(t)){
        return false;
    }  
    else if (t -> right_son == NULL){
        return false;
    }
    else{
        return true;
    }
}

// Return the height of a node
int get_height(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    else{
        return 1 + max(get_height(t -> left_son) , get_height(t -> right_son));
    }
}

// In fix saerch function by increasing order
void in_fixe_search(p_tree t , FILE* file , int mode , int* c){

    if (t == NULL){
        printf("\n");
    }
    else{

        if (mode == 1){
            if(has_left_son(t)){
                in_fixe_search(t -> left_son , file , mode , c);
            }
            
            if((t -> element) -> pass == 1){
                fprintf(file , "%d,%d,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n\n", *c , (t -> element) -> id , (t -> element) -> date , (t -> element) -> data , (t -> element) -> min , (t -> element) -> max ,
                        (t -> element) -> average , (t -> element) -> average_1 , (t -> element) -> x , (t -> element) -> y);
                *c += 1;
            }

            if(has_right_son(t)){
                in_fixe_search(t -> right_son , file , mode , c);
            }
        }

        else{
            if(has_left_son(t)){
                in_fixe_search(t -> left_son , file , mode , c);
            }
            
            if((t -> element) -> pass == 1){
                fprintf(file , "%d,%d,%s,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", *c , (t -> element) -> id , (t -> element) -> date , (t -> element) -> hour , (t -> element) -> data , (t -> element) -> min , (t -> element) -> max ,
                        (t -> element) -> average , (t -> element) -> average_1 , (t -> element) -> x , (t -> element) -> y);
                // *c += 1;
            }

            if(has_right_son(t)){
                in_fixe_search(t -> right_son , file , mode , c);
            }
        }
    }
}

// In fix saerch function by decreasing order
void reverse_in_fixe_search(p_tree t , FILE* file , int mode , int* c){

    if (t == NULL){
        printf("\n");
    }
    else{
        if (mode == 1){
            if(has_left_son(t)){
                reverse_in_fixe_search(t -> left_son , file , mode , c);
            }

            if((t -> element) -> pass == 1){
                fprintf(file , "%d,%d,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n\n", *c , (t -> element) -> id , (t -> element) -> date , (t -> element) -> data , (t -> element) -> min , (t -> element) -> max ,
                        (t -> element) -> average , (t -> element) -> average_1 , (t -> element) -> x , (t -> element) -> y);
                *c += 1;
            }

            if(has_right_son(t)){
                reverse_in_fixe_search(t -> right_son , file , mode , c);
            }
        }
        else{
            if(has_left_son(t)){
                reverse_in_fixe_search(t -> left_son , file , mode , c);
            }

            if((t -> element) -> pass == 1){
                fprintf(file , "%d,%d,%s,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", *c , (t -> element) -> id , (t -> element) -> date , (t -> element) -> data , (t -> element) -> min , (t -> element) -> max ,
                        (t -> element) -> average , (t -> element) -> average_1 , (t -> element) -> x , (t -> element) -> y);
                *c += 1;
            }

            if(has_right_son(t)){
                reverse_in_fixe_search(t -> right_son , file , mode , c);
            }
        }
    }
}

// Resarch a node in a tree sorted by id
bool search_by_id(p_tree t , Data_Set* element){

    bool is_in = false;

    if (is_empty(t)){
        return false;
    }

    else if((t -> element) -> id == element -> id){
        return true;
    }

    else if((t -> element) -> id > element -> id){
        if(has_left_son(t)){
            return search_by_id(t -> left_son , element);
        }
    }

    else if((t -> element) -> id < element -> id){
        if(has_right_son(t)){
            return search_by_id(t -> right_son , element);
        }
    }
    return is_in;
}

// Resarch a node in a tree sorted by date
bool search_by_date(p_tree t , Data_Set* element , int* nmb_knots){

    bool is_in = false;

    if (is_empty(t)){
        return false;
    }

    else if(strcmp((t -> element) -> date , element -> date) == 0){
        *nmb_knots += 1;
        is_in = true;

        // Compute the average
        (t -> element) -> nmb_data += 1;
        (t -> element) -> sum += element -> data; 
        (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);

        (t -> element) -> pass = 1;
    }

    else if(strcmp((t -> element) -> date , element -> date) > 0){
        *nmb_knots += 1;
        if(has_left_son(t)){
            return search_by_date(t -> left_son , element , nmb_knots);
        }
    }

    else if(strcmp((t -> element) -> date , element -> date) < 0){
        *nmb_knots += 1;
        if(has_right_son(t)){
            return search_by_date(t -> right_son , element , nmb_knots);
        }
    }
    return is_in;
}

// Binary search algorithme for date
// Search a date in a tab of p_tree
// and return the index of the id
bool binary_search_tree_id(p_tree array[] , int size , int value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (((array[*index] -> element) -> id) == value){
            return true;
        }
        else if (((array[*index] -> element) -> id) < value){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// Binary search algorithme for date
// Search a date in a tab of p_tree
// and return the index of the date
bool binary_search_tree_date(p_tree array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp((array[*index] -> element) -> date , value) == 0){
            return true;
        }
        else if (strcmp((array[*index] -> element) -> date , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

// Binary search algorithme for hour
// Search an hour in a tab of p_tree
// and return the index of the hour
bool binary_search_tree_hour(p_tree* array[] , int size , char* value , int* index) {
    int low = 0;
    int high = size - 1;

    while (low <= high){

        *index = (low + high) / 2;

        if (strcmp((array[0][*index] -> element) -> hour , value) == 0){
            return true;
        }
        else if (strcmp((array[0][*index] -> element) -> hour , value) < 0){
            low = *index + 1;
        }
        else{
            high = *index - 1;
        }
    }
    return false;
}

//********************************************************************//
//***********************   BINARY SEARCH TREE   *********************//
//********************************************************************//

p_tree insert_ABR_by_ID(p_tree t , Data_Set* element , int mode){

    if (t == NULL){
        t = create_tree(element);
        (t -> element) -> pass = 1; 
        return t;
    }
    else if ( get_element(t) -> id > element -> id){
        t -> left_son = insert_ABR_by_ID(t -> left_son , element , mode);
    }
    else  if (get_element(t) -> id < element -> id ){
        t -> right_son = insert_ABR_by_ID(t -> right_son , element , mode);
    }
    else{
        if(mode == 0){
            // Compute the max
            if((t -> element) -> max < element -> data){
                (t -> element) -> max = element -> data;
            }
            return t;
        }
        else if(mode == 1){
            // Compute the min
            if((t -> element) -> min > element -> data){
                (t -> element) -> min = element -> data;
            } 

            // Compute the max
            if((t -> element) -> max < element -> data){
                (t -> element) -> max = element -> data;
            }

            // Compute the average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> data; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);

            return t; 
        }
        else if (mode == 2){
            // Compute the d_x average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> d_x; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);

            // Compute the d_y average
            (t -> element) -> nmb_data_1 += 1;
            (t -> element) -> sum_1 += element -> d_y; 
            (t -> element) -> average_1 = ((t -> element) -> sum_1) / ((t -> element) -> nmb_data_1);

            return t;
        } 
        else{
            t -> right_son = insert_ABR_by_ID(t -> right_son , element , mode);

        }
    }
    return t;
}

p_tree insert_ABR_by_date(p_tree t , Data_Set* element , int mode){

    if (t == NULL){
        t = create_tree(element);
        (t -> element) -> pass = 1;
        return t;
    }
    else if (strcmp((t -> element) -> date , element -> date) > 0){
        t -> left_son = insert_ABR_by_date(t -> left_son , element , mode);
    }
    else  if (strcmp((t -> element) -> date , element -> date) < 0){
        t -> right_son = insert_ABR_by_date(t -> right_son , element , mode);
    }
    else{
        if(mode == 1){
            return t;
        }
        else{
            // Compute the average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> data; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);
            return t;
        }
    }
    return t;
}

// Insert a new nodes by max
p_tree insert_ABR_by_max(p_tree t , Data_Set* element){
    
    if (t == NULL){
        t = create_tree(element);
        return t;
    }
    else if ((t -> element) -> max >= element -> max){
        t -> left_son = insert_ABR_by_max(t -> left_son , element);
    }
    else  if ((t -> element) -> max < element -> max){
        t -> right_son = insert_ABR_by_max(t -> right_son , element);
    }
    return t;
}

// Insert a new nodes by max field from a tree sorted by id  
void insert_ABR_by_max_from_id(p_tree t1 , p_tree* t2){

    if (t1 == NULL){
        return;
    }
    else{
        if(has_left_son(t1)){
            insert_ABR_by_max_from_id(t1 -> left_son , t2);
        }

        *t2 = insert_ABR_by_max(*t2 , t1 -> element);

        if(has_right_son(t1)){
            insert_ABR_by_max_from_id(t1 -> right_son , t2);
        }
    }
}

// Print ABR with default fields
void print_ABR(p_tree t , int space){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%d , %s , %lf , %lf , %lf]\n", (t -> element)-> id , (t -> element)-> date ,(t -> element)-> min , (t -> element)-> max , (t -> element)-> average);

        if (p -> left_son != NULL){
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            
            printf(" |__ ");
            print_ABR(p -> left_son , space+1);
        }
        else{
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            printf(" |__ \n");
        }

        if (p -> right_son != NULL){
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }

            printf(" |__ ");
            print_ABR(p -> right_son , space+1);
        }
        else{
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            printf(" |__ \n");
        }             
        space = 0;
    }
}

//********************************************************************//
//*******************************   AVL   ****************************//
//********************************************************************//

// Return the balance factor of a node
int get_balance_factor(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    else{
        return( get_height(t -> right_son) - get_height(t -> left_son));
    }
}

// Left rotation operation
p_tree left_rotation(p_tree t){
    p_tree pivot = NULL;
    int eq_t = 0;
    int eq_p = 0;
    
    pivot = t -> right_son;
    t -> right_son = pivot -> left_son;
    pivot -> left_son = t;

    eq_t = t -> equilibre;
    eq_p = pivot -> equilibre;
    (t -> equilibre) = eq_t - max(pivot -> equilibre , 0) - 1;
    (pivot -> equilibre) = Min(eq_t - 2 , eq_t + eq_p - 2 , eq_p - 1);

    t = pivot;
    return t;
}

// Right rotation operation
p_tree right_rotation(p_tree t){
    p_tree pivot = NULL;
    int eq_t = 0;
    int eq_p = 0;

    pivot = t -> left_son;
    t -> left_son = pivot -> right_son;
    pivot -> right_son = t;

    eq_t = t -> equilibre;
    eq_p = pivot -> equilibre;
    (t -> equilibre) = eq_t - min(eq_p , 0) + 1;
    (pivot -> equilibre) = Max(eq_t + 2 , eq_t + eq_p + 2 , eq_p + 1); 

    t = pivot;
    return t;
}

// Double left rotation operation
p_tree double_left_rotation(p_tree t){
    t -> right_son = right_rotation(t -> right_son);
    return left_rotation(t);
}

// Double right rotation operation
p_tree double_right_rotation(p_tree t){
    t -> left_son = left_rotation(t -> left_son);
    return right_rotation(t);
}

// Balance a node in an AVL
p_tree balance_AVL(p_tree t){

    if(t -> equilibre >= 2){
        if(((t -> right_son) -> equilibre) >= 0){
            return left_rotation(t);
        }
        else{
            return double_left_rotation(t);
        }
    }
    else if (t -> equilibre <= -2){
        if(((t -> left_son) -> equilibre) <= 0){
            return right_rotation(t);
        }
        else{
            return double_right_rotation(t);
        }
    }
    else{
        return t;
    }
}

// Insert a new node by id
p_tree insert_AVL_by_ID(p_tree t , Data_Set* element , int* h , int mode){

    if (t == NULL){
        *h = 1;
        t = create_tree(element);
        ((t -> element) -> pass) = 1; 
        return t;
    }
    else if ((t -> element) -> id > element -> id){
        t -> left_son = insert_AVL_by_ID(t -> left_son , element , h , mode);
        *h = -*h;
    }
    else  if ((t -> element) -> id < element -> id ){
        t -> right_son = insert_AVL_by_ID(t -> right_son , element , h , mode);
    }
    else{
        *h = 0;
        ((t -> element) -> pass) = 1;

        if(mode == 0){
            // Compute the max
            if((t -> element) -> max < element -> data){
                (t -> element) -> max = element -> data;
            }
            return t;
        }
        else if(mode == 1){
            // Compute the min
            if((t -> element) -> min > element -> data){
                (t -> element) -> min = element -> data;
            } 

            // Compute the max
            if((t -> element) -> max < element -> data){
                (t -> element) -> max = element -> data;
            }

            // Compute the average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> data; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);

            return t; 
        }
        else if (mode == 2){
            // Compute the d_x average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> d_x; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);

            // Compute the d_y average
            (t -> element) -> nmb_data_1 += 1;
            (t -> element) -> sum_1 += element -> d_y; 
            (t -> element) -> average_1 = ((t -> element) -> sum_1) / ((t -> element) -> nmb_data_1);

            return t;
        } 
        else{
            t -> right_son = insert_AVL_by_ID(t -> right_son , element , h , mode);
        }
    }

    if(*h != 0){
        t -> equilibre += *h;
        t = balance_AVL(t);
        if(t -> equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return t;
}

// Insert a new nodes by date
p_tree insert_AVL_by_date(p_tree t , Data_Set* element , int mode , int* h){

    if (t == NULL){
        *h = 1;
        t = create_tree(element);
        (t -> element) -> pass = 1;
        return t;
    }
    else if (strcmp((t -> element) -> date , element -> date) > 0){
        t -> left_son = insert_AVL_by_date(t -> left_son , element , mode , h);
        *h = -*h;
    }
    else  if (strcmp((t -> element) -> date , element -> date) < 0){
        t -> right_son = insert_AVL_by_date(t -> right_son , element , mode , h);
    }
    else{
        *h = 0;
        if (mode == 1){
            return t;
        }
        else{
            // Compute the average
            (t -> element) -> nmb_data += 1;
            (t -> element) -> sum += element -> data; 
            (t -> element) -> average = ((t -> element) -> sum) / ((t -> element) -> nmb_data);
            return t;
        }
    }

    if(*h != 0){
        t -> equilibre += *h;
        t = balance_AVL(t);
        if(t -> equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }   
    return t;    
}

// Insert a new nodes by max
p_tree insert_AVL_by_max(p_tree t , Data_Set* element){
    
    if (t == NULL){
        t = create_tree(element);
        return t;
    }
    else if ((t -> element) -> max >= element -> max){
        t -> left_son = insert_AVL_by_max(t -> left_son , element);
    }
    else  if ((t -> element) -> max < element -> max){
        t -> right_son = insert_AVL_by_max(t -> right_son , element);
    }

    return t;
}

// Insert a new nodes by max field from a tree sorted by id  
void insert_AVL_by_max_from_id(p_tree t1 , p_tree* t2){

    if (t1 == NULL){
        return;
    }
    else{
        if(has_left_son(t1)){
            insert_AVL_by_max_from_id(t1 -> left_son , t2);
        }

        *t2 = insert_AVL_by_max(*t2 , t1 -> element);

        if(has_right_son(t1)){
            insert_AVL_by_max_from_id(t1 -> right_son , t2);
        }
    }
}

// Insert a new nodes by max field from a tree sorted by id  
void insert_AVL_by_date_from_id(p_tree t1 , p_tree* t2){

    if (t1 == NULL){
        return;
    }
    else{
        if(has_left_son(t1)){
            insert_AVL_by_date_from_id(t1 -> left_son , t2);
        }

        int balance  = 0;
        *t2 = insert_AVL_by_date(*t2 , t1 -> element , 1 , &balance);

        if(has_right_son(t1)){
            insert_AVL_by_date_from_id(t1 -> right_son , t2);
        }
    }
}

// Print AVL with default fields
void print_AVL(p_tree t , int space){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%d , %s , %lf , %lf , %lf -- eq : %d]\n", (t -> element)-> id , (t -> element)-> date ,(t -> element)-> min , (t -> element)-> max , (t -> element)-> average , t -> equilibre);

        if (p -> left_son != NULL){
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            
            printf(" |__ ");
            print_AVL(p -> left_son , space+1);
        }
        else{
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            printf(" |__ \n");
        }

        if (p -> right_son != NULL){
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }

            printf(" |__ ");
            print_AVL(p -> right_son , space+1);
        }
        else{
            for (int i = 0 ; i < space ; i++){
                printf("     ");
            }
            printf(" |__ \n");
        }             
        space = 0;
    }
}

void fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    else{
        if(has_left_son(t)){
            fixe_search(t -> left_son , file);
        }
        
        if((t -> element) -> pass == 0){
            fprintf(file , "%d , %d\n", (t -> element) -> pass , (t -> element) -> id );
        }

        if(has_right_son(t)){
            fixe_search(t -> right_son , file);
        }
    }
}
  