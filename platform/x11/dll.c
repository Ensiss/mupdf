#include        <stdlib.h>
#include        "dll.h"

static t_dll            *_pool = NULL;

static t_dll_node       *dll_node_create(void *data)
{
  t_dll_node    *node = malloc(sizeof(*node));

  if (!node)
    exit(1);
  node->prev = NULL;
  node->next = NULL;
  node->data = data;
  return node;
}

static void      dll_node_delete(t_dll_node *node, char freedata)
{
  if (freedata)
    free(node->data);
  free(node);
}

t_dll           *dll_create()
{
  t_dll         *dll = malloc(sizeof(*dll));

  if (!_pool)
    {
      _pool = (void *)1; // Avoid infinite recursion
      _pool = dll_create();
    }
  if (!dll)
    exit(1);
  dll->begin = NULL;
  dll->end = NULL;
  dll->size = 0;
  return dll;
}

void            dll_delete(t_dll *dll)
{
  t_dll_node    *it = dll->begin;
  t_dll_node    *next;

  while (it)
    {
      next = it->next;
      dll_node_delete(it, 0);
      it = next;
    }
  free(dll);
}

void            dll_delete_free_data(t_dll *dll)
{
  t_dll_node    *it = dll->begin;
  t_dll_node    *next;

  while (it)
    {
      next = it->next;
      dll_node_delete(it, 1);
      it = next;
    }
  free(dll);
}

void            dll_cleanup()
{
  if (_pool)
    dll_delete(_pool);
}

static void     dll_push_back_raw(t_dll *dll, t_dll_node *node)
{
  node->prev = NULL;
  node->next = NULL;

  if (!dll->begin)
    {
      dll->begin = node;
      dll->end = node;
    }
  else
    {
      dll->end->next = node;
      node->prev = dll->end;
      dll->end = node;
    }
  dll->size++;
}

static void     dll_push_front_raw(t_dll *dll, t_dll_node *node)
{
  node->prev = NULL;
  node->next = NULL;

  if (!dll->begin)
    {
      dll->begin = node;
      dll->end = node;
    }
  else
    {
      dll->begin->prev = node;
      node->next = dll->begin;
      dll->begin = node;
    }
  dll->size++;
}

static t_dll_node       *dll_pop_back_raw(t_dll *dll)
{
  t_dll_node            *node = dll->end;

  if (node)
    {
      if (node->prev)
        {
          node->prev->next = NULL;
          dll->end = node->prev;
        }
      else
        {
          dll->begin = NULL;
          dll->end = NULL;
        }
      dll->size--;
    }
  return node;
}

static t_dll_node       *dll_pop_front_raw(t_dll *dll)
{
  t_dll_node            *node = dll->begin;

  if (node)
    {
      if (node->next)
        {
          node->next->prev = NULL;
          dll->begin = node->next;
        }
      else
        {
          dll->begin = NULL;
          dll->end = NULL;
        }
      dll->size--;
    }
  return node;
}

static t_dll_node       *dll_create_reuse_node(void *data)
{
  t_dll_node    *node;

  if (_pool && _pool->size)
    {
      node = dll_pop_front_raw(_pool);
      node->data = data;
    }
  else
    node = dll_node_create(data);
  return node;
}

static void             *dll_store_node(t_dll_node *node)
{
  if (!node)
    return NULL;
  dll_push_back_raw(_pool, node);
  return node->data;
}

void            dll_push_back(t_dll *dll, void *data)
{
  dll_push_back_raw(dll, dll_create_reuse_node(data));
}

void            dll_push_front(t_dll *dll, void *data)
{
  dll_push_front_raw(dll, dll_create_reuse_node(data));
}

void            dll_insert_before(t_dll *dll, t_dll_node *node, void *data)
{
  t_dll_node    *newnode;

  if (!node)
    return;
  if (node == dll->begin)
    dll_push_front(dll, data);
  else
    {
      newnode = dll_create_reuse_node(data);
      newnode->prev = node->prev;
      newnode->next = node;
      newnode->prev->next = newnode;
      newnode->next->prev = newnode;
      dll->size++;
    }
}

void            dll_insert_after(t_dll *dll, t_dll_node *node, void *data)
{
  t_dll_node    *newnode;

  if (!node)
    return;
  if (node == dll->end)
    dll_push_back(dll, data);
  else
    {
      newnode = dll_create_reuse_node(data);
      newnode->prev = node;
      newnode->next = node->next;
      newnode->prev->next = newnode;
      newnode->next->prev = newnode;
      dll->size++;
    }
}

void            *dll_pop_back(t_dll *dll)
{
  return dll_store_node(dll_pop_back_raw(dll));
}

void            *dll_pop_front(t_dll *dll)
{
  return dll_store_node(dll_pop_front_raw(dll));
}

void            *dll_erase(t_dll *dll, t_dll_node *node)
{
  if (!node)
    return NULL;
  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (node == dll->begin)
    dll->begin = node->next;
  if (node == dll->end)
    dll->end = node->prev;
  dll->size--;
  return dll_store_node(node);
}
