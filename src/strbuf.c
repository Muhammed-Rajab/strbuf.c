#include "../include/strbuf/strbuf.h"

#include <stdlib.h>

#define STRBUF_INIT_CAP 16

strbuf strbuf_new(void) {
  strbuf sb;
  sb.len = 0;
  sb.cap = STRBUF_INIT_CAP;
  sb.data = malloc(sb.cap);

  if (!sb.data) {
    sb.cap = 0;
    return sb;
  }

  sb.data[0] = '\0';
  return sb;
}

void strbuf_free(strbuf *sb) {
  if (!sb || !sb->data)
    return;

  free(sb->data);
  sb->len = 0;
  sb->cap = 0;
  sb->data = NULL;
}
