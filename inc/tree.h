// General Tree with single root and variable number of childs
#ifndef _TREE_H
#define _TREE_H

#include "dir_node.h"
#define TREE struct tree

// The tree struct
struct tree
{
    NODE *root;
    NODE *current;
};

// Constructor for the tree
TREE *
make_tree();

// Constructor for a test tree used in early debugging stages
TREE *
make_test_tree();

// appends a childe to the node currently pointed at by current element
void
add_child(NODE *node, TREE *tree);

// pre order traversal for the tree, the passed function fn will be executed at each node
static int
traverse_pre_helper(NODE *root, int (*fn)(NODE *));

int
traverse_pre(TREE *tree, int (*fn)(NODE *));

// post order traversal for the tree, the passed function fn will be executed at each node
static int
traverse_post_helper(NODE *root, int (*fn)(NODE *));

int
traverse_post(TREE *tree, int (*fn)(NODE *));

#endif // _TREE_H