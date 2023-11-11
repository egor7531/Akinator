#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

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

#endif //TREE_H_INCLUDED
