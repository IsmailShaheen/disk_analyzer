#include "stdlib.h"
#include "stdio.h"
#include "string.h"
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

char *
get_size(NODE *node)
{
    double size = node->size;
    int i = 0;
    while (size / 1024 > 1) {
        size = size / 1024;
        i++;
    }

    int length = snprintf(NULL, 0, "%g", size);
    char *str = (char *)malloc(length + 4 );
    snprintf(str, length + 4, "%g", size);

    switch (i)
    {
    case 0:
        strcat(str, " B");
        break;
    case 1:
        strcat(str, " KB");
        break;
    case 2:
        strcat(str, " MB");
        break;
    case 3:
        strcat(str, " GB");
        break;
    case 4:
        strcat(str, " TB");
        break;
    case 5:
        strcat(str, " PB");
        break;
    default:
        break;
    }

    return str;
}
