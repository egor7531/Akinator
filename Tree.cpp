#include <stdlib.h>

#include "Tree.h"

void tree_ctor(Tree *tree)
{
    tree->size = 0;
    tree->root = (TreeNode *)calloc(1, sizeof(TreeNode));

    if(tree->root == NULL)
    {
        printf("");
        abort();
    }
}

void * operator_new(const int count)
{
    void * ptr = calloc(count, sizeof(char));
    return ptr;
}
