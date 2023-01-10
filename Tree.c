#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string.h>

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

//*******************************************************************************************************************//
//*********************************************                         *********************************************//
//*********************************************           TREE          *********************************************//
//*********************************************                         *********************************************//
//*******************************************************************************************************************//

p_tree create_tree(Data_Set* new){

    p_tree t = malloc(sizeof(Tree));
    if (t == NULL){
        exit(1);
    }

    t -> element = new;
    t -> left_son = NULL;
    t -> right_son = NULL;
    t -> equilibre = 0;

    return t;
}

bool is_empty(p_tree t){
     
    if (t == NULL){
        return true;
    }

    else{
        return false;
    }
}

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

Data_Set* get_element(p_tree t){
    return (t -> element);
}

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

int get_height(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    
    else{
        return 1 + max(get_height(t -> left_son) , get_height(t -> right_son));
    }
}

void in_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    
    else{

        if(has_left_son(t)){
            in_fixe_search(t -> left_son , file);
        }

        fprintf(file , "%d , %s , %lf , %lf , %lf\n", (t -> element)-> id , (t -> element)-> date , (t -> element)-> min , (t -> element)-> max , (t -> element)-> average);

        if(has_right_son(t)){
            in_fixe_search(t -> right_son , file);
        }
    }
}

void reverse_in_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    
    else{

        if(has_right_son(t)){
            reverse_in_fixe_search(t -> right_son , file);
        }
        
        fprintf(file , "%d , %s , %lf , %lf , %lf\n", (t -> element)-> id , (t -> element)-> date , (t -> element)-> min , (t -> element)-> max , (t -> element)-> average);
        
        if(has_left_son(t)){
            reverse_in_fixe_search(t -> left_son , file);
        }
    }
}

int get_balance_factor(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    else{
        return( get_height(t -> right_son) - get_height(t -> left_son));
    }
}

p_tree left_rotation(p_tree t){

    p_tree pivot = t -> right_son;
    p_tree x = pivot -> left_son;

    pivot -> left_son = t;
    t -> right_son = x;

    t -> height = get_height(t);
    pivot -> height = get_height(pivot);

    t -> equilibre = get_balance_factor(pivot);
    pivot -> equilibre = get_balance_factor(pivot);
    
    return pivot;
}

p_tree right_rotation(p_tree t){
    
    p_tree pivot = t -> left_son;
    p_tree x = pivot -> right_son;

    pivot -> right_son = t;
    t -> left_son = x;

    t -> height = get_height(t);
    pivot -> height = get_height(pivot);

    t -> equilibre = get_balance_factor(pivot);
    pivot -> equilibre = get_balance_factor(pivot);

    return pivot;
}

p_tree double_left_rotation(p_tree t){

    t -> right_son = right_rotation(t -> right_son);
    return left_rotation(t);

}

p_tree double_right_rotation(p_tree t){

    t -> left_son = left_rotation(t -> left_son);
    return right_rotation(t);

}

p_tree balance_AVL(p_tree t){

    if(t -> equilibre >= 2){
        if((t -> right_son) -> equilibre >= 0){
            return left_rotation(t);
        }
        else{
            return double_left_rotation(t);
        }
    }
    else if (t -> equilibre <= -2){
        if((t -> left_son) -> equilibre <= 0){
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

p_tree insert_AVL_by_ID(p_tree t , Data_Set* element){

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( get_element(t) -> id > element -> id){
        t -> left_son = insert_AVL_by_ID(t -> left_son , element);
    }

    else  if (get_element(t) -> id < element -> id ){
        t -> right_son = insert_AVL_by_ID(t -> right_son , element);
    }

    else{
        return t;
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree insert_AVL_by_min(p_tree t , Data_Set* element){

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (get_element(t)-> min >= element-> min){
        t -> left_son = insert_AVL_by_min(t -> left_son , element);
    }

    else  if (get_element(t)-> min < element-> min){
        t -> right_son = insert_AVL_by_min(t -> right_son , element);
    }

    else{
        return t;
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree insert_AVL_by_max(p_tree t , Data_Set* element){
    
    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (get_element(t)-> max >= element-> max){
        t -> left_son = insert_AVL_by_max(t -> left_son , element);
    }

    else  if (get_element(t)-> max < element-> max){
        t -> right_son = insert_AVL_by_max(t -> right_son , element);
    }

    else{
        return t;
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree insert_AVL_by_average(p_tree t , Data_Set* element){

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (get_element(t)-> average >= element-> average){
        t -> left_son = insert_AVL_by_average(t -> left_son , element);
    }

    else  if (get_element(t)-> average < element-> average){
        t -> right_son = insert_AVL_by_average(t -> right_son , element);
    }

    else{
        return t;
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;

}

p_tree insert_AVL_by_date(p_tree t , Data_Set* element){

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (strcmp(get_element(t)-> date , element-> date) >= 0){
        t -> left_son = insert_AVL_by_date(t -> left_son , element);
    }

    else  if (strcmp(get_element(t)-> date , element-> date) < 0){
        t -> right_son = insert_AVL_by_date(t -> right_son , element);
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

void print_AVL(p_tree t , int space){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%d , %lf , %lf , %lf -- eq : %d]\n", (t -> element)-> id , (t -> element)-> min , (t -> element)-> max , (t -> element)-> average , t -> equilibre);

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



/*

bool is_empty(p_tree t){
     
    if (t == NULL){
        return true;
    }

    else{
        return false;
    }
}

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

int nmb_leaf(p_tree t){
    
    if (is_empty(t)){
        return 0;
    } 
    if (is_leaf(t)){
        return 1;
    } 
    return nmb_leaf(t -> left_son) + nmb_leaf(t -> right_son);
}

bool has_left_son(p_tree t){
    
    if(is_empty(t)){
        printf("L'arbre est vide\n");
        exit(1);
    }  

    else if (t -> left_son == NULL){
        return false;
    }

    else{
        return true;
    }
}

bool has_right_son(p_tree t){

    if(is_empty(t)){
        printf("L'arbre est vide\n");
        exit(1);
    }  

    else if (t -> right_son == NULL){
        return false;
    }

    else{
        return true;
    }
}

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

void* get_element(p_tree t){
    return (t -> element);
}

int get_height(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    
    else{
        return 1 + max(get_height(t -> left_son) , get_height(t -> right_son));
    }
}

bool add_left_son(p_tree t , void* element){

    if(is_empty(t)){
        t = create_tree(element);
        return true;
    }

    else if (!has_left_son(t)){
        t -> left_son = create_tree(element);  
        return true;
    }

    else{
        return false;
    }
}

bool add_right_son(p_tree t , void* element){

    if(is_empty(t)){
        t = create_tree(element);
        return true;
    }

    else if (!has_right_son(t)){
        t -> right_son = create_tree(element);
        return true;
    }
    
    else{
        return false;
    }
}

void delete_left_son(p_tree t){

    if (!is_empty(t) && has_left_son(t)){

        if (has_left_son(t -> left_son)){
            delete_left_son(t -> left_son);
        }

        if (has_right_son(t -> left_son)){
            delete_right_son(t -> left_son);
        }

        free((t -> left_son));
        t -> left_son = NULL;
    }    
}

void delete_right_son(p_tree t){

    if (!is_empty(t) && has_right_son(t)){

        if (has_left_son(t -> right_son)){
            delete_left_son(t -> right_son);
        }

        if (has_right_son(t -> right_son)){
            delete_right_son(t -> right_son);
        }
        
        free(t -> right_son);
        t -> right_son = NULL;
    }     
}

/*
void print_tree(p_tree t , int j){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%[format]pair.field]\n" , get_element(p));

        if (p -> left_son != NULL){
            for (int i = 0 ; i < j ; i++){
                printf(" |   ");
            }
            
            printf(" |-> ");
            print_tree(p -> left_son , j+1);
        }
        else{
            for (int i = 0 ; i < j ; i++){
                printf(" |   ");
            }
            printf(" |-> \n");
        }

        if (p -> right_son != NULL){
            for (int i = 0 ; i < j ; i++){
                printf(" |   ");
            }

            printf(" |-> ");
            print_tree(p -> right_son , j+1);
        }
        else{
            for (int i = 0 ; i < j ; i++){
                printf(" |   ");
            }
            printf(" |-> \n");
        }
                
        j = 0;
    }
}

void pre_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }

    else{

        fprintf(file , "[format]pair.fields\n" , get_element(t));

        if(has_left_son(t)){
            pre_fixe_search(t -> left_son , file);
        }

        if(has_right_son(t)){
            pre_fixe_search(t -> right_son , file);
        }
    }
}

void in_fixe_data_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    
    else{

        if(has_left_son(t)){
            in_fixe_data_search(t -> left_son , file);
        }

        fprintf(file , "%d , %lf\n" , ((Data_Pair*)get_element(t)) -> key , ((Data_Pair*)get_element(t)) -> data);

        if(has_right_son(t)){
            in_fixe_data_search(t -> right_son , file);
        }
    }
}

void in_fixe_id_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    
    else{

        if(has_left_son(t)){
            in_fixe_id_search(t -> left_son , file);
        }

        fprintf(file , "%d , %lf\n" , get_element_id(t) , get_element_data(t));

        if(has_right_son(t)){
            in_fixe_id_search(t -> right_son , file);
        }
    }
}

void reverse_in_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }
    
    else{

        if(has_right_son(t)){
            reverse_in_fixe_search(t -> right_son , file);
        }

        fprintf(file , "%lf" , get_element(t));

        if(has_left_son(t)){
            reverse_in_fixe_search(t -> left_son , file);
        }
    }
}

void post_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("\n");
    }

    else{

        if(has_left_son(t)){
            post_fixe_search(t -> left_son , file);
        }

        if(has_right_son(t)){
            post_fixe_search(t -> right_son , file);
        }

        fprintf(file , "[format]pair.fields\n" , get_element(t));
    }
}
*/

//*********************************************************************************************************************//
//**********************************************                         **********************************************//
//**********************************************   BINARY SEARCH TREE    **********************************************//
//**********************************************                         **********************************************//
//*********************************************************************************************************************//

/*
p_tree insert_ABR_by_ID(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( ((ID_Pair*)get_element(t)) -> id >= ((ID_Pair*)element) -> id){
        t -> left_son = insert_ABR_by_ID(t -> left_son , element);
    }

    else  if ( ((ID_Pair*)get_element(t)) -> id < ((ID_Pair*)element) -> id ){
        t -> right_son = insert_ABR_by_ID(t -> right_son , element);
    }

    return t;
}

p_tree insert_ABR_by_dates(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( ((Date_Pair*)get_element(t)) -> date >= ((Date_Pair*)element) -> date){
        t -> left_son = insert_ABR_by_dates(t -> left_son , element);
    }

    else  if ( ((Date_Pair*)get_element(t)) -> date < ((Date_Pair*)element) -> date ){
        t -> right_son = insert_ABR_by_dates(t -> right_son , element);
    }

    return t;
}

p_tree insert_ABR_by_data(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( ((Data_Pair*)get_element(t)) -> data >= ((Data_Pair*)element) -> data){
        t -> left_son = insert_ABR_by_data(t -> left_son , element);
    }

    else  if ( ((Data_Pair*)get_element(t)) -> data < ((Data_Pair*)element) -> data ){
        t -> right_son = insert_ABR_by_data(t -> right_son , element);
    }

    return t;
}

p_tree remove_min(p_tree t , void** element){

    p_tree p;

    if(has_left_son(t)){
        t -> left_son = remove_min(t -> left_son , element);
    }

    else{
        *element = t -> element;
        p = t;
        t = t -> right_son;
        free(p);
    }

    return t;
}

/*
p_tree remove_element_ABR(p_tree t , void* element){
    
    if(t == NULL){
        return t;
    }

    else if (strcmp(sort_method , "ID") == 0 ){    
    
        if ((t -> element).ID >= element.ID){
            t -> left_son = remove_element_ABR(t -> left_son , element , sort_method);
        }

        else  if ((t -> element).ID < element.ID){
            t -> right_son = remove_element_ABR(t -> right_son , element , sort_method);
        }

        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
        }
    }
    else if (strcmp(sort_method , "date") == 0){    
    
        if (compare_dates((t -> element).date , element.date) < 0){
            t -> left_son = remove_element_ABR(t -> left_son , element , sort_method);
        }

        else  if (compare_dates((t -> element).date , element.date) > 0){
            t -> right_son = remove_element_ABR(t -> right_son , element , sort_method);
        }

        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
        }
    }
    else if (strcmp(sort_method , "data") == 0){    
    
        if ((t -> element).data >= element.data){
            t -> left_son = remove_element_ABR(t -> left_son , element , sort_method);
        }

        else  if ((t -> element).data < element.data){
            t -> right_son = remove_element_ABR(t -> right_son , element , sort_method);
        }

        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
        }
    }

    return t; 
}

bool is_ABR(p_tree t , char* type){
    
    if (t == NULL){
        printf("ERROR");
        exit(1);
    }

    else if(strcmp(type , "id")){
        if(has_left_son(t)){

            if((t -> element).id < ((t -> left_son) -> element).id){
                return false;
            }
            else{
                return is_ABR(t -> left_son , type);
            }
        }

        if (has_right_son(t)){  

            if ((t -> element).id > ((t -> right_son) -> element).id){
                return false;
            }
            else{
                return is_ABR(t -> right_son , type);
            }
        }

        return true;
    }

    else if(strcmp(type , "date")){
        if(has_left_son(t)){

            if(compare_dates((t -> element).date , ((t -> left_son) -> element).date) < 0){
                return false;
            }
            else{
                return is_ABR(t -> left_son , type);
            }
        }

        if (has_right_son(t)){  

            if (compare_dates((t -> element).date , ((t -> right_son) -> element).date) > 0){
                return false;
            }
            else{
                return is_ABR(t -> right_son , type);
            }
        }

        return true;       
    }

    else if(strcmp(type , "data")){
        if(has_left_son(t)){

            if((t -> element).data < ((t -> left_son) -> element).data){
                return false;
            }
            else{
                return is_ABR(t -> left_son , type);
            }
        }

        if (has_right_son(t)){  

            if ((t -> element).data > ((t -> right_son) -> element).data){
                return false;
            }
            else{
                return is_ABR(t -> right_son , type);
            }
        }
        return true;       
    }
}
*/

//*******************************************************************************************************************//
//*********************************************                        **********************************************//
//*********************************************        AVL TREE        **********************************************//    
//*********************************************                        **********************************************//
//*******************************************************************************************************************//

/*
int get_balance_factor(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    else{
        return( get_height(t -> right_son) - get_height(t -> left_son));
    }
}

p_tree left_rotation(p_tree t){

    p_tree pivot = t -> right_son;
    p_tree x = pivot -> left_son;

    pivot -> left_son = t;
    t -> right_son = x;

    t -> height = get_height(t);
    pivot -> height = get_height(pivot);

    t -> equilibre = get_balance_factor(pivot);
    pivot -> equilibre = get_balance_factor(pivot);
    
    return pivot;
}

p_tree right_rotation(p_tree t){
    
    p_tree pivot = t -> left_son;
    p_tree x = pivot -> right_son;

    pivot -> right_son = t;
    t -> left_son = x;

    t -> height = get_height(t);
    pivot -> height = get_height(pivot);

    t -> equilibre = get_balance_factor(pivot);
    pivot -> equilibre = get_balance_factor(pivot);

    return pivot;
}

p_tree double_left_rotation(p_tree t){

    t -> right_son = right_rotation(t -> right_son);
    return left_rotation(t);

}

p_tree double_right_rotation(p_tree t){

    t -> left_son = left_rotation(t -> left_son);
    return right_rotation(t);

}

p_tree balance_AVL(p_tree t){

    if(t -> equilibre >= 2){
        if((t -> right_son) -> equilibre >= 0){
            return left_rotation(t);
        }
        else{
            return double_left_rotation(t);
        }
    }
    else if (t -> equilibre <= -2){
        if((t -> left_son) -> equilibre <= 0){
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

p_tree insert_AVL_by_ID(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( *((int*)get_element(t)) > *((int*)element) ){
        t -> left_son = insert_AVL_by_ID(t -> left_son , element);
    }

    else  if ( *((int*)get_element(t)) < *((int*)element) ){
        t -> right_son = insert_AVL_by_ID(t -> right_son , element);
    }
    else{
        return t;
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree insert_AVL_by_dates(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( ((Date_Pair*)get_element(t)) -> date >= ((Date_Pair*)element) -> date){
        t -> left_son = insert_AVL_by_dates(t -> left_son , element);
    }

    else  if ( ((Date_Pair*)get_element(t)) -> date < ((Date_Pair*)element) -> date ){
        t -> right_son = insert_AVL_by_dates(t -> right_son , element);
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree insert_AVL_by_datas(p_tree t , void* element) {

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if ( ((Data_Pair*)get_element(t)) -> data >= ((Data_Pair*)element) -> data){
        t -> left_son = insert_AVL_by_datas(t -> left_son , element);
    }

    else  if ( ((Data_Pair*)get_element(t)) -> data < ((Data_Pair*)element) -> data){
        t -> right_son = insert_AVL_by_datas(t -> right_son , element);
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

/*
p_tree remove_min_AVL(p_tree t , void* p_element){

    if(has_left_son(t)){
        t -> left_son = remove_min_AVL(t -> left_son , p_element);
    }

    else{
        p_tree p = NULL; // !!! changement !!!
        *p_element = t -> element;
        p = t;
        t = t -> right_son;
        free(p);
    }

    return t;
}

p_tree remove_element_AVL(p_tree t , void* element , char* sort_method){

if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (strcmp(sort_method , "ID") == 0 ){    
    
        if ((t -> element).ID >= element.ID){
            t -> left_son = remove_element_AVL(t -> left_son , element , sort_method);
        }

        else  if ((t -> element).ID < element.ID){
            t -> right_son = remove_element_AVL(t -> right_son , element , sort_method);
        }
        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
            return t;
        }
    }
    else if (strcmp(sort_method , "date") == 0){    
    
        if (compare_dates((t -> element).date , element.date) < 0){
            t -> left_son = remove_element_AVL(t -> left_son , element , sort_method);
        }

        else  if (compare_dates((t -> element).date , element.date) > 0){
            t -> right_son = remove_element_AVL(t -> right_son , element , sort_method);
        }
        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
            return t;
        }
    }
    else if (strcmp(sort_method , "data") == 0){    
    
        if ((t -> element).data >= element.data){
            t -> left_son = remove_element_AVL(t -> left_son , element , sort_method);
        }

        else  if ((t -> element).data < element.data){
            t -> right_son = remove_element_AVL(t -> right_son , element , sort_method);
        }
        else{
            p_tree tmp = t;
            t = t -> left_son;
            free(tmp);
            tmp = NULL;
            return t;
        }
    }

    int balance = get_balance_factor(t);
    t -> equilibre = balance;
    t = balance_AVL(t);

    return t;
}

void print_AVL(p_tree t , int space){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%s , eq : %d]\n" , get_element(p) , p -> equilibre);

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
*/