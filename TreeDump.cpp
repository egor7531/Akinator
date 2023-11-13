#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"

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

void get_free_node(const TreeNode * node, FILE * fp)
{
    #ifdef DEBUG
    assert(fp != NULL);
    assert(node != NULL);
    #endif

    fprintf(fp, "node_%d [label = \"%d\"];\n", node, node->data);

    if(!node->leftNode)
        return;
    fprintf(fp, "node_%d -> node_%d;\n", node, node->leftNode);
    get_free_node(node->leftNode, fp);

    if(!node->rightNode)
        return;
    fprintf(fp, "node_%d -> node_%d;\n", node, node->rightNode);
    get_free_node(node->rightNode, fp);
}

void tree_graphic_dump(const Tree * tree)
{
    #ifdef DEBUG
    assert(tree != NULL);
    assert(tree->root != NULL);
    #endif

    const char * nameFile = "Dump.dot";
    FILE * fp = fopen(nameFile, "wb");

    if(fp == NULL)
    {
        printf("Can't open file: %s\n", nameFile);
        fclose(fp);
        abort();
    }

    fprintf(fp, "digraph Tree\n"
                "{\n"
                "rankdir = TB;\n");

    get_free_node(tree->root, fp);

    fprintf(fp, "}");

    fclose(fp);
}
