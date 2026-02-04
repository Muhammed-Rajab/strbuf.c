#include "../include/strbuf/strbuf.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// macros

#define STRBUF_REQUIRE_INIT(sb)                                                \
  do {                                                                         \
    if (!(sb) || !(sb)->data)                                                  \
      return STRBUF_ERR_INVALID;                                               \
  } while (0)

const char *strbuf_err_str(strbuf_err err) {
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
  STRBUF_REQUIRE_INIT(sb);

  if (needed <= sb->cap)
    return STRBUF_OK;

  size_t new_cap = sb->cap;
  while (new_cap < needed) {
    if (new_cap > SIZE_MAX / 2)
      return STRBUF_ERR_RANGE;

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
  STRBUF_REQUIRE_INIT(sb);

  sb->len = 0;
  sb->data[0] = '\0';
  return STRBUF_OK;
}

strbuf_err strbuf_append_n(strbuf *sb, const char *s, size_t n) {
  STRBUF_REQUIRE_INIT(sb);

  if (!s)
    return STRBUF_ERR_INVALID;

  strbuf_err err;

  err = strbuf_reserve(sb, sb->len + n + 1);
  if (err != STRBUF_OK)
    return err;

  memcpy(sb->data + sb->len, s, n);
  sb->len += n;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}

strbuf_err strbuf_append(strbuf *sb, const char *s) {
  if (!s)
    return STRBUF_ERR_INVALID;
  return strbuf_append_n(sb, s, strlen(s));
}

strbuf_err strbuf_push(strbuf *sb, char ch) {
  STRBUF_REQUIRE_INIT(sb);

  strbuf_err err;

  err = strbuf_reserve(sb, sb->len + 2); // 1 for ch and 1 for \0
  if (err != STRBUF_OK)
    return err;

  sb->data[sb->len] = ch;
  sb->len += 1;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}

strbuf_err strbuf_pop(strbuf *sb, char *ch) {
  STRBUF_REQUIRE_INIT(sb);

  if (sb->len == 0)
    return STRBUF_ERR_RANGE;

  if (ch)
    *ch = sb->data[sb->len - 1];

  sb->len -= 1;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}

strbuf_err strbuf_slice(strbuf *from, strbuf *to, size_t start, size_t stop) {
  STRBUF_REQUIRE_INIT(from);
  STRBUF_REQUIRE_INIT(to);

  strbuf_err err;

  if (!(start <= from->len && stop >= start && stop <= from->len))
    return STRBUF_ERR_RANGE;

  strbuf_clear(to);

  size_t needed = stop - start;
  err = strbuf_append_n(to, from->data + start, needed);
  if (err != STRBUF_OK)
    return err;

  return STRBUF_OK;
}

bool strbuf_cmp(const strbuf *a, const strbuf *b) {
  if (!a || !b || !a->data || !b->data)
    return false;

  return a->len == b->len && memcmp(a->data, b->data, a->len) == 0;
}

bool strbuf_cmp_cstr(const strbuf *sb, const char *lit) {
  if (!sb || !sb->data || !lit)
    return false;

  if (sb->len == 0)
    return *lit == '\0';

  size_t n = strlen(lit);
  return sb->len == n && memcmp(sb->data, lit, n) == 0;
}

strbuf_err strbuf_get(const strbuf *sb, int64_t index, char *ch) {
  STRBUF_REQUIRE_INIT(sb);

  if (!ch)
    return STRBUF_ERR_INVALID;

  // WARN: this can cause issues if len is like bigger than i64 max value
  if (sb->len > INT64_MAX)
    return STRBUF_ERR_RANGE;
  int64_t ilen = (int64_t)sb->len;

  if (index < -ilen || index > ilen - 1)
    return STRBUF_ERR_RANGE;

  index = index >= 0 ? index : ilen + index;

  *ch = sb->data[index];

  return STRBUF_OK;
}

strbuf_err strbuf_reverse(strbuf *sb) {
  STRBUF_REQUIRE_INIT(sb);

  // you don't have to reverse a string of len 1 or 0
  if (sb->len <= 1)
    return STRBUF_OK;

  // truncates floating part
  size_t half = sb->len / 2;

  for (size_t i = 0; i < half; i += 1) {
    // index of the last char
    size_t ni = sb->len - 1 - i;

    char tmp = sb->data[i];
    sb->data[i] = sb->data[ni];
    sb->data[ni] = tmp;
  }

  return STRBUF_OK;
}

strbuf_err strbuf_copy(const strbuf *src, strbuf *dst) {
  STRBUF_REQUIRE_INIT(src);
  STRBUF_REQUIRE_INIT(dst);

  strbuf_err err;

  err = strbuf_clear(dst);
  if (err != STRBUF_OK)
    return err;

  err = strbuf_append_n(dst, src->data, src->len);
  if (err != STRBUF_OK)
    return err;

  return STRBUF_OK;
}

// NOTE: `*sb` should either be ZERO-INITIALIZED or PREVIOUSLY FREED.
strbuf_err strbuf_from_strlit(strbuf *sb, const char *lit) {
  if (!sb || !lit)
    return STRBUF_ERR_INVALID;

  strbuf_err err;

  err = strbuf_init(sb);
  if (err != STRBUF_OK)
    return err;

  err = strbuf_append(sb, lit);
  if (err != STRBUF_OK) {
    strbuf_free(sb);
    return err;
  }

  return STRBUF_OK;
}

strbuf_err strbuf_append_repeated_char(strbuf *sb, char ch, size_t n) {
  STRBUF_REQUIRE_INIT(sb);

  strbuf_err err;

  err = strbuf_reserve(sb, sb->len + n + 1);
  if (err != STRBUF_OK)
    return err;

  memset(sb->data + sb->len, ch, n);

  sb->len += n;

  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}

// NOTE: `*sb` should either be ZERO-INITIALIZED or PREVIOUSLY FREED.
strbuf_err strbuf_repeat_char(strbuf *sb, char ch, size_t n) {
  if (!sb)
    return STRBUF_ERR_INVALID;

  strbuf_err err;
  err = strbuf_init(sb);
  if (err != STRBUF_OK)
    return err;

  err = strbuf_reserve(sb, n + 1);
  if (err != STRBUF_OK) {
    strbuf_free(sb);
    return err;
  }

  memset(sb->data, ch, n);
  sb->len = n;
  sb->data[sb->len] = '\0';

  return STRBUF_OK;
}
