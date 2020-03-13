#ifndef _NODE_DIR_H
#define _NODE_DIR_H

#include "stdlib.h"
#define NODE struct node_dir

struct node_dir
{
    char type;
    short level;
    long long size;
    char *name;

    node_dir *parent;
    node_dir **childs;
};

NODE *make_node(
    char type,
    short level,
    long long size,
    char *name)
{
    NODE *temp = (NODE *)malloc(sizeof(NODE));
    temp->type = type;
    temp->level = level;
    temp->size = size;
    temp->name = name;
    temp->parent = NULL;
    temp->childs = NULL;
    return temp;
}

#endif  // _NODE_DIR_H