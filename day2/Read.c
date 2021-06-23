#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef short sample_t;

void die(char * s) {
  perror(s); 
  exit(1);
}

int main(int argc, char ** argv) {
  FILE * wp;
  if ((wp = fopen("a.txt", "rb")) == NULL){
      die("fileopen");
  }
  printf("%d\n", 1);

//   int* fr = (int*)malloc((sizeof(int)*100));
//   long* y = (long*)malloc((sizeof(long)*100));
  int fr[4096];
  double y[4096];
  int i = 0;
  int cnt = 0;
  double total = 0;
  double total_part = 0;
  while(fscanf(wp, "%d %lf\n", fr+i, y+i) != EOF){
      printf("%d, %lf\n", *(fr+i), *(y+i));
      if (i+1 % 4096 == 0){
          cnt ++;
          i = -1;
      }
      i++;
  }
//   for(int i = 0; i < 100; i++){
//       fscanf(wp, "%d %ld\n", fr+i, y+i);
//       printf("%d, %ld\n", *(fr+i), *(y+i));
//   }
  fclose(wp);
  return 0;
}
