#ifndef STRBUF_H_
#define STRBUF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  STRBUF_OK = 0,
  STRBUF_ERR_OOM,
  STRBUF_ERR_RANGE,
  STRBUF_ERR_INVALID,
} strbuf_err;

// Invariants:
// - sb->data is either NULL (uninitialized / freed) or points to sb->cap bytes
// - sb->cap >= 1 if sb->data != NULL
// - sb->len < sb->cap
// - sb->data[sb->len] == '\0'
// NOTE: ALL MUTATING FUNCTIONS REQUIRE INITIALIZED `strbuf`
// DO THIS OR GET FUCKED.
typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf;

// WARN: for all the methods, a valid strbuf must be passed.
// no checking for null is implemented to keep the API clean.
strbuf_err strbuf_init(strbuf *sb);
void strbuf_free(strbuf *sb);

// can be garbage if uninitialized
size_t strbuf_len(const strbuf *sb);

// can be NULL
const char *strbuf_cstr(const strbuf *sb);

const char *strbuf_errstr(strbuf_err err);

// needed is total capacity including null terminator.
strbuf_err strbuf_reserve(strbuf *sb, size_t needed);
strbuf_err strbuf_clear(strbuf *sb);

strbuf_err strbuf_append_n(strbuf *sb, const char *s, size_t n);
strbuf_err strbuf_append(strbuf *sb, const char *s);

strbuf_err strbuf_push(strbuf *sb, char ch);
strbuf_err strbuf_pop(strbuf *sb, char *ch);

// to is also initialized, btw
// to is cleared before writing the slice
strbuf_err strbuf_slice(strbuf *from, strbuf *to, size_t start, size_t stop);

strbuf_err strbuf_cmp(strbuf *a, strbuf *b, bool *equal);

#endif
