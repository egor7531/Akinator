#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Tree.h"
#include "TreeDump.h"
#include "File.h"
#include "Stack.h"
#include "StackDump.h"

const int MAX_SIZE_OBJECT = 20;
const int MAX_SIZE_ANSWER = 20;

void get_object(char *buf, char *object);
void read_buf_in_tree(TreeNode **node, char **buf);
void clear_buffer(void);
void get_answer(char *answer);
void guess_object(Tree *tree);
void get_info_object(const TreeNode *node, Stack *features, const char *object, bool *beObject);
void define_object(Tree *tree);
int main()
{
    Tree tree = {};
    tree_ctor(&tree);

    int fileSize = 0;
    const char *nameFile = "Akinator.txt";
    char *buf = get_file_content(nameFile, &fileSize);
    read_buf_in_tree(&tree.root, &buf);

    while(true)
    {
        int command = 0;
        printf( "Guess the object    - 1\n"
                "Compare two objects - 2\n"
                "Define an object    - 3\n"
                "Exit with saving    - 4\n"
                "Exit without saving - 5\n\n"
                "Enter command: " );
        scanf("%d", &command);
        clear_buffer();

        switch(command)
        {
            case 1:
            {
                guess_object(&tree);
                FILE *fp = fopen(nameFile, "wb");
                if(fp == nullptr)
                {
                    printf("Can't open file: %s\n", nameFile);
                    abort();
                }
                print_nodes(fp, tree.root);
                fclose(fp);
                break;
            }

            case 2:
                break;

            case 3:
                define_object(&tree);
                break;

            case 4:
            {
                tree_graphic_dump(&tree);
                if(system("dot dump.dot -T png -o dump.png") > 0)
                {
                printf("Error work of \"system\"\n");
                abort();
                }

                FILE *fp = fopen(nameFile, "wb");
                if(fp == nullptr)
                {
                    printf("Can't open file: %s\n", nameFile);
                    abort();
                }
                print_nodes(fp, tree.root);
                fclose(fp);
                break;
            }

            default: printf("Unknown command: %d\n", command);
                break;
        }

        if(command == 4 || command == 5)
            break;
    }

    tree_dtor(&tree);
    return 0;
}

void get_object(char *buf, char *object)
{
    #ifdef NDEBUG
    assert(buf != nullptr);
    assert(object != nullptr);
    #endif

    for(int i = 0; *buf != '"'; i++)
    {
        object[i] = *buf;
        buf++;
    }
}

void read_buf_in_tree(TreeNode **node, char **buf)
{
    #ifdef NDEBUG
    assert(buf != nullptr);
    #endif

    char object[MAX_SIZE_OBJECT] = {};
    sscanf(*buf, "%s", object);
    *buf += strlen(object) + 1;

    if(!strcmp(object, "("))
    {
        *node = (TreeNode *)calloc(sizeof(TreeNode), sizeof(char));
        if(*node == nullptr)
        {
            printf("Node is null\n");
            abort();
        }

        (*node)->data = (char *)calloc(MAX_SIZE_OBJECT, sizeof(char));
        if((*node)->data == nullptr)
        {
            printf("Data is null\n");
            abort();
        }

        (*buf)++;
        get_object(*buf, (*node)->data);

        *buf += strlen((*node)->data) + 2;
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

    read_buf_in_tree(&((*node)->leftNode), buf);
    read_buf_in_tree(&((*node)->rightNode), buf);
}

void clear_buffer(void)
{
    while(getchar() != '\n');
}

void get_answer(char *answer)
{
    #ifdef NDEBUG
    assert(answer != nullptr);
    #endif

    fgets(answer, MAX_SIZE_ANSWER, stdin);
    answer[strlen(answer) - 1] = 0;
}

void guess_object(Tree *tree)
{
    #ifdef NDEBUG
    assert(tree != nullptr);
    assert(tree->root != nullptr);
    #endif

    char answer[MAX_SIZE_ANSWER] = {};
    TreeNode *caront = tree->root;

    while(true)
    {
        if(caront->leftNode == nullptr && caront->rightNode == nullptr)
        {
            printf("It's \"%s\" ?\n", caront->data);
            get_answer(answer);

            if(!strcmp(answer, "yes"))
                printf("I guessed !!!\n");
            else
            {
                caront->leftNode = (TreeNode *)calloc(sizeof(char), sizeof(TreeNode));
                if(caront->leftNode == nullptr)
                    abort();

                caront->leftNode->data = (char *)calloc(MAX_SIZE_ANSWER, sizeof(char));
                if(caront->leftNode->data == nullptr)
                    abort();

                caront->rightNode = (TreeNode *)calloc(sizeof(char), sizeof(TreeNode));
                if(caront->rightNode == nullptr)
                    abort();

                caront->rightNode->data = (char *)calloc(MAX_SIZE_ANSWER, sizeof(char));
                if(caront->rightNode->data == nullptr)
                    abort();

                printf("Who did you wish for ?\n");
                get_answer(answer);

                strcpy(caront->leftNode->data, answer);
                strcpy(caront->rightNode->data, caront->data);

                printf("How is \"%s\" different from \"%s\" ?\n", caront->data, answer);
                get_answer(answer);

                strcpy(caront->data, answer);

                printf("OK\n");
            }
            break;
        }
        else
        {
            printf("%s ?\n", caront->data);
            get_answer(answer);

            if(!strcmp(answer, "yes"))
                caront = caront->leftNode;
            else
                caront = caront->rightNode;
        }
    }
}

void get_info_object(const Tree *tree, const TreeNode *node, Stack *features, const char *object, bool *beObject)
{
    #ifdef NDEBUG
    assert(tree != nullptr);
    assert(node != nullptr);
    assert(features != nullptr);
    assert(object != nullptr);
    assert(beObject != nullptr);
    #endif

    if(!strcmp(node->data, object))
    {
        *beObject = true;

        TreeNode *caront = tree->root;
        printf("Your object \"%s\" has this definition: ", object);
        for(int i = 0; i < features->sizeStack - 1; i++)
        {
            if(features->data[i])
            {
                printf("%s, ", caront->data);
                caront = caront->leftNode;
            }
            else
            {
                printf("not %s, ", caront->data);
                caront = caront->rightNode;
            }
        }
        if(features->data[features->sizeStack - 1])
            printf("%s\n", caront->data);
        else
            printf("not %s\n", caront->data);
    }

    if(*beObject)
        return;

    if(node->leftNode != nullptr && node->rightNode != nullptr)
    {
        stack_push(features, 1);
        get_info_object(tree, node->leftNode, features, object, beObject);
        int x = 0;
        stack_pop(features, &x);

        if(*beObject)
            return;

        stack_push(features, 0);
        get_info_object(tree, node->rightNode, features, object, beObject);
        x = 0;
        stack_pop(features, &x);
    }
}

void define_object(Tree *tree)
{
    #ifdef NDEBUG
    assert(tree != nullptr);
    assert(tree->root != nullptr);
    #endif

    printf("Enter object you want to define: ");
    char answer[MAX_SIZE_ANSWER] = {};
    get_answer(answer);

    Stack features = {};
    stack_ctor(&features);

    bool beObject = false;
    get_info_object(tree, tree->root, &features, answer, &beObject);

    if(!beObject)
        printf("There is no such word: %s\n", answer);
}
