#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/strbuf/strbuf.h"

int main() {
  strbuf sb;
  strbuf_err err = strbuf_init(&sb);
  if (err != STRBUF_OK) {
    fprintf(stderr, "failed to init strbuf: %s\n", strbuf_errstr(err));
    return 1;
  }

  strbuf_append(&sb, "oy mate! ");
  strbuf_append(&sb, "are ya stoopid?");
  strbuf_append(&sb, "12356789");
  printf("%s\n", strbuf_cstr(&sb));
  printf("len: %zu, cap: %zu\n", strbuf_len(&sb), sb.cap);

  strbuf slice;
  err = strbuf_init(&slice);
  if (err != STRBUF_OK) {
    fprintf(stderr, "failed to init strbuf: %s\n", strbuf_errstr(err));
    return 1;
  }

  err = strbuf_slice(&sb, &slice, 0, 4);
  if (err != STRBUF_OK) {
    fprintf(stderr, "failed to slice strbuf: %s\n", strbuf_errstr(err));
    return 1;
  }

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  err = strbuf_slice(&sb, &slice, 0, sb.len);
  if (err != STRBUF_OK) {
    fprintf(stderr, "failed to slice strbuf: %s\n", strbuf_errstr(err));
    return 1;
  }

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  // causes crash
  // err = strbuf_slice(&sb, &slice, 0, sb.len + 5);
  // if (err != STRBUF_OK) {
  //   fprintf(stderr, "failed to slice strbuf: %s\n", strbuf_errstr(err));
  //   return 1;
  // }

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  bool equal = false;
  err = strbuf_cmp(&sb, &slice, &equal);
  if (err != STRBUF_OK) {
    fprintf(stderr, "failed to compare strbuf: %s\n", strbuf_errstr(err));
    return 1;
  }

  printf("are slice and sb equal? %s\n", equal ? "yes" : "no");

  char ch;
  strbuf_get(&slice, 0, &ch);
  printf("slice: ch at %d is %c\n", 0, ch);

  strbuf_get(&slice, -slice.len, &ch);
  printf("slice: ch at %ld is %c\n", (int64_t)(-slice.len), ch);

  strbuf_get(&slice, -1, &ch);
  printf("slice: ch at %d is %c\n", -1, ch);

  strbuf_free(&sb);
  strbuf_free(&slice);
  return EXIT_SUCCESS;
}
