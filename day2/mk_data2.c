#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
  char *filename = argv[1];
  int fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if (fd == -1) {perror("open"); exit(1);}
  int N = 256;
  unsigned char data[N];
  for (int i = 0; i < N; i++){
    data[i] = i;
  }
  int fw = write(fd, data, N);
  if (fw == -1){perror("write");exit(1);}
  close(fd);
}
