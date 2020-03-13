#ifndef _TREE_H
#define _TREE_H

#include "node_dir.h"
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
        node->parent = tree->current->parent;
    }
}

#endif