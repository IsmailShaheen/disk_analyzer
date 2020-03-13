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

    NODE *parent;
    NODE **childs;
    short child_count;
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
    NODE *temp_child = NULL;
    temp->childs = &temp_child;
    temp->child_count = 0;
    return temp;
}

#endif  // _NODE_DIR_H