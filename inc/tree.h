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
make_tree();

TREE *
make_test_tree();

void
add_child(NODE *node, TREE *tree);

static int
traverse_pre_helper(NODE *root, int (*fn)(NODE *));

int
traverse_pre(TREE *tree, int (*fn)(NODE *));

static int
traverse_post_helper(NODE *root, int (*fn)(NODE *));

int
traverse_post(TREE *tree, int (*fn)(NODE *));

#endif // _TREE_H