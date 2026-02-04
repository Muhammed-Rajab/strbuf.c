#ifndef STRBUF_H_
#define STRBUF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// error handling
typedef enum {
  STRBUF_OK = 0,      // success
  STRBUF_ERR_OOM,     // out of memory
  STRBUF_ERR_RANGE,   // out of range
  STRBUF_ERR_INVALID, // invalid arguments
} strbuf_err;

// strbuf_err as string
const char *strbuf_err_str(strbuf_err err);

/*
 * strbuf: i don't fuck with null terminated strings
 *
 * INVARIANTS:
 * - sb->data is either NULL (uninitialized / freed) or points to sb->cap bytes
 * - sb->cap >=STRBUF_INIT_CAP  if sb->data != NULL
 * - sb->len < sb->cap
 * - sb->data[sb->len] == '\0'
 *
 * pretty much every mutating functions require an initialized `strbuf` struct.
 * ignore this and get fucked.
 *
 * every method except the following checks whether passed `strbuf` is NULL:
 * - strbuf_cstr
 * - strbuf_len
 *
 * the struct fields should not be accessed raw. `strbuf` has functions for a
 * reason.
 * */
#define STRBUF_INIT_CAP 16

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf;

// initialize
strbuf_err strbuf_init(strbuf *sb);

// free
void strbuf_free(strbuf *sb);

// returns len no NULL-check.
size_t strbuf_len(const strbuf *sb);

// returns data. no NULL-check.
const char *strbuf_cstr(const strbuf *sb);

// NOTE: '\\0' is not part of the string length.
strbuf_err strbuf_reserve(strbuf *sb, size_t needed);

// sets len = 0, data[0] = '\0', and cap is unchanged.
strbuf_err strbuf_clear(strbuf *sb);

// appends `n` characters from `*s` to *sb`, excluding `\0`.
// for eg: "hello", is 5 characters, but tries to reserve 6 character space.
strbuf_err strbuf_append_n(strbuf *sb, const char *s, size_t n);

// appends `*s` to `*sb`. `*s` must be NULL-terminated.
strbuf_err strbuf_append(strbuf *sb, const char *s);

// pushes `ch` to the end of `*sb`.
strbuf_err strbuf_push(strbuf *sb, char ch);

// pops last character from `*sb` and sets to `*ch` if ch not NULL.
strbuf_err strbuf_pop(strbuf *sb, char *ch);

// slices `*from` from `start` to `stop`, excluding `stop`.
// `*from` and `*to` must be initialized.
strbuf_err strbuf_slice(strbuf *from, strbuf *to, size_t start, size_t stop);

// compares `*a` contents against `*b`. set *equal = true if EQUAL
strbuf_err strbuf_cmp(const strbuf *a, const strbuf *b, bool *equal);

// character at `index` from `*sb`, set it to `*ch`.
// NOTE: supports negative indexing. index=-1, means last character
strbuf_err strbuf_get(const strbuf *sb, int64_t index, char *ch);

// reverse `*sb` in place.
strbuf_err strbuf_reverse(strbuf *sb);

// copies `*sb` to `*dest`. `*dest` must be intialised.
strbuf_err strbuf_copy(strbuf *src, strbuf *dst);

// constructs `*sb` from `*lit`.
// NOTE: `*sb` MUST NOT BE INITIALISED.
strbuf_err strbuf_from_strlit(strbuf *sb, const char *lit);

#endif
