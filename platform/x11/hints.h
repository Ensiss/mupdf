#ifndef         __HINTS_H__
#define         __HINTS_H__

#include        "mupdf/fitz.h"
#include        "tree.h"
#include        "dll.h"

typedef struct  s_hint
{
  char          hint[8];
  fz_link       *link;
}               t_hint;

t_dll           *computeHints(fz_link *page_links, char *hintchars);

#endif
