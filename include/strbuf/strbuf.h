#ifndef STRBUF_H_
#define STRBUF_H_

#include <stddef.h>
#include <stdlib.h>

typedef enum {
  STRBUF_OK = 0,
  STRBUF_ERR_OOM,
  STRBUF_ERR_RANGE,
  STRBUF_ERR_INVALID,
} strbuf_err;

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf;

// WARN: for all the methods, a valid strbuf must be passed.
// no checking for null is implemented to keep the API clean.
strbuf_err strbuf_init(strbuf *sb);
void strbuf_free(strbuf *sb);

size_t strbuf_len(const strbuf *sb);
const char *strbuf_cstr(const strbuf *sb);

const char *strbuf_errstr(strbuf_err err);

strbuf_err strbuf_reserve(strbuf *sb, size_t needed);
strbuf_err strbuf_clear(strbuf *sb);

#endif
