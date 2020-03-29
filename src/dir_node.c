#include "stdlib.h"
#include "dir_node.h"

NODE *
make_node(
    char type,
    unsigned short level,
    long long size,
    const char *name)
{
    NODE *temp = (NODE *)malloc(sizeof(NODE));
    temp->type = type;
    temp->level = level;
    temp->size = size;
    temp->name = name;
    
    temp->parent = NULL;
    temp->childs = (NODE **)malloc(AVG_FILE_NUM * sizeof(NODE *));
    temp->child_count = 0;
    temp->child_cap = AVG_FILE_NUM;
    return temp;
}