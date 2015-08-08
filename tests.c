#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#include "art.h"

extern word_t bytes;

byte_t* getWord (FILE* f) {
  byte_t* buf = NULL;
  int c = 0, i = 0, bufsize = 10;
  buf = malloc(bufsize + 1);
  memset(buf, 0, bufsize + 1);
  while ((c = fgetc(f)) != '\n') {
    if (c == EOF) return NULL;
    if (i == bufsize)
      buf = realloc(buf, (bufsize += 10) + 1);
    buf[i++] = (byte_t)c;
  }
  buf[i] = 0;
  return buf;
}

void wordBench (Art* d, int argc, char** argv) {
  FILE* in = fopen(argv[1], "r");
  byte_t* word, *val;
  size_t actual = 0;
  int wc = 0;
  double end, start;
  srand(time(NULL));
  start = (float)clock()/CLOCKS_PER_SEC;
  while ((word = getWord(in))) {
    int l = strlen((char *)word);
    wc++;
    
    val = malloc(l + 5);
    memset(val, 0, l + 5);
    strncpy((char *)val, (char *)word, l);
    strncat((char *)val, "-val", 4);
    val[l + 4] = 0;
    
    actual += l;
    artPut(d, (byte_t*)word, l, (word_t)val);
    free(word);
    word = NULL;
  }
  end = (float)clock()/CLOCKS_PER_SEC;
  printf("Inserted %d words.\n", wc);
  printf("Total: %lu bytes.\n", bytes);
  printf("Overhead per key: %lu bytes.\n", (bytes - actual)/wc);
  printf("Actual: %lu bytes.\n", actual);
  printf("Finshed in %f.\n", end-start);
  fclose(in);
}

void deleteBench (Art* d, char* file) {
  FILE* in = fopen(file, "r");
  byte_t* word, *val;
  int wc = 0;
  double end, start;
  start = (float)clock()/CLOCKS_PER_SEC;
  while ((word = getWord(in))) {
    int l = strlen((char *)word);
    artRemove(d, (byte_t*)word, l);
    wc++;
    /*if (wc == 10001) break;*/
    free(word);
    word = NULL;
  }
  end = (float)clock()/CLOCKS_PER_SEC;
  printf("Deleted %d words.\n", wc);
  printf("Finshed in %f.\n", end-start);
  fclose(in);
}

void getBench (Art* d, char* file) {
  FILE* in = fopen(file, "r");
  byte_t* word, *val;
  double end, start;
  int wc = 0;
  word_t vv;
  start = (float)clock()/CLOCKS_PER_SEC;
  while ((word = getWord(in))) {
    int l = strlen((char *)word);
    vv = artGet(d, word, l);
    if (vv) {
      wc++;
    }
    free(word);
    word = NULL;
  }
  end = (float)clock()/CLOCKS_PER_SEC;
  printf("Retrieved %d words.\n", wc);
  printf("Finshed in %f.\n", end-start);
  printf("Total: %lu bytes.\n", bytes);
  fclose(in);
}

void numberBench (Art* d) {
  double end, start;
  size_t actual = 0;
  int i;
  srand(time(NULL));
  start = (float)clock()/CLOCKS_PER_SEC;
  artPut(d, (byte_t *)"hello", 5, (word_t)"world");
  for (i = 0; i < 1000000; i++) {
    actual += sizeof(int);
    artPut(d, (byte_t *)(&i), sizeof(int), (word_t)"val");
  }
  end = (float)clock()/CLOCKS_PER_SEC;
  printf("Inserted 1 million integers.\n");
  printf("Total: %lu bytes.\n", bytes);
  printf("Overhead per key: %lu bytes.\n", (bytes - actual)/1000000);
  printf("Actual: %lu bytes.\n", actual);
  printf("Finshed in %f.\n", end-start);
}

int main (int argc, char** argv) {
  word_t val;
  Art* d = artNew();
  artVal* vals;

  if (argc < 2) {
    return 1;
  }
  
  wordBench(d, argc, argv);
  puts("Press enter to continue...");
  getchar();
  
  getBench(d, argv[1]);
  puts("Press enter to continue...");
  getchar();

  vals = artGetWithPrefix(d, (byte_t *)"far", 2);
  while (vals) {
    printf("key: %s\nvalue: %s\n", vals->key, (char *)vals->val);
    vals = vals->next;
  }
  puts("Press enter to continue...");
  getchar();

  
  deleteBench(d, argv[1]);
  puts("Press enter to continue...");
  getchar();

  getBench(d, argv[1]);
  puts("Press enter to continue...");
  getchar();
  
  if (argc > 2) {
    puts((char *)artGet(d, (byte_t *)argv[2], strlen(argv[2])));
  }

  return 0;
}
