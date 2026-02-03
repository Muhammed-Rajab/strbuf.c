#include "../include/strbuf/strbuf.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STRBUF_INIT_CAP 16

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

strbuf_err strbuf_init(strbuf *sb) {
  if (!sb)
    return STRBUF_ERR_INVALID;

  sb->len = 0;
  sb->cap = STRBUF_INIT_CAP;
  sb->data = malloc(sb->cap);

  if (!sb->data) {
    sb->data = NULL;
    sb->cap = 0;
    sb->len = 0;
    return STRBUF_ERR_OOM;
  }

  sb->data[0] = '\0';
  return STRBUF_OK;
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

  size_t new_cap = sb->cap ? sb->cap : 1;
  while (new_cap < needed) {
    if (new_cap > SIZE_MAX / 2) {
      return STRBUF_ERR_RANGE;
    }
    new_cap *= 2;
  }

  char *p = (char *)realloc(sb->data, new_cap);
  if (!p)
    return STRBUF_ERR_OOM;

  sb->data = p;
  sb->cap = new_cap;
  return STRBUF_OK;
}

strbuf_err strbuf_clear(strbuf *sb) {
  if (!sb)
    return STRBUF_ERR_INVALID;

  sb->len = 0;
  sb->data[0] = '\0';
  return STRBUF_OK;
}

strbuf_err strbuf_append_n(strbuf *sb, const char *s, size_t n) {
  strbuf_err err;

  if (!sb || !s)
    return STRBUF_ERR_INVALID;

  err = strbuf_reserve(sb, sb->len + n + 1);
  if (err != STRBUF_OK)
    return err;

  memcpy(sb->data + sb->len, s, n);
  sb->len += n;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}

strbuf_err strbuf_append(strbuf *sb, const char *s) {
  return strbuf_append_n(sb, s, strlen(s));
}

strbuf_err strbuf_push(strbuf *sb, const char ch) {
  strbuf_err err;

  if (!sb)
    return STRBUF_ERR_INVALID;

  err = strbuf_reserve(sb, sb->len + 2); // 1 for ch and 1 for \0
  if (err != STRBUF_OK)
    return err;

  sb->data[sb->len] = ch;
  sb->len += 1;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}
