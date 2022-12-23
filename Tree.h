#ifndef TREE_H
#define TREE_H

#include<stdbool.h>

typedef struct Tree Tree;
struct Tree {
    float element;
    Tree* left_son;
    Tree* right_son;
    int equilibre;
    int height;
};

typedef Tree* p_tree;

int max(int a , int b);

int min(int a , int b);

int Max(int a , int b , int c);

int Min(int a , int b , int c);

//******************************************************//
//***********************   TREE   *********************//
//******************************************************//
p_tree create_tree(float element);

int max(int a , int b);

bool is_empty(p_tree t);

bool is_leaf(p_tree t);

int nmb_leaf(p_tree t);

int get_element(p_tree t);

int get_height(p_tree a);

int nmb_nodes(p_tree t);

bool has_left_son(p_tree t);

bool has_right_son(p_tree t);

bool add_left_son(p_tree t , float element);

bool add_right_son(p_tree t , float element);

bool arbre_filiforme(p_tree t);

void delete_left_son(p_tree t);

void delete_right_son(p_tree t);

void print_tree(p_tree t , int j);

void pre_fixe_search(p_tree t , FILE* file);

void in_fixe_search(p_tree t , FILE* file);

void post_fixe_search(p_tree t , FILE* file);

void make_tab_with_Tree(p_tree t , float* tab , int* range);

//********************************************************************//
//***********************   BINARY SEARCH TREE   *********************//
//********************************************************************//

bool search(p_tree t , float element , int* nmb_nodes);

p_tree insert_ABR(p_tree t , float element);

bool is_ABR(p_tree t);

p_tree remove_min(p_tree t , float* element);

p_tree remove_element_ABR(p_tree t , float element);

p_tree convert_to_ABR(p_tree t , int nmb_nodes);

p_tree make_ABR_with_tab(p_tree t , float* tab , int size);

void sort_tab_with_tree(float* tab , int* nmb_nodes , p_tree t);

//********************************************************************//
//*******************************   AVL   ****************************//
//********************************************************************//

void print_AVL(p_tree t , int j);

int get_balance_factor(p_tree t);

p_tree insert_AVL(p_tree t , float element);

p_tree remove_min_AVL(p_tree t , float* element , int* h);

p_tree remove_element_AVL(p_tree t , float element , int* h);

p_tree left_rotation(p_tree t);

p_tree right_rotation(p_tree t);

p_tree balance_AVL(p_tree t);

#endif