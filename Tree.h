#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

typedef char * elem_t;

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
void tree_dtor(Tree *tree);
void add_tree_node(Tree *tree, const elem_t value);

#endif //TREE_H_INCLUDED
