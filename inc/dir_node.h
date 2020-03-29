// Node struct to be used by the tree struct
#ifndef _DIR_NODE_H
#define _DIR_NODE_H

#define NODE struct dir_node
#define AVG_FILE_NUM 4

// the node struct
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

// constructor for the node
NODE *
make_node(
    char type,
    unsigned short level,
    long long size,
    const char *name);

// returns the size as a c string while appending the appropriate suffix eg. MB, GB
char *
get_size(NODE *node);

#endif  // _DIR_NODE_H