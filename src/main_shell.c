#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

int glb_lvl = 0;
TREE *tree;
TREE *test_tree;

static int
display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf);
static int
append_tree(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf);
int
printfn (NODE * node);

int
set_size (NODE * node);

int
main(int argc, char *argv[])
{
    test_tree = make_test_tree();
    tree = make_tree();
    int flags = 0;

    if (argc > 2 && strchr(argv[2], 'd') != NULL)
        flags |= FTW_DEPTH;
    if (argc > 2 && strchr(argv[2], 'p') != NULL)
        flags |= FTW_PHYS;

    if (nftw((argc < 2) ? "." : argv[1], append_tree, 20, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }

    traverse_post(tree, set_size);
    traverse_pre(tree, printfn);
    exit(EXIT_SUCCESS);
}

static int
display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    printf("%-3s %2d %7jd   %-40s %d %s\n",
        (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
        (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
        (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
        (tflag == FTW_SLN) ? "sln" : "???",
        ftwbuf->level, (long long) sb->st_size,
        fpath, ftwbuf->base, fpath + ftwbuf->base);
    return 0;           /* To tell nftw() to continue */
}

static int
append_tree(const char *fpath, const struct stat *sb,
            int tflag, struct FTW *ftwbuf)
{
    char *name = strcat(fpath + ftwbuf->base, "\0");
    char *name2 = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(name2, name);
    NODE *node  = make_node(tflag,
                            ftwbuf->level, 
                            sb->st_size, 
                            name2);

    if (ftwbuf->level > glb_lvl || tree->root == NULL) {
        add_child(node, tree);
    } else if (ftwbuf->level == glb_lvl) {
        tree->current = tree->current->parent;
        add_child(node, tree);
    } else if (ftwbuf->level < glb_lvl) {
        tree->current = tree->current->parent->parent;
        add_child(node, tree);
    }
    
    tree->current = node;
    glb_lvl = ftwbuf->level;

    return 0;
}

int
printfn (NODE * node)
{
    for (int i = 0; i < node->level; i++)
        printf("-");
    printf("%s__%iB\n", node->name, node->size);
    
    return 0;
}

int 
set_size(NODE *node)
{
    for (int i = 0; i < node->child_count; i++)
        node->size += node->childs[i]->size;
    
    return 0;
}