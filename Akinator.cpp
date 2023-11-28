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
void define_object(const Tree *tree);
void compare_objects(const Tree *tree);

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
                guess_object(&tree);
                break;
            case 2:
                compare_objects(&tree);
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
    assert(buf != nullptr);
    assert(object != nullptr);

    for(int i = 0; *buf != '"'; i++)
    {
        object[i] = *buf;
        buf++;
    }
}

void read_buf_in_tree(TreeNode **node, char **buf)
{
    assert(buf != nullptr);

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
    while(getchar() != '\n') {;}
}

void get_answer(char *answer)
{
    assert(answer != nullptr);

    fgets(answer, MAX_SIZE_ANSWER, stdin);
    answer[strlen(answer) - 1] = 0;
}

void guess_object(Tree *tree)
{
    assert(tree != nullptr);
    assert(tree->root != nullptr);

    char answer[MAX_SIZE_ANSWER] = {};
    TreeNode *caront = tree->root;

    while(true)
    {
        if(caront->leftNode == nullptr && caront->rightNode == nullptr)
        {
            printf("It's \"%s\" ?\n", caront->data);
            get_answer(answer);

            if(!strcmp(answer, "yes"))
                printf("I guessed !\n");
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

void get_info_object(const TreeNode *node, Stack *features, const char *object, bool *beObject)
{
    assert(node != nullptr);
    assert(features != nullptr);
    assert(object != nullptr);
    assert(beObject != nullptr);

    if(!strcmp(node->data, object))
        *beObject = true;

    if(*beObject)
        return;

    if(node->leftNode != nullptr && node->rightNode != nullptr)
    {
        stack_push(features, 1);
        get_info_object(node->leftNode, features, object, beObject);

        if(*beObject)
            return;

        int x = 0;
        stack_pop(features, &x);
        stack_push(features, 0);
        get_info_object(node->rightNode, features, object, beObject);

        if(*beObject)
            return;

        x = 0;
        stack_pop(features, &x);
    }
}

void define_object(const Tree *tree)
{
    assert(tree != nullptr);
    assert(tree->root != nullptr);

    printf("Enter object you want to define: ");
    char answer[MAX_SIZE_ANSWER] = {};
    get_answer(answer);

    Stack features = {};
    stack_ctor(&features);
    bool beObject = false;
    get_info_object(tree->root, &features, answer, &beObject);

    if(!beObject)
    {
        printf("There is no such word\n");
        return;
    }

    printf("Definition of \"%s\": ", answer);
    TreeNode *caront = tree->root;
    for(int i = 0; i < features.sizeStack - 1; i++)
    {
        if(features.data[i])
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
    if(features.data[features.sizeStack - 1])
    {
        printf("%s\n", caront->data);
        caront = caront->leftNode;
    }
    else
    {
        printf("not %s\n", caront->data);
        caront = caront->rightNode;
    }
}

void compare_objects(const Tree *tree)
{
    assert(tree != nullptr);

    printf("Enter first object: ");
    char answer1[MAX_SIZE_ANSWER] = {};
    get_answer(answer1);
    Stack features1 = {};
    stack_ctor(&features1);
    bool beObject1 = false;
    get_info_object(tree->root, &features1, answer1, &beObject1);

    printf("Enter second object: ");
    char answer2[MAX_SIZE_ANSWER] = {};
    get_answer(answer2);
    Stack features2 = {};
    stack_ctor(&features2);
    bool beObject2 = false;
    get_info_object(tree->root, &features2, answer2, &beObject2);

    if(!beObject1)
    {
        printf("There is no such word: %s\n", answer1);
        return;
    }

    if(!beObject2)
    {
        printf("There is no such word: %s\n", answer2);
        return;
    }

    TreeNode *node = tree->root;
    int i = 0;
    for(; i < features1.sizeStack && i < features2.sizeStack &&
                        features1.data[i] == features2.data[i]; i++)
    {
        if(i == 0)
            printf("Your objects are similar in that ");

        if(features1.data[i])
        {
            printf("%s, ", node->data);
            node = node->leftNode;
        }
        else
        {
            printf("not %s, ", node->data);
            node = node->rightNode;
        }
    }
    printf("\n");

    TreeNode *node1 = node;
    for(int j = i; j < features1.sizeStack; j++)
    {
        if(i == j)
            printf("Your objects differ in that object \"%s\" is ", answer1);

        if(features1.data[j])
        {
            printf("%s, ", node1->data);
            node1 = node1->leftNode;
        }
        else
        {
            printf("not %s, ", node1->data);
            node1 = node1->rightNode;
        }
    }

    for(int j = i; j < features2.sizeStack; j++)
    {
        if(i == j)
            printf("but \"%s\" is ", answer2);

        if(features2.data[j])
        {
            printf("%s, ", node->data);
            node = node->leftNode;
        }
        else
        {
            printf("not %s, ", node->data);
            node = node->rightNode;
        }
    }
    printf("\n");
}
