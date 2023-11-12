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
    assert(fp != NULL);
    assert(node != NULL);

    fprintf(fp, "%d", node->data);
    if(!node->leftNode)
    {
        fprintf(fp, ";\n");
        return;
    }
    fprintf(fp, "->", node->data);
    get_free_node(node->leftNode, fp);

    fprintf(fp, "%d", node->data);
    if(!node->rightNode)
    {
        fprintf(fp, ";\n");
        return;
    }
    fprintf(fp, "->", node->data);
    get_free_node(node->rightNode, fp);
}

void tree_graphic_dump(const Tree * tree)
{
    assert(tree != NULL);
    assert(tree->root != NULL);

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