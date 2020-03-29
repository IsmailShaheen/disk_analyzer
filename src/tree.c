#include "stdlib.h"
#include "string.h"
#include "tree.h"

TREE *
make_tree()
{
    TREE *temp = (TREE *)malloc(sizeof(TREE));
    temp->root = NULL;
    temp->current = NULL;
    return temp;
}

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

void
add_child(NODE *node, TREE *tree)
{
    // checks for the stupid kcore file that is not human readable and reports
    // an invalid size (for physical memory representation)
    if (strcmp((const char *)node->name, "kcore") == 0)
        node->size = 0;
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