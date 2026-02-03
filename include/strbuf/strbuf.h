#ifndef STRBUF_H_
#define STRBUF_H_

#include <stddef.h>

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf;

strbuf strbuf_new(void);
void strbuf_free(strbuf *sb);

#endif
