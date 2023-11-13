#include <assert.h>
#include <stdlib.h>

#include "Tree.h"

void * operator_new(const int count)
{
    #ifdef DEBUG
    assert(count > 0);
    #endif

    void * ptr = calloc(count, sizeof(char));

    if(ptr == nullptr)
        abort();

    return ptr;
}
                                                //зачем нужны эти функции?
void operator_delete(void * ptr)
{
    #ifdef DEBUG
    assert(ptr != nullptr);
    #endif

    free(ptr);
}

void tree_ctor(Tree *tree)
{
    #ifdef DEBUG
    assert(tree != nullptr);
    #endif

    tree->size = 0;
    /*tree->root = (TreeNode *)calloc(1, sizeof(TreeNode));
    tree->root->data = 10;
    if(tree->root == NULL)
        abort();*/
}

void tree_node_dtor( TreeNode * node)
{
    if(!node)
        return;

    tree_node_dtor(node->leftNode);
    tree_node_dtor(node->rightNode);
    operator_delete(node);
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
    #ifdef DEBUG
    assert(tree != nullptr);
    #endif

    if(tree->root == nullptr)
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

