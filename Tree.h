#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED

struct TreeNode
{
    void *elem;
    TreeNode *leftNode;
    TreeNode *rightNode;
};

struct Tree
{
    void* (*elem_ctor)(void* elem);
    void (*elem_dtor)(void* elem);
    void (*write_elem)(FILE* fp, void* elem);
    TreeNode* root;
    int size;
    int errors;
};

enum Tree_Errors
{
    NO_ERRORS    = 0 << 1,
    NODE_IS_NULL = 1 << 1
};

Tree* tree_ctor(void* (*elem_ctor)(void* data),
                void (*elem_dtor)(void* elem),
                void (*write_elem)(FILE* fp, void* elem));
void tree_dtor(Tree *tree);
TreeNode* tree_node_new(Tree* tree, void* elem);
void tree_link_node(TreeNode* node1, TreeNode* node2);
TreeNode* tree_node_insert(Tree* tree, TreeNode* node_parent, void* elem);

#endif //TREE_H_INCLUDED
