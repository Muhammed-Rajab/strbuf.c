#ifndef STRBUF_H_
#define STRBUF_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * strbuf: length-based string buffer with a guaranteed trailing '\0'
 *
 *  i don't *rely* on null-terminated strings,
 *  but i keep one for interop and sanity.
 *
 * INVARIANTS:
 * - sb->data is either NULL (zero-initialized/ freed) or points to sb->cap
 *    bytes
 * - sb->cap >=STRBUF_INIT_CAP  if sb->data != NULL
 * - sb->len < sb->cap
 * - sb->data[sb->len] == '\0'
 *
 * `strbuf` LIFECYCLE:
 *    1. zero-initialize (e.g. `strbuf sb = {0};`)
 *    2. initialize via `strbuf_init` or `strbuf_from_strlit`
 *    3. use
 *    4. `strbuf_free`
 *    5. repeat from 1 or 2
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

//----------------------------------------------------------
// STRBUF_ERR
//----------------------------------------------------------

typedef enum {
  STRBUF_OK = 0,      // success
  STRBUF_ERR_OOM,     // out of memory
  STRBUF_ERR_RANGE,   // out of range
  STRBUF_ERR_INVALID, // invalid arguments
} strbuf_err;

// returns strbuf_err as string.
const char *strbuf_err_str(strbuf_err err);

//----------------------------------------------------------
// STRBUF
//----------------------------------------------------------

#define STRBUF_INIT_CAP 16

typedef struct {
  char *data;
  size_t len;
  size_t cap;
} strbuf;

// initialize a ZERO-INITIALIZED or PREVIOUSLY FREED strbuf.
strbuf_err strbuf_init(strbuf *sb);

// free a strbuf
void strbuf_free(strbuf *sb);

// returns sb->len. no NULL-check.
size_t strbuf_len(const strbuf *sb);

// returns sb->data. no NULL-check.
const char *strbuf_cstr(const strbuf *sb);

// ensures capacity for at least `needed` bytes (including space for '\0')
// NOTE: when reserving, make sure to account for `\0`
strbuf_err strbuf_reserve(strbuf *sb, size_t needed);

// sets len = 0, data[0] = '\0', and cap is unchanged.
strbuf_err strbuf_clear(strbuf *sb);

// appends `n` characters from `*s` to *sb`, excluding `\0`.
// for eg: "hello", is 5 characters, but tries to reserve 6 character space.
// NOTE: strlen(s) must be >= n
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
strbuf_err strbuf_copy(const strbuf *src, strbuf *dst);

// constructs `*sb` from `*lit`.
// NOTE: `*sb` should either be ZERO-INITIALIZED or PREVIOUSLY FREED.
strbuf_err strbuf_from_strlit(strbuf *sb, const char *lit);

// appends`ch` to `*sb` `n` times.
strbuf_err strbuf_append_repeated_char(strbuf *sb, char ch, size_t n);

// constructs *sb with `ch` `n` times.
// NOTE: `*sb` should either be ZERO-INITIALIZED or PREVIOUSLY FREED.
strbuf_err strbuf_repeat_char(strbuf *sb, char ch, size_t n);

#endif
