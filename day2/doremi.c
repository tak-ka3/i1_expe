#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
  int A = atoi(argv[1]);
  int n = atoi(argv[2]);
  // int N = n*13230;
  double f_level = 261.626;
  for (int i = 0; i < n; i++){
    for (int j = 0; j < 13230; j++){
      short x = A*sin(2.0*M_PI*f_level*j/44100);
      // printf("%d\n", x);
      write(1, &x, 2);
    }
    if (i%7==2 || i%7==6){
      f_level *= pow(2.0, 1.0/12);
      //printf("%lf\n",f_level);
    }else{
      f_level*= pow(2.0, 1.0/6);
    }
  }
}
