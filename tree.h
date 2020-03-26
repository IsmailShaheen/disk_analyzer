#ifndef _TREE_H
#define _TREE_H

#include "dir_node.h"
#define TREE struct tree

struct tree
{
    NODE *root;
    NODE *current;
};

TREE *make_tree()
{
    TREE *temp = (TREE *)malloc(sizeof(TREE));
    temp->root = NULL;
    temp->current = NULL;
    return temp;
}

void add_child(NODE *node, TREE *tree)
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

#endif