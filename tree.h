#ifndef _TREE_H
#define _TREE_H

#include "dir_node.h"
#define TREE struct tree

struct tree
{
    NODE *root;
    NODE *current;
};

TREE *
make_tree()
{
    TREE *temp = (TREE *)malloc(sizeof(TREE));
    temp->root = NULL;
    temp->current = NULL;
    return temp;
}

void
add_child(NODE *node, TREE *tree)
{
    if (tree->root == NULL) {
        tree->root = node;
        tree->current = node;
    } else {
        node->parent = tree->current;
        if (tree->current->child_count == tree->current->child_cap) {
            tree->current->childs = (NODE **)realloc(tree->current->childs, 2 * tree->current->child_cap * sizeof(NODE *));
            tree->current->child_cap *= 2;
        }
        tree->current->childs[tree->current->child_count++] = node;
    }
}

static int
traverse_pre_helper(NODE *root, int (*fn)(NODE *))
{
    if (root == NULL)
        return 0;
    
    int fn_ret = fn(root);
    if (fn_ret != 0)
        return fn_ret;
    
    for (int i = 0; i < root->child_count; i++) {
        traverse_pre_helper(root->childs[i], fn);
    }
    
    return 0;
}

int
traverse_pre(TREE *tree, int (*fn)(NODE *))
{
    return traverse_pre_helper(tree->root, fn);
}

static int
traverse_post_helper(NODE *root, int (*fn)(NODE *))
{
    if (root == NULL)
        return 0;
    
    for (int i = 0; i < root->child_count; i++) {
        traverse_post_helper(root->childs[i], fn);
    }
    
    int fn_ret = fn(root);
    if (fn_ret != 0)
        return fn_ret;
    
    return 0;
}

int
traverse_post(TREE *tree, int (*fn)(NODE *))
{
    return traverse_post_helper(tree->root, fn);
}

#endif // _TREE_H