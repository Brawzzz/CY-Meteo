#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include <string.h>

#include "Tree.h"

//*******************************************************************************************************************//
//*********************************************                         *********************************************//
//*********************************************           TREE          *********************************************//
//*********************************************                         *********************************************//
//*******************************************************************************************************************//

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

p_tree create_tree(float new){

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

bool is_sheet(p_tree t){

    if(is_empty(t)){
        printf("ERROR\n");
        exit(1);
    }  

    else if(t -> left_son == NULL && t -> right_son == NULL){
        return true;
    }

    else{
        return false;
    }
}

int nmb_sheet(p_tree t){
    
    if (is_empty(t)){
        return 0;
    } 
    if (is_sheet(t)){
        return 1;
    } 
    return nmb_sheet(t -> left_son) + nmb_sheet(t -> right_son);
}

bool has_left_son(p_tree t){
    
    if(is_empty(t)){
        printf("ERROR\n");
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
        printf("ERROR\n");
        exit(1);
    }  

    else if (t -> right_son == NULL){
        return false;
    }

    else{
        return true;
    }
}

int nmb_knots(p_tree t){

    if(is_empty(t)){
        return 0;
    }
    else if(is_sheet(t)){
        return 1;
    } 
    else{
        return 1 + nmb_knots(t -> left_son) + nmb_knots(t -> right_son); 
    }
}

int get_element(p_tree t){
    return t -> element;
}

int get_height(p_tree t){

    if (is_empty(t)){
        return 0;
    }
    
    else{
        return 1 + max(get_height(t -> left_son) , get_height(t -> right_son));
    }
}

bool add_left_son(p_tree t , float element){
    
    p_tree new = create_tree(element);

    if(is_empty(t)){
        t = new;
        return true;
    }

    else if (!has_left_son(t)){
        t -> left_son = new;  
        return true;
    }

    else{
        return false;
    }
}

bool add_right_son(p_tree t , float element){
    
    p_tree new = create_tree(element);

    if(is_empty(t)){
        t = new;
        return true;
    }

    else if (!has_right_son(t)){
        t -> right_son = new;
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

void print_tree(p_tree t , int j){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%f]\n" , p -> element);

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
        printf("ERROR");
        exit(1);
    }

    else{

        fprintf(file , "%f\n" , t -> element);

        if(has_left_son(t)){
            pre_fixe_search(t -> left_son , file);
        }

        if(has_right_son(t)){
            pre_fixe_search(t -> right_son , file);
        }
    }
}

void in_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("ERROR");
        exit(1);
    }
    
    else{

        if(has_left_son(t)){
            in_fixe_search(t -> left_son , file);
        }

        fprintf(file , "%f\n" , t -> element);

        if(has_right_son(t)){
            in_fixe_search(t -> right_son , file);
        }
    }
}

void post_fixe_search(p_tree t , FILE* file){

    if (t == NULL){
        printf("ERROR");
        exit(1);
    }

    else{

        if(has_left_son(t)){
            post_fixe_search(t -> left_son , file);
        }

        if(has_right_son(t)){
            post_fixe_search(t -> right_son , file);
        }

        fprintf(file , "%f\n" , t -> element);
    }
}

void make_tab_with_Tree(p_tree t , float* tab , int* range){

    if (is_empty(t)){
        printf("L'abre est vide");
    }

    else{
    
        if(has_left_son(t)){
            make_tab_with_Tree(t -> left_son , tab , range);
        }

        *(tab + *range) = t -> element;
        *range += 1;

        if(has_right_son(t)){
            make_tab_with_Tree(t -> right_son , tab , range);
        }
    }
}

bool arbre_filiforme(p_tree t){
    
    if (is_empty(t)){
        return false;
    }

    else{
        if (nmb_sheet(t) == 1){
            return true;
        }
        else{
            return false;
        }
    }
}

//*********************************************************************************************************************//
//**********************************************                         **********************************************//
//**********************************************   BINARY SEARCH TREE    **********************************************//
//**********************************************                         **********************************************//
//*********************************************************************************************************************//

bool search(p_tree t , float element , int* nmb_knots){

    bool is_in = false;

    if (is_empty(t)){
        printf("L'arbre est vide");
        is_in = false;
    }

    else if(t -> element == element){
        *nmb_knots += 1;
        is_in = true;
    }

    else if(t -> element > element){
        *nmb_knots += 1;
        if(has_left_son(t)){
            return search(t -> left_son , element , nmb_knots);
        }
    }

    else if(t -> element < element){
        *nmb_knots += 1;
        if(has_right_son(t)){
            return search(t -> right_son , element , nmb_knots);
        }
    }
    return is_in;
}

p_tree insert_ABR(p_tree t , float element){

    if (t == NULL){
        t = create_tree(element);
        return t;
    }

    else if (t -> element >= element){
        t -> left_son = insert_ABR(t -> left_son , element);
    }

    else  if (t -> element < element){
        t -> right_son = insert_ABR(t -> right_son , element);
    }

    return t;
}

p_tree remove_min(p_tree t , float* element){

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

p_tree remove_element_ABR(p_tree t , float element){
    
    if(t == NULL){
        return t;
    }

    else if (element < t -> element){
        t -> left_son = remove_element_ABR(t -> left_son , element);
    }

    else if (element > t -> element){
        t -> right_son = remove_element_ABR(t -> right_son , element);
    }

    else if(has_right_son(t)){
        t -> right_son = remove_min(t -> right_son , &(t -> element));
    }

    else{
        p_tree tmp = t;
        t = t -> left_son;
        free(tmp);
        tmp = NULL;
    }

    return t; 
}

bool is_ABR(p_tree t){
    
    if (t == NULL){
        printf("ERROR");
        exit(1);
    }

    else{

        if(has_left_son(t)){

            if(t -> element < (t -> left_son) -> element){
                return false;
            }
            else{
                return is_ABR(t -> left_son);
            }
        }

        if (has_right_son(t)){  

            if (t -> element > (t -> right_son) -> element){
                return false;
            }
            else{
                return is_ABR(t -> right_son);
            }
        }

        return true;
    }
}

p_tree make_ABR_with_tab(p_tree t , float* tab , int size){

    for (int i = 0 ; i < size-1 ; i++){
        t = insert_ABR(t , tab[i]);
    }
    return t;
}

p_tree convert_to_ABR(p_tree t , int nmb_knots){

    p_tree p = NULL;
    
    float* tab = malloc(nmb_knots * sizeof(int));
    if (tab == NULL){
        exit(1);
    }

    int range = 0;
    
    if (is_empty(t)){
        printf("L'arbre est vide\n");
        return t;
    }

    else if(is_ABR(t)){
        printf("L'arbre est deja un ABR\n");
        return t;
    }

    else{
        make_tab_with_Tree(t , tab , &range);
        
        for (int i = 0 ; i < nmb_knots ; i++){
           p = insert_ABR(p , tab[i]); 
        }

        return p;
    }
}

void sort_tab_with_tree(float* tab , int* nmb_knots , p_tree t){
    if (t == NULL){
        printf("ERROR");
        exit(1);
    }

    else{

        if(has_left_son(t)){
            sort_tab_with_tree(tab , nmb_knots , t -> left_son);
        }

        *(tab + *nmb_knots) = t -> element;
        *nmb_knots += 1;

        if(has_right_son(t)){
            sort_tab_with_tree(tab , nmb_knots , t -> right_son);
        }
    }
}

//*******************************************************************************************************************//
//*********************************************                        **********************************************//
//*********************************************        AVL TREE        **********************************************//    
//*********************************************                        **********************************************//
//*******************************************************************************************************************//

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

p_tree insert_AVL(p_tree t , float key) {
    // Si l'arbre est vide, retourne une nouvelle node
    if (t == NULL) {
        return create_tree(key);
    }
 
    // Si la clé à insérer est inférieure à la clé de la racine,
    // insère la nouvelle node dans le sous-arbre gauche
    if (key <= t -> element) {
        t -> left_son = insert_AVL(t -> left_son , key);
    }
 
    // Si la clé à insérer est supérieure à la clé de la racine,
    // insère la nouvelle node dans le sous-arbre droit
    else if (key > t -> element) {
        t -> right_son = insert_AVL(t -> right_son , key);
    }
 
    // Calcul de l'équilibre de la racine
    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree remove_min_AVL(p_tree t , float* p_element , int* h){

    p_tree tmp = NULL;

    if(t -> left_son != NULL){
        t -> left_son = remove_min_AVL(t -> left_son , p_element , h);
        *h = -*h;
    }

    else{
        *p_element = t -> element;
        *h = -1;
        tmp = t;
        t = t -> right_son;
        free(tmp);
        tmp = NULL;
        return t;
    }

    if(*h != 0){
        t -> equilibre = t -> equilibre + *h;
        if(t -> equilibre == 0){
            *h = -1;
        }
        else{
            *h = 0;
        }
    }
    return t;
}

p_tree remove_element_AVL(p_tree t , float element , int* h){

    p_tree tmp;

    if(t == NULL){
        return t;
    }

    else if (element > t -> element){
        t -> right_son = remove_element_AVL(t -> right_son , element , h);
        *h = 1;
    }

    else if (element < t -> element){
        t -> left_son = remove_element_AVL(t -> left_son , element , h);
        *h = -1;
    }

    else if(has_right_son(t)){
        t -> right_son = remove_min_AVL(t -> right_son , &(t -> element) , h);
    }

    else{
        tmp = t;
        t = t -> left_son;
        free(tmp);
        *h = -1; 
    }

    if(*h != 0){
        t -> equilibre = t -> equilibre + *h;
        if (t -> equilibre == 0){
            *h = -1;
        }
        else{
            *h = 0;
        }
    }

    return t; 
}

void print_AVL(p_tree t , int space){

    p_tree p = t;

    if(p == NULL){
        printf("abre vide\n");
    }
    else{

        printf("[%f , eq : %d]\n" , p -> element , p -> equilibre);

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
