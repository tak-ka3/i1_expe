#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
  int N = atoi(argv[1]);
  short data[N];
  while (1){
    int n = read(0, data, 2*N);
    if (n == -1){perror("read");exit(1);}
    if (n == 0) break;
    write(1,data, 2);
  }
}
