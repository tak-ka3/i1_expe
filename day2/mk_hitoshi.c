#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
  int fd = open("hitoshi",O_WRONLY|O_CREAT|O_TRUNC, 064);
  int N = 6;
  unsigned char data[N];
  data[0] = 228;
  data[1] = 186;
  data[2] = 186;
  data[3] = 229;
  data[4] = 191;
  data[5] = 151;
  write(fd, data, N);
  close(fd);
}
