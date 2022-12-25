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

bool add_left_son(p_tree t , double element){

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

bool add_right_son(p_tree t , double element){

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

        fprintf(file , "%lf\n" , t -> element);

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

        fprintf(file , "%lf\n" , t -> element);

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

        fprintf(file , "%lf\n" , t -> element);
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
        if (nmb_leaf(t) == 1){
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

bool search(p_tree t , double element , int* nmb_nodes){

    bool is_in = false;

    if (is_empty(t)){
        printf("L'arbre est vide");
        is_in = false;
    }

    else if(t -> element == element){
        *nmb_nodes += 1;
        is_in = true;
    }

    else if(t -> element > element){
        *nmb_nodes += 1;
        if(has_left_son(t)){
            return search(t -> left_son , element , nmb_nodes);
        }
    }

    else if(t -> element < element){
        *nmb_nodes += 1;
        if(has_right_son(t)){
            return search(t -> right_son , element , nmb_nodes);
        }
    }
    return is_in;
}

p_tree insert_ABR(p_tree t , double element){

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

p_tree remove_min(p_tree t , double* element){

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

p_tree remove_element_ABR(p_tree t , double element){
    
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

p_tree make_ABR_with_tab(p_tree t , double* tab , int size){

    for (int i = 0 ; i < size-1 ; i++){
        t = insert_ABR(t , tab[i]);
    }
    return t;
}

p_tree convert_to_ABR(p_tree t , int nmb_nodes){

    p_tree p = NULL;
    
    float* tab = malloc(nmb_nodes * sizeof(int));
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
        
        for (int i = 0 ; i < nmb_nodes ; i++){
           p = insert_ABR(p , tab[i]); 
        }

        return p;
    }
}

void sort_tab_with_tree(double* tab , int* nmb_nodes , p_tree t){
    if (t == NULL){
        printf("ERROR");
        exit(1);
    }

    else{

        if(has_left_son(t)){
            sort_tab_with_tree(tab , nmb_nodes , t -> left_son);
        }

        *(tab + *nmb_nodes) = t -> element;
        *nmb_nodes += 1;

        if(has_right_son(t)){
            sort_tab_with_tree(tab , nmb_nodes , t -> right_son);
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

p_tree insert_AVL(p_tree t , double element) {
    // Si l'arbre est vide, retourne une nouvelle node
    if (t == NULL) {
        return create_tree(element);
    }
 
    // Si la clé à insérer est inférieure à la clé de la racine,
    // insère la nouvelle node dans le sous-arbre gauche
    if (element <= t -> element) {
        t -> left_son = insert_AVL(t -> left_son , element);
    }
 
    // Si la clé à insérer est supérieure à la clé de la racine,
    // insère la nouvelle node dans le sous-arbre droit
    else if (element > t -> element) {
        t -> right_son = insert_AVL(t -> right_son , element);
    }
 
    // Calcul de l'équilibre de la racine
    int balance = get_balance_factor(t);
    t -> equilibre = balance;

    t = balance_AVL(t);

    return t;
}

p_tree remove_min_AVL(p_tree t , double* p_element){

    if(has_left_son(t)){
        t -> left_son = remove_min_AVL(t -> left_son , p_element);
    }

    else{
        p_tree p = NULL;
        *p_element = t -> element;
        p = t;
        t = t -> right_son;
        free(p);
    }

    return t;
}

p_tree remove_element_AVL(p_tree t , double element){

    if(t == NULL){
        return t;
    }

    else if (element > t -> element){
        t -> right_son = remove_element_AVL(t -> right_son , element);
    }

    else if (element < t -> element){
        t -> left_son = remove_element_AVL(t -> left_son , element);
    }

    else if(has_right_son(t)){
        t -> right_son = remove_min_AVL(t -> right_son , &(t -> element));    
    }

    else{
        p_tree tmp = t;
        t = t -> left_son;
        free(tmp);
        tmp = NULL;
        return t;
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
