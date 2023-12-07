#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Tree.h"

TreeNode* tree_node_new(Tree* tree, void* elem)
{
    assert(tree != nullptr);
    assert(elem != nullptr);

    TreeNode* node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if(node == nullptr)
        return nullptr;

    node->elem = tree->elem_ctor(elem);
    if(node->elem == nullptr)
        return nullptr;

    node->leftNode = nullptr;
    node->rightNode = nullptr;

    if(tree->size == 0)
        tree->root = node;

    tree->size++;
    return node;
}

void tree_node_dtor(Tree* tree, TreeNode* node)
{
    assert(tree != NULL);

    if(!node)
        return;

    tree_node_dtor(tree, node->leftNode);
    tree_node_dtor(tree, node->leftNode);
    tree->elem_dtor(node->elem);
    free(node);
}

Tree* tree_ctor(void* (*elem_ctor)(void* data),
                void (*elem_dtor)(void* elem),
                void (*write_elem)(FILE* fp, void* elem))
{
    assert(elem_ctor != nullptr);
    assert(elem_dtor != nullptr);
    assert(write_elem != nullptr);

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    if(tree == nullptr)
        return nullptr;

    tree->root = nullptr;
    tree->elem_ctor = elem_ctor;
    tree->elem_dtor = elem_dtor;
    tree->write_elem = write_elem;
    int size = 0;
    int errors = NO_ERRORS;

    return tree;
}

void tree_dtor(Tree* tree)
{
    assert(tree != NULL);

    tree_node_dtor(tree, tree->root);
    free(tree);
}

void tree_link_node(TreeNode* node1, TreeNode* node2)
{
    assert(node1 != nullptr);
    assert(node2 != nullptr);

    if(node1->leftNode != nullptr && node1->rightNode != nullptr)
        return;

    if(node1->leftNode == nullptr)
        node1->leftNode = node2;
    else
        node1->rightNode = node2;
}

TreeNode* tree_node_insert(Tree* tree, TreeNode* nodeParent, void* elem)
{
    assert(tree != nullptr);
    assert(elem != nullptr);

    if(tree->root == nullptr)
    {
        tree->root = tree_node_new(tree, elem);
        return tree->root;
    }

    TreeNode* nodeSon = tree_node_new(tree, elem);
    tree_link_node(nodeParent, nodeSon);

    return nodeSon;
}
