#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
  int A = atoi(argv[1]);
  int f = atoi(argv[2]);
  int n = atoi(argv[3]);
  for (int i = 0; i < n; i++){
    short x = A*sin(2.0*M_PI*f*i/44100);
    // printf("%f", A*sin(2.0*M_PI*f*i/100));
    write(1,&x,2);
  }
}
