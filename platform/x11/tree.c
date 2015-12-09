#include        <stdlib.h>
#include        "tree.h"

t_tree          *tree_create(void *data)
{
  t_tree        *tree = malloc(sizeof(t_tree));

  if (!tree)
    exit(1);
  tree->data = data;
  tree->children = NULL;
  return tree;
}

void            tree_delete(t_tree *tree)
{
  t_dll_node    *node;

  if (tree->children)
    {
      for (node = tree->children->begin; node; node = node->next)
        tree_delete((t_tree *) node->data);
      dll_delete(tree->children);
    }
  free(tree);
}

t_tree          *tree_add_child(t_tree *parent, void *data)
{
  t_tree        *child = tree_create(data);

  if (!parent->children)
    parent->children = dll_create();
  dll_push_back(parent->children, child);
  return child;
}
