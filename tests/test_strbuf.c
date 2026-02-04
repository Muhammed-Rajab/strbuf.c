/*
 * GPT generated test, btw. I swear to god, I didn't use it
 * to write the main library. I just don't want to write tests.
 * */
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../include/strbuf/strbuf.h"

static void expect_ok(strbuf_err err, const char *msg) {
  if (err != STRBUF_OK) {
    fprintf(stderr, "FAIL: %s (%s)\n", msg, strbuf_err_str(err));
    assert(0);
  }
}

static void expect_true(bool cond, const char *msg) {
  if (!cond) {
    fprintf(stderr, "FAIL: %s\n", msg);
    assert(0);
  }
}

int main(void) {
  printf("=== strbuf tests ===\n");

  // --------------------------------------------------
  // init / free
  // --------------------------------------------------
  strbuf sb = {0};
  expect_ok(strbuf_init(&sb), "strbuf_init");

  expect_true(strbuf_len(&sb) == 0, "len == 0 after init");
  expect_true(strcmp(strbuf_cstr(&sb), "") == 0, "empty string after init");

  // --------------------------------------------------
  // append / append_n
  // --------------------------------------------------
  expect_ok(strbuf_append(&sb, "hello"), "append \"hello\"");
  expect_true(strbuf_len(&sb) == 5, "len == 5");
  expect_true(strcmp(strbuf_cstr(&sb), "hello") == 0, "content == hello");

  expect_ok(strbuf_append_n(&sb, " world!!!", 6), "append_n \" world\"");
  expect_true(strcmp(strbuf_cstr(&sb), "hello world") == 0,
              "content == hello world");

  // --------------------------------------------------
  // push / pop
  // --------------------------------------------------
  expect_ok(strbuf_push(&sb, '!'), "push '!'");
  expect_true(strcmp(strbuf_cstr(&sb), "hello world!") == 0, "push worked");

  char ch = 0;
  expect_ok(strbuf_pop(&sb, &ch), "pop");
  expect_true(ch == '!', "popped '!'");
  expect_true(strcmp(strbuf_cstr(&sb), "hello world") == 0, "pop worked");

  // --------------------------------------------------
  // get (positive and negative indexing)
  // --------------------------------------------------
  expect_ok(strbuf_get(&sb, 0, &ch), "get index 0");
  expect_true(ch == 'h', "index 0 == h");

  expect_ok(strbuf_get(&sb, -1, &ch), "get index -1");
  expect_true(ch == 'd', "index -1 == d");

  // --------------------------------------------------
  // reserve (implicit test via growth)
  // --------------------------------------------------
  size_t old_cap = sb.cap;
  expect_ok(strbuf_reserve(&sb, old_cap * 4), "reserve larger capacity");
  expect_true(sb.cap >= old_cap * 4, "capacity grew");

  // --------------------------------------------------
  // clear
  // --------------------------------------------------
  expect_ok(strbuf_clear(&sb), "clear");
  expect_true(strbuf_len(&sb) == 0, "len == 0 after clear");
  expect_true(strcmp(strbuf_cstr(&sb), "") == 0, "empty after clear");

  // --------------------------------------------------
  // from_strlit
  // --------------------------------------------------
  strbuf sb2 = {0};
  expect_ok(strbuf_from_strlit(&sb2, "abcdef"), "from_strlit");
  expect_true(strcmp(strbuf_cstr(&sb2), "abcdef") == 0, "from_strlit content");

  // --------------------------------------------------
  // slice
  // --------------------------------------------------
  strbuf slice = {0};
  expect_ok(strbuf_init(&slice), "init slice");
  expect_ok(strbuf_slice(&sb2, &slice, 1, 4), "slice [1,4)");
  expect_true(strcmp(strbuf_cstr(&slice), "bcd") == 0, "slice content");

  // --------------------------------------------------
  // copy
  // --------------------------------------------------
  strbuf copy = {0};
  expect_ok(strbuf_init(&copy), "init copy");
  expect_ok(strbuf_copy(&slice, &copy), "copy");
  expect_true(strbuf_cmp(&slice, &copy), "copy equals source");

  // --------------------------------------------------
  // cmp / cmp_cstr
  // --------------------------------------------------
  expect_true(strbuf_cmp(&sb2, &sb2), "cmp same buffer");
  expect_true(strbuf_cmp_cstr(&sb2, "abcdef"), "cmp_cstr matches");
  expect_true(!strbuf_cmp_cstr(&sb2, "abcdeg"), "cmp_cstr mismatch");

  // --------------------------------------------------
  // reverse
  // --------------------------------------------------
  expect_ok(strbuf_reverse(&sb2), "reverse");
  expect_true(strcmp(strbuf_cstr(&sb2), "fedcba") == 0, "reverse worked");

  // --------------------------------------------------
  // append_repeated_char
  // --------------------------------------------------
  expect_ok(strbuf_clear(&sb2), "clear sb2");
  expect_ok(strbuf_append_repeated_char(&sb2, 'x', 5), "append_repeated_char");
  expect_true(strcmp(strbuf_cstr(&sb2), "xxxxx") == 0, "repeated char append");

  // --------------------------------------------------
  // repeat_char (constructor)
  // --------------------------------------------------
  strbuf sb3 = {0};
  expect_ok(strbuf_repeat_char(&sb3, 'z', 3), "repeat_char");
  expect_true(strcmp(strbuf_cstr(&sb3), "zzz") == 0, "repeat_char content");

  // --------------------------------------------------
  // cleanup
  // --------------------------------------------------
  strbuf_free(&sb);
  strbuf_free(&sb2);
  strbuf_free(&slice);
  strbuf_free(&copy);
  strbuf_free(&sb3);

  printf("All tests passed ✅\n");
  return 0;
}
