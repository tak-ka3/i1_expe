/* 
 * p05.c
 */

/* 
 * usage:
 *   ./a.out
 * 
 * Intended behavior
 *   It reads the contents of this file and print it.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
int main()
{
  FILE * fp = fopen("p05.c", "rb");
  char buf[1000];
  while (1) {
    int n = fread(buf, 1, 1000, fp);
    if (n == 0) break;
    fwrite(buf, 1, n, stdout);
  }
  return 0;
}

