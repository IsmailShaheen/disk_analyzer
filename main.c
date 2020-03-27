#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "tree.h"
#include "test_tree.h"

int glb_lvl= 0;
TREE *tree;
TREE *test_tree;

static int
display_info(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    printf("%-3s %2d %7jd   %-40s %d %s\n",
        (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
        (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
        (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
        (tflag == FTW_SLN) ? "sln" : "???",
        ftwbuf->level, (intmax_t) sb->st_size,
        fpath, ftwbuf->base, fpath + ftwbuf->base);
    return 0;           /* To tell nftw() to continue */
}

static int
append_tree(const char *fpath, const struct stat *sb,
             int tflag, struct FTW *ftwbuf)
{
    NODE *node  = make_node(tflag,
                            ftwbuf->level, 
                            sb->st_size, 
                            fpath + ftwbuf->base);
    if (ftwbuf->level > glb_lvl)
        add_child(node, tree);
    
    if (ftwbuf->level = glb_lvl)
        add_child(node ->parent, tree);

    if (ftwbuf->level < glb_lvl) 
        add_child(node->parent->parent, tree);
    
    glb_lvl = ftwbuf->level;

    return 0;
}

int
printfn (NODE * node)
{
    for (int i = 0; i < node->level; i++)
        printf("    ");
    printf("%s\n", node->name);
    
    return 0;
}

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

    traverse_pre(test_tree, printfn);
    exit(EXIT_SUCCESS);
}