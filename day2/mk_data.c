#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
  char *filename = argv[1];
  int fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC, 0644);
  int N = 256;
  unsigned char data[N];
  for (int i = 0; i < N; i++){
    data[i] = i;
  }
  write(fd, data, N);
  close(fd);
}
