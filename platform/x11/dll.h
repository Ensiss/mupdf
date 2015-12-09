#ifndef         __DLL_H__
#define         __DLL_H__

#include        <stdint.h>

typedef struct  s_dll_node      t_dll_node;
struct          s_dll_node
{
  t_dll_node    *prev;
  t_dll_node    *next;
  void          *data;
};

typedef struct  s_dll
{
  t_dll_node    *begin;
  t_dll_node    *end;
  uint32_t      size;
}               t_dll;

t_dll           *dll_create();
void            dll_delete(t_dll *dll);
void            dll_cleanup();

void            dll_push_back(t_dll *dll, void *data);
void            dll_push_front(t_dll *dll, void *data);
void            dll_insert_before(t_dll *dll, t_dll_node *node, void *data);
void            dll_insert_after(t_dll *dll, t_dll_node *node, void *data);
void            *dll_pop_back(t_dll *dll);
void            *dll_pop_front(t_dll *dll);
void            *dll_erase(t_dll *dll, t_dll_node *node);

#endif
