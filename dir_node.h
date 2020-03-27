#ifndef _DIR_NODE_H
#define _DIR_NODE_H

#include "stdlib.h"
#define NODE struct dir_node
#define AVG_FILE_NUM 4

struct dir_node
{
    unsigned char type;
    unsigned short level;
    long long size;
    const char *name;

    NODE *parent;
    NODE **childs;
    unsigned short child_count;
    unsigned short child_cap;
};

NODE *
make_node(
    char type,
    short level,
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

#endif  // _DIR_NODE_H