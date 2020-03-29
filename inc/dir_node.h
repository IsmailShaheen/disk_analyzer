#ifndef _DIR_NODE_H
#define _DIR_NODE_H

#define NODE struct dir_node
#define AVG_FILE_NUM 4

struct dir_node
{
    char type;
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
    unsigned short level,
    long long size,
    const char *name);

char *
get_size(NODE *node);

#endif  // _DIR_NODE_H