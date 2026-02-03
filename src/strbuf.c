#include "../include/strbuf/strbuf.h"

#include <stdlib.h>

strbuf strbuf_new(void) {
  return (strbuf){
      .data = NULL,
      .cap = 0,
      .len = 0,
  };
}

void strbuf_free(strbuf *sb) {
  if (!sb)
    return;

  if (!sb->data)
    return;

  free(sb->data);

  sb->len = 0;
  sb->cap = 0;
  sb->data = NULL;
}
