#ifndef TREE_H
#define TREE_H

#include "tools.h"

typedef struct Tree Tree;
struct Tree {
    Data_Set* element;
    Tree* left_son;
    Tree* right_son;
    int equilibre;
    int height;
};

typedef Tree* p_tree;

//******************************************************/
//***********************   TREE   *********************//
//******************************************************//

p_tree create_tree(Data_Set* element);

bool is_leaf(p_tree t);

bool has_left_son(p_tree t);

bool has_right_son(p_tree t);

Data_Set* get_element(p_tree t);

int get_height(p_tree t);

int nmb_nodes(p_tree t);

void in_fixe_search(p_tree t , FILE* file , int mode , int* c);

void reverse_in_fixe_search(p_tree t , FILE* file , int mode , int* c);

bool search_by_id(p_tree t , Data_Set* element);

bool search_by_date(p_tree t , Data_Set* element , int* nmb_knots);

bool binary_search_tree_id(p_tree array[] , int size , int value , int* index);

bool binary_search_tree_date(p_tree array[] , int size , char* value , int* index);

bool binary_search_tree_hour(p_tree* array[] , int size , char* value , int* index);

//********************************************************************/
//***********************   BINARY SEARCH TREE   *********************//
//********************************************************************//

p_tree insert_ABR_by_ID(p_tree t , Data_Set* element , int mode);

p_tree insert_ABR_by_date(p_tree t , Data_Set* element , int mode);

p_tree insert_ABR_by_max(p_tree t , Data_Set* element);

void insert_ABR_by_max_from_id(p_tree t1 , p_tree* t2);

void print_ABR(p_tree t , int space);

//********************************************************************/
//*******************************   AVL   ****************************//
//********************************************************************//

int get_balance_factor(p_tree t);

p_tree left_rotation(p_tree t);

p_tree right_rotation(p_tree t);

p_tree balance_AVL(p_tree t);

p_tree double_left_rotation(p_tree t);

p_tree double_right_rotation(p_tree t);

p_tree insert_AVL_by_ID(p_tree t , Data_Set* element , int* h , int mode);

p_tree insert_AVL_by_date(p_tree t , Data_Set* element , int mode , int* h);

p_tree insert_AVL_by_max(p_tree t , Data_Set* element , int* h);

void insert_AVL_by_max_from_id(p_tree t1 , p_tree* t2);

void insert_AVL_by_date_from_id(p_tree t1 , p_tree* t2);

void print_AVL(p_tree t , int space);

#endif
