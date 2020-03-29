#ifndef _DIR_NODE_H
#define _DIR_NODE_H

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
    const char *name);

#endif  // _DIR_NODE_H