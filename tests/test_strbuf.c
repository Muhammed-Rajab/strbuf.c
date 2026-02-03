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

  strbuf_append(&sb, "oy mate!\n");
  strbuf_append(&sb, "are ya stoopid?");
  strbuf_append(&sb, "12356789");
  printf("%s\n", strbuf_cstr(&sb));
  printf("len: %zu, cap: %zu\n", strbuf_len(&sb), sb.cap);

  strbuf_free(&sb);
  return EXIT_SUCCESS;
}
