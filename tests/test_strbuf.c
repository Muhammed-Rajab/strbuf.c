#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/strbuf/strbuf.h"

#define RUN_AND_PRINT_IF_NOT_STRBUF_OK(fn)                                     \
  err = (fn);                                                                  \
  if (err != STRBUF_OK) {                                                      \
    fprintf(stderr, "line %d: strbuf error: %s\n", __LINE__,                   \
            strbuf_err_str(err));                                              \
    exit(1);                                                                   \
  }

int main() {
  strbuf_err err;

  strbuf sb;
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_init(&sb))

  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_append(&sb, "oy mate! "))
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_append(&sb, "are ya stoopid?"))
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_append(&sb, "12356789"))

  printf("%s\n", strbuf_cstr(&sb));
  printf("len: %zu, cap: %zu\n", strbuf_len(&sb), sb.cap);

  strbuf slice;
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_init(&slice))
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_slice(&sb, &slice, 0, 4))

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_slice(&sb, &slice, 0, sb.len))

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  // WARN: causes crash
  // RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_slice(&sb, &slice, 0, sb.len + 5));

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("len: %zu, cap: %zu\n", strbuf_len(&slice), slice.cap);

  bool equal = false;
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_cmp(&sb, &slice, &equal))

  printf("are slice and sb equal? %s\n", equal ? "yes" : "no");

  char ch;
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_get(&slice, 0, &ch))
  printf("slice: ch at %d is %c\n", 0, ch);

  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_get(&slice, -slice.len, &ch))
  printf("slice: ch at %ld is %c\n", (int64_t)(-slice.len), ch);

  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_get(&slice, -1, &ch))
  printf("slice: ch at %d is %c\n", -1, ch);

  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_reverse(&slice))
  printf("reversed slice is: '%s'\n", strbuf_cstr(&slice));

  strbuf new;
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_init(&new))
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_cpy(&slice, &new))

  printf("slice: '%s'\n", strbuf_cstr(&slice));
  printf("new: '%s'\n", strbuf_cstr(&new));

  strbuf name = {0};
  RUN_AND_PRINT_IF_NOT_STRBUF_OK(strbuf_from_strlit(&name, "Rajab Kabeer"))
  printf("name: '%s'\n", strbuf_cstr(&name));

  strbuf_free(&sb);
  strbuf_free(&slice);
  strbuf_free(&new);
  strbuf_free(&name);
  return EXIT_SUCCESS;
}
