#ifndef         __TREE_H__
#define         __TREE_H__

#include        "dll.h"

typedef struct  s_tree  t_tree;

struct  s_tree
{
  void  *data;
  t_dll *children;
};

t_tree          *tree_create(void *data);
void            tree_delete(t_tree *tree);
t_tree          *tree_add_child(t_tree *parent, void *data);

#endif
