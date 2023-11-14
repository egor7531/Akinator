#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Tree.h"

void print_nodes(FILE *fp, const TreeNode * node)
{
    #ifdef DEBUG
    assert(fp != nullptr);
    #endif

    if(!node)
    {
        fprintf(fp, "nil ");
        return;
    }
    fprintf(fp, "( ");
    fprintf(fp, "\"%s\" ", node->data);
    print_nodes(fp, node->leftNode);
    print_nodes(fp, node->rightNode);
    fprintf(fp, ") ");
}

void get_free_node(const TreeNode * node, FILE * fp)
{
    #ifdef DEBUG
    assert(fp != nullptr);
    assert(node != nullptr);
    #endif

    fprintf(fp, "node_%d [label = \"%s\"];\n", node, node->data);

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
    assert(tree != nullptr);
    assert(tree->root != nullptr);
    #endif

    const char * nameFile = "Dump.dot";
    FILE * fp = fopen(nameFile, "wb");

    if(fp == nullptr)
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
