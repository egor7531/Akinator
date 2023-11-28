#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Tree.h"

void tree_ctor(Tree *tree)
{
    assert(tree != nullptr);

    tree->size = 0;
    /*tree->root = (TreeNode *)calloc(1, sizeof(TreeNode));
    tree->root->data = 10;
    if(tree->root == NULL)
        abort();*/
}

void tree_node_dtor(TreeNode * node)
{
    if(!node)
        return;

    tree_node_dtor(node->leftNode);
    tree_node_dtor(node->rightNode);
    free(node);
}

void tree_dtor(Tree *tree)
{
    tree_node_dtor(tree->root);
    tree->size = 0;
    tree->root = nullptr;
    tree = nullptr;
}

void add_tree_node(Tree *tree, elem_t value)
{
    assert(tree != nullptr);

    if(tree->root == nullptr)
    {
        tree->root = (TreeNode *)calloc(sizeof(char), sizeof(TreeNode));
        if(tree->root)
            abort();
        tree->root->data = value;
        return;
    }

    TreeNode* caront = tree->root;

    TreeNode* node = (TreeNode *)calloc(sizeof(char), sizeof(TreeNode));
    if(node)
        abort();
    node->data = value;

    while(true)
    {
        if(value <= caront->data)
        {
            if(caront->leftNode == nullptr)
            {
                caront->leftNode = node;
                break;
            }
            else
                caront = caront->leftNode;
        }
        else
        {
            if(caront->rightNode == nullptr)
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

