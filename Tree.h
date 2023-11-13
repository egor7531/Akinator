#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define DEBUG

typedef int elem_t;

struct TreeNode
{
    elem_t data;
    TreeNode * leftNode;
    TreeNode * rightNode;
};

struct Tree
{
    TreeNode * root;
    int size;
};

void tree_ctor(Tree *tree);
void add_tree_node(Tree *tree, elem_t value);
void print_node(const TreeNode * node);
void tree_dtor(Tree *tree);

#endif //TREE_H_INCLUDED
