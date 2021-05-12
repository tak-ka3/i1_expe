#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
  char *filename = argv[1];
  int fd = open(filename,O_RDONLY);
  if (fd == -1) {perror("open"); exit(1);}
  int N = 1;
  unsigned char data[N];
  int j = 0;
  while (1){
    int n = read(fd, data, N);
    if (n == -1){perror("read");exit(1);}
    if (n == 0) break;
    for (int i = 0; i < N; i++){
      printf("%d %d\n", j, data[i]);
      j++;
    }
  }
  close(fd);
}
