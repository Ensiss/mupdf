#include        "hints.h"

static void     searchTree(t_tree *t, char *base, int basesz, t_dll *out, fz_link **link)
{
  t_dll_node    *node;
  t_hint        *hint;
  char          c;

  if (t->data)
    {
      c = (char) (long) t->data;
      base[basesz++] = c + ('A' - 'a') * (c >= 'a' && c <= 'z');
      base[basesz] = '\0';
    }
  if (!t->children || !t->children->size)
    {
      if (t->data)
        {
          if (!(hint = malloc(sizeof(t_hint))))
            exit(1);
          strcpy(hint->hint, base);
          hint->link = *link;
          *link = (*link)->next;
          dll_push_back(out, hint);
        }
    }
  else
    {
      for (node = t->children->begin; node; node = node->next)
        searchTree((t_tree *) node->data, base, basesz, out, link);
    }
  if (t->data)
    base[--basesz] = '\0';
}

static t_tree   *genTree(fz_link *page_links, char *hintchars)
{
  t_tree        *tree = tree_create(NULL);
  t_dll         *queue = dll_create();
  int           hintcharsz = strlen(hintchars);
  fz_link       *link;
  t_tree        *parent, *node;
  t_dll         *children;

  dll_push_back(queue, tree);
  for (link = page_links; link; link = link->next)
    {
      parent = (t_tree *) queue->begin->data;
      children = parent->children;

      if (children && children->size == hintcharsz)
        {
          dll_pop_front(queue);
          node = tree_add_child((t_tree *)children->begin->data, (void *) (long) hintchars[0]);
          dll_push_back(queue, node);
          parent = (t_tree *) queue->begin->data;
          children = parent->children;
        }
      node = tree_add_child(parent, (void *) (long) hintchars[children ? children->size : 0]);
      dll_push_back(queue, node);
    }
  dll_delete(queue);
  return tree;
}

t_dll           *computeHints(fz_link *page_links, char *hintchars)
{
  t_tree        *tree = genTree(page_links, hintchars);
  t_dll         *hints = dll_create();
  char          hintbuf[8];

  searchTree(tree, hintbuf, 0, hints, &page_links);
  tree_delete(tree);
  return hints;
}
