#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Tree.h"
#include "TreeDump.h"
#include "File.h"
#include "Stack.h"
#include "StackDump.h"

enum Command
{
    GUESS_OBJECT        = 1,
    DEFINE_OBJECT       = 2,
    COMPARE_TWO_OBJECTS = 3,
    EXIT_WITH_SAVING    = 4,
    EXIT_WITHOUT_SAVING = 5
};

const int MAX_SIZE_OBJECT = 20;
const int MAX_SIZE_ANSWER = 20;

const char *nameFile = "Akinator.txt";
const char* nameFileDot = "Dump.dot";
const char* nameFilePng = "Dump.png";

void* elem_ctor(void* elem);
void elem_dtor(void* elem);
void write_elem(FILE* fp, void* elem);

void write_BD(FILE* fp, const TreeNode* node);
void write_buf_in_tree(Tree* tree, TreeNode** node, char** buf);
void clear_buffer(void);
char* get_object(char** buf);
char* get_answer();
void fill_stack_features(const TreeNode* node, Stack* features, const char* object, bool* wasObject);
Stack* get_info_object(const Tree* tree, const char* object);

void guess_object(Tree* tree);
void define_object(const Tree* tree);
void compare_objects(const Tree* tree);
void save_tree(const Tree* tree, const char* nameFile);

int main()
{
    Tree* tree = tree_ctor(elem_ctor, elem_dtor, write_elem);

    int fileSize = 0;
    char* buf = get_file_content(nameFile, &fileSize);
    write_buf_in_tree(tree, &tree->root, &buf);

    while(true)
    {
        int command = 0;
        printf( "Guess the object    - %d\n"
                "Define an object    - %d\n"
                "Compare two objects - %d\n"
                "Exit with saving    - %d\n"
                "Exit without saving - %d\n\n"
                "Enter command: ",
                GUESS_OBJECT, DEFINE_OBJECT, COMPARE_TWO_OBJECTS,
                EXIT_WITH_SAVING, EXIT_WITHOUT_SAVING);
        scanf("%d", &command);
        clear_buffer();

        switch(command)
        {
            case GUESS_OBJECT:
                guess_object(tree);
                break;
            case DEFINE_OBJECT:
                define_object(tree);
                break;
            case COMPARE_TWO_OBJECTS:
                compare_objects(tree);
                break;
            case EXIT_WITH_SAVING:
                save_tree(tree, nameFile);
                break;
            case EXIT_WITHOUT_SAVING:
                break;
            default: printf("Unknown command: %d\n", command);
                break;
        }

        if(command == 4 || command == 5)
            break;
    }

    tree_dtor(tree);
    return 0;
}

void* elem_ctor(void* elem)
{
    assert(elem != nullptr);

    char* data = strdup((char*)elem);
    if(data == nullptr)
        return nullptr;

    return data;
}

void elem_dtor(void* elem)
{
    free(elem);
}

void write_elem(FILE* fp, void* elem)
{
    assert(elem != nullptr);

    fprintf(fp, "%s", (char*)elem);
}

void write_BD(FILE* fp, const TreeNode* node)
{
    assert(fp != nullptr);

    if(node == nullptr)
    {
        fprintf(fp, "nil ");
        return;
    }
    fprintf(fp, "( ");
    fprintf(fp, "\"");
    write_elem(fp, node->elem);
    fprintf(fp, "\" ");
    write_BD(fp, node->leftNode);
    write_BD(fp, node->rightNode);
    fprintf(fp, ") ");
}

void write_buf_in_tree(Tree* tree, TreeNode **node, char **buf)
{
    assert(tree != nullptr);
    assert(buf != nullptr);

    char object[MAX_SIZE_OBJECT] = {};
    sscanf(*buf, "%s", object);
    *buf += strlen(object) + 1;

    if(!strcmp(object, "("))
    {
        *node = tree_node_new(tree, get_object(&(++(*buf))));
        if(*node == nullptr)
        {
            printf("Node is null\n");
            return;
        }

        (*buf) += 2;
    }
    else
    {
        sscanf(*buf, "%s", object);

        while(!strcmp(object, ")"))
        {
            *buf += strlen(object) + 1;
            sscanf(*buf, "%s", object);
        }
        return;
    }

    write_buf_in_tree(tree, &((*node)->leftNode), buf);
    write_buf_in_tree(tree, &((*node)->rightNode), buf);
}

void clear_buffer(void)
{
    while(getchar() != '\n');
}

char* get_object(char** buf)
{
    assert(buf != nullptr);

    char* object = (char*)calloc(MAX_SIZE_OBJECT, sizeof(char));
    if(object == nullptr)
        return nullptr;

    for(int i = 0; **buf != '"'; i++)
    {
        object[i] = **buf;
        (*buf)++;
    }

    return object;
}

char* get_answer()
{
    char* answer = (char*)calloc(MAX_SIZE_ANSWER, sizeof(char));
    if(answer == nullptr)
        return nullptr;

    fgets(answer, MAX_SIZE_ANSWER, stdin);
    answer[strlen(answer) - 1] = 0;

    return answer;
}

void fill_stack_features(const TreeNode* node, Stack* features, const char* object, bool* wasObject)
{
    assert(node != nullptr);
    assert(features != nullptr);
    assert(object != nullptr);
    assert(wasObject != nullptr);

    if(node->leftNode != nullptr && node->rightNode != nullptr)
    {
        stack_push(features, 1);
        fill_stack_features(node->leftNode, features, object, wasObject);

        if(*wasObject)
            return;

        int x = 0;
        stack_pop(features, &x);
        stack_push(features, 0);
        fill_stack_features(node->rightNode, features, object, wasObject);

        if(*wasObject)
            return;

        x = 0;
        stack_pop(features, &x);
    }

    else if(!strcmp((char*)node->elem, object))
        *wasObject = true;
}

Stack* get_info_object(const Tree* tree, const char* object)
{
    assert(tree != nullptr);

    Stack* features = stack_ctor();
    if(features == nullptr)
        return nullptr;

    bool wasObject = false;
    fill_stack_features(tree->root, features, object, &wasObject);

    if(!wasObject)
    {
        printf("There is no such word: %s\n", object);
        return nullptr;
    }

    return features;
}

void guess_object(Tree* tree)
{
    assert(tree != nullptr);
    assert(tree->root != nullptr);

    TreeNode *node = tree->root;

    while(true)
    {
        if(node->leftNode == nullptr && node->rightNode == nullptr)
        {
            printf("It's \"%s\" ?\n", node->elem);
            char* answer = get_answer();
            if(!strcmp(get_answer(), "yes"))
                printf("I guessed !\n");
            else
            {
                printf("Who did you wish for ?\n");
                char* guess_object = get_answer();
                node->leftNode = tree_node_new(tree, guess_object);
                if(node->leftNode == nullptr)
                    return;
                node->rightNode = tree_node_new(tree, node->elem);
                if(node->rightNode == nullptr)
                    return;
                free(guess_object);

                char* feature = get_answer();
                printf("How is \"%s\" different from \"%s\" ?\n", node->elem, feature);
                strcpy((char*)node->elem, feature);
                free(feature);

                printf("OK\n");
            }
            free(answer);
            break;
        }
        else
        {
            printf("%s ?\n", node->elem);
            char* answer = get_answer();
            if(!strcmp(answer, "yes"))
                node = node->leftNode;
            else
                node = node->rightNode;
            free(answer);
        }
    }
}

void define_object(const Tree* tree)
{
    assert(tree != nullptr);
    assert(tree->root != nullptr);

    printf("Enter object you want define: ");
    char* answer = get_answer();
    Stack* features = get_info_object(tree, answer);
    if(features == nullptr)
        return;

    printf("Definition of \"%s\": ", answer);
    TreeNode *node = tree->root;
    for(int i = 0; i < features->sizeStack; i++)
    {
        if(features->data[i])
        {
            printf("%s, ", node->elem);
            node = node->leftNode;
        }
        else
        {
            printf("not %s, ", node->elem);
            node = node->rightNode;
        }
    }
    free(answer);
    free(features);
    printf("\n");
}

void compare_objects(const Tree* tree)
{
    assert(tree != nullptr);

    printf("Enter first object: ");
    char* answer1 = get_answer();
    Stack* features1 = get_info_object(tree, answer1);
    if(features1 == nullptr)
        return;

    printf("Enter first object: ");
    char* answer2 = get_answer();
    Stack* features2 = get_info_object(tree, answer2);
    if(features2 == nullptr)
        return;

    TreeNode *node = tree->root;
    int i = 0;
    for(; i < features1->sizeStack && i < features2->sizeStack &&
                        features1->data[i] == features2->data[i]; i++)
    {
        if(i == 0)
            printf("Your objects are similar in that ");

        if(features1->data[i])
        {
            printf("%s, ", node->elem);
            node = node->leftNode;
        }
        else
        {
            printf("not %s, ", node->elem);
            node = node->rightNode;
        }
    }
    printf("\n");

    TreeNode *node1 = node;
    for(int j = i; j < features1->sizeStack; j++)
    {
        if(i == j)
            printf("Your objects differ in that object \"%s\" is ", answer1);

        if(features1->data[j])
        {
            printf("%s, ", node1->elem);
            node1 = node1->leftNode;
        }
        else
        {
            printf("not %s, ", node1->elem);
            node1 = node1->rightNode;
        }
    }
    free(features1);

    for(int j = i; j < features2->sizeStack; j++)
    {
        if(i == j)
            printf("but \"%s\" is ", answer2);

        if(features2->data[j])
        {
            printf("%s, ", node->elem);
            node = node->leftNode;
        }
        else
        {
            printf("not %s, ", node->elem);
            node = node->rightNode;
        }
    }

    free(answer1);
    free(answer2);
    free(features2);
    printf("\n");
}

void save_tree(const Tree* tree, const char* nameFile)
{
    assert(tree != nullptr);

    tree_graphic_dump(tree, nameFileDot, nameFilePng);

    FILE *fp = fopen(nameFile, "wb");
    if(fp == nullptr)
    {
        fclose(fp);
        printf("Can't open file: %s\n", nameFile);
        return;
    }

    write_BD(fp, tree->root);
    fclose(fp);
}
