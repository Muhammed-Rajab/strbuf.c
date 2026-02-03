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

  strbuf_free(&sb);
  return EXIT_SUCCESS;
}
