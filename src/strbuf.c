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

size_t strbuf_len(const strbuf *sb) { return sb->len; }

const char *strbuf_cstr(const strbuf *sb) { return sb->data; }

strbuf_err strbuf_reserve(strbuf *sb, size_t needed) {
  if (needed <= sb->cap)
    return STRBUF_OK;

  size_t new_cap = sb->cap;
  while (new_cap < needed)
    new_cap *= 2;

  char *p = (char *)realloc(sb->data, new_cap);
  if (!p)
    return STRBUF_ERR_OOM;

  sb->data = p;
  sb->cap = new_cap;
  return STRBUF_OK;
}

const char *strbuf_errstr(strbuf_err err) {
  switch (err) {
  case STRBUF_OK:
    return "ok";
  case STRBUF_ERR_OOM:
    return "out of memory";
  case STRBUF_ERR_INVALID:
    return "invalid argument";
  case STRBUF_ERR_RANGE:
    return "out of range";
  default:
    return "unknown error";
  }
}
