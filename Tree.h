#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

#define DEBUG

const int MAX_SIZE_OBJECT = 50;
typedef char * elem_t;

struct TreeNode
{
    elem_t data;
    TreeNode * leftNode;
    TreeNode * rightNode;
};

struct Tree
{
    TreeNode * root;
    int size;
};

void tree_ctor(Tree *tree);
void tree_dtor(Tree *tree);
void add_tree_node(Tree *tree, const elem_t value);

void * operator_new(const int count);
void operator_delete(void * ptr);

void read_buf_in_tree(TreeNode **node, char **buf);

#endif //TREE_H_INCLUDED
