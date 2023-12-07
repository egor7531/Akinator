#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreeDump.h"

void get_conection_nodes(const Tree* tree, const TreeNode* node, FILE* fp)
{
    assert(fp != nullptr);
    assert(tree != nullptr);
    assert(tree->root != nullptr);

    fprintf(fp, "node_%d [label = \"", node);
    tree->write_elem(fp, node->elem);
    fprintf(fp, "\"];\n");

    if(!node->leftNode)
        return;
    fprintf(fp, "node_%d -> node_%d;\n", node, node->leftNode);
    get_conection_nodes(tree, node->leftNode, fp);

    if(!node->rightNode)
        return;
    fprintf(fp, "node_%d -> node_%d;\n", node, node->rightNode);
    get_conection_nodes(tree, node->rightNode, fp);
}

void tree_system(const Tree* tree, const char* nameFileDot, const char* nameFilePng)
{
    assert(tree != nullptr);
    assert(nameFileDot != nullptr);
    assert(nameFilePng != nullptr);

    const int MAX_SIZE_COMMAND = 100;
    char command[MAX_SIZE_COMMAND] = "dot ";
    strncat(command, nameFileDot, MAX_SIZE_COMMAND);
    strncat(command, " -T png -o ", MAX_SIZE_COMMAND);
    strncat(command, nameFilePng, MAX_SIZE_COMMAND);

    if(system(command) > 0)
    {
        return;
    }
}

void tree_graphic_dump(const Tree * tree, const char* nameFileDot, const char* nameFilePng)
{
    assert(tree != nullptr);

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
    get_conection_nodes(tree, tree->root, fp);

    fprintf(fp, "}");
    fclose(fp);
    tree_system(tree, nameFileDot, nameFilePng);
}
