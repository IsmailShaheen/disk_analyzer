#include "tree.h"

TREE *
make_test_tree()
{
    TREE *test_tree = make_tree();

    NODE *node1  = make_node(0, 0, 5, "node1");
    NODE *node2  = make_node(0, 1, 10, "node2");
    NODE *node3  = make_node(0, 1, 10, "node3");
    NODE *node4  = make_node(0, 2, 20, "node4");

    add_child(node1, test_tree);
    test_tree->current = node1;
    add_child(node2, test_tree);
    add_child(node3, test_tree);
    test_tree->current = node2;
    add_child(node4, test_tree);
    return test_tree;
}