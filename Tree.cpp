#include <stdlib.h>
#include <stdio.h>

#include "Tree.h"

void tree_ctor(Tree *tree)
{
    tree->size = 0;
    /*tree->root = (TreeNode *)calloc(1, sizeof(TreeNode));
    tree->root->data = 10;
    if(tree->root == NULL)
        abort();*/
}

void * operator_new(const int count)
{
    void * ptr = calloc(count, sizeof(char));
    return ptr;
}

void add_tree_node(Tree *tree, elem_t value)
{
    if(tree->root == NULL)
    {
        tree->root = (TreeNode *)operator_new(sizeof(TreeNode));
        tree->root->data = value;
        return;
    }

    TreeNode* caront = tree->root;

    TreeNode* node = (TreeNode *)operator_new(sizeof(TreeNode));
    node->data = value;

    while(true)
    {
        if(value <= caront->data)
        {
            if(caront->leftNode == NULL)
            {
                caront->leftNode = node;
                break;
            }
            else
                caront = caront->leftNode;
        }
        else
        {
            if(caront->rightNode== NULL)
            {
                caront->rightNode = node;
                break;
            }
            else
            caront = caront->rightNode;
        }
    }
    tree->size++;
}

/*TreeNode * add_tree_node(TreeNode * node, elem_t value)
{
    if(node == NULL)
    {
        node = (TreeNode *)operator_new(sizeof(TreeNode));
        node->data = value;
        return node;
    }
    else if(value <= node->data)
        return add_tree_node(node->leftNode, value);
    else
        return add_tree_node(node->rightNode, value);
}*/

void print_node(const TreeNode * node)
{
    if(!node)
    {
        printf("nil ");
        return;
    }
    printf("( ");
    printf("%d ", node->data);
    print_node(node->leftNode);
    print_node(node->rightNode);
    printf(") ");
}
