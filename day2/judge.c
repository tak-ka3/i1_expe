#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef short sample_t;

double min5(double a, double b, double c, double d, double e){
  double m = a;
  if (m > b){
    m = b;
  }
  if (m > c){
    m = c;
  }
  if (m > d){
    m = d;
  }
  if (m > e){
    m = e;
  }
  return m;
}

void die(char * s) {
  perror(s); 
  exit(1);
}

/* fd から 必ず n バイト読み, bufへ書く.
   n バイト未満でEOFに達したら, 残りは0で埋める.
   fd から読み出されたバイト数を返す */
ssize_t read_n(int fd, ssize_t n, void * buf) {
  ssize_t re = 0;
  while (re < n) {
    ssize_t r = read(fd, buf + re, n - re);
    if (r == -1) die("read");
    if (r == 0) break;
    re += r;
  }
  memset(buf + re, 0, n - re);
  return re;
}

/* fdへ, bufからnバイト書く */
ssize_t write_n(int fd, ssize_t n, void * buf) {
  ssize_t wr = 0;
  while (wr < n) {
    ssize_t w = write(fd, buf + wr, n - wr);
    if (w == -1) die("write");
    wr += w;
  }
  return wr;
}

/* 標本(整数)を複素数へ変換 */
void sample_to_complex(sample_t * s, 
		       complex double * X, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) X[i] = s[i];
}

/* 複素数を標本(整数)へ変換. 虚数部分は無視 */
void complex_to_sample(complex double * X, 
		       sample_t * s, 
		       long n) {
  long i;
  for (i = 0; i < n; i++) {
    s[i] = creal(X[i]);
  }
}

/* 高速(逆)フーリエ変換;
   w は1のn乗根.
   フーリエ変換の場合   偏角 -2 pi / n
   逆フーリエ変換の場合 偏角  2 pi / n
   xが入力でyが出力.
   xも破壊される
 */
void fft_r(complex double * x, 
	   complex double * y, 
	   long n, 
	   complex double w) {
  if (n == 1) { y[0] = x[0]; }
  else {
    complex double W = 1.0; 
    long i;
    for (i = 0; i < n/2; i++) {
      y[i]     =     (x[i] + x[i+n/2]); /* 偶数行 */
      y[i+n/2] = W * (x[i] - x[i+n/2]); /* 奇数行 */
      W *= w;
    }
    fft_r(y,     x,     n/2, w * w);
    fft_r(y+n/2, x+n/2, n/2, w * w);
    for (i = 0; i < n/2; i++) {
      y[2*i]   = x[i];
      y[2*i+1] = x[i+n/2];
    }
  }
}

void fft(complex double * x, 
	 complex double * y, 
	 long n) {
  long i;
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) - 1.0j * sin(arg);
  fft_r(x, y, n, w);
  for (i = 0; i < n; i++) y[i] /= n;
}

void ifft(complex double * y, 
	  complex double * x, 
	  long n) {
  double arg = 2.0 * M_PI / n;
  complex double w = cos(arg) + 1.0j * sin(arg);
  fft_r(y, x, n, w);
}

int pow2check(long N) {
  long n = N;
  while (n > 1) {
    if (n % 2) return 0;
    n = n / 2;
  }
  return 1;
}

void print_complex(FILE * wp, 
		   complex double * Y, long n) {
  long i;
  for (i = 0; i < n; i++) {
    fprintf(wp, "%ld %f %f %f %f\n", 
	    i, 
	    creal(Y[i]), cimag(Y[i]),
	    cabs(Y[i]), atan2(cimag(Y[i]), creal(Y[i])));
  }
}


int main(int argc, char ** argv) {
  (void)argc;
  long n = atol(argv[1]);
  int low = atol(argv[2]);
  int high = atol(argv[3]);
  char *filename = argv[4];
  if (!pow2check(n)) {
    fprintf(stderr, "error : n (%ld) not a power of two\n", n);
    exit(1);
  }
  FILE * wp = fopen("fft.dat", "wb");
  FILE * wp2 = fopen(filename, "wb");

  FILE * wpa;
  FILE * wpi;
  FILE * wpu;
  FILE * wpe;
  FILE * wpo;
  if ((wpa = fopen("a.txt", "rb")) == NULL){
      die("fileopen");
  }
  if ((wpi = fopen("i.txt", "rb")) == NULL){
      die("fileopen");
  }
  if ((wpu = fopen("u.txt", "rb")) == NULL){
      die("fileopen");
  }
  if ((wpe = fopen("e.txt", "rb")) == NULL){
      die("fileopen");
  }
  if ((wpo = fopen("o.txt", "rb")) == NULL){
      die("fileopen");
  }
  if (wp == NULL) die("fopen");
  sample_t * buf = calloc(sizeof(sample_t), n);
  complex double * X = calloc(sizeof(complex double), n);
  complex double * Y = calloc(sizeof(complex double), n);
  double y[230];
  for (int i = 0; i < 230; i++){
    y[i] = 0;
  }
  int cnt_while = 0; // 何回yが足されたか数える
  while (1) {
    /* 標準入力からn個標本を読む */
    ssize_t m = read_n(0, n * sizeof(sample_t), buf);
    if (m == 0) break;
    /* 複素数の配列に変換 */
    sample_to_complex(buf, X, n);
    /* FFT -> Y */
    fft(X, Y, n);
    
    // print_complex(wp, Y, n);
    // fprintf(wp, "----------------\n");
	
    // 標本はいくつか読み取る
    int cnt_ori = 0;
    for (int i = 0; i < n/2; i++){
          fprintf(wp2, "%ld %f \n", 44100/n*i, cabs(Y[i]));
          if (44100 / n *i > 50 && 44100/n*i < 1205){
            y[cnt_ori] += cabs(Y[i]);
            cnt_ori++;
          }
    }
    cnt_while++;

    // 上のやつの平均を取ったものが必要そう
    /* IFFT -> Z */
    ifft(Y, X, n);
    /* 標本の配列に変換 */
    complex_to_sample(X, buf, n);
    /* 標準出力へ出力 */
    // write_n(1, m, buf);
  }
  for (int i = 0; i < 230; i++){
    y[i] /= cnt_while;
  }
  int fra[230];
  double ya[4096];
  double total_ya[230] = {};
  int ind = 0;
  int cnta = 0;
  while(fscanf(wpa, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > 50 && *(fra+ind) < 1205){
      total_ya[ind] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==230){
          ind = -1;
          cnta++;
      }
      ind++;
    }
  }
  for (int i = 0; i < 230; i++){
    total_ya[i] /= cnta;
  }
  double total_yi[230] = {};
  ind = 0;
  int cnti = 0;
  while(fscanf(wpi, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > 50 && *(fra+ind) < 1205){
      total_yi[ind] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==230){
          ind = -1;
          cnti++;
      }
      ind++;
    }
  }
  for (int i = 0; i < 230; i++){
    total_yi[i] /= cnti;
  }

  double total_yu[230] = {};
  ind = 0;
  int cntu = 0;
  while(fscanf(wpu, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > 50 && *(fra+ind) < 1205){
      total_yu[ind] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==230){
          ind = -1;
          cntu++;
      }
      ind++;
    }
  }
  for (int i = 0; i < 230; i++){
    total_yu[i] /= cntu;
  }

  double total_ye[230] = {};
  ind = 0;
  int cnte = 0;
  while(fscanf(wpe, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > 50 && *(fra+ind) < 1205){
      total_ye[ind] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==230){
          ind = -1;
          cnte++;
      }
      ind++;
    }
  }
  for (int i = 0; i < 230; i++){
    total_ye[i] /= cnte;
  }

  double total_yo[230] = {};
  ind = 0;
  int cnto = 0;
  while(fscanf(wpo, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > 50 && *(fra+ind) < 1205){
      total_yo[ind] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==230){
          ind = -1;
          cnto++;
      }
      ind++;
    }
  }
  for (int i = 0; i < 230; i++){
    total_yo[i] /= cnto;
  }

  double total_a2 = 0;
  double total_i2 = 0;
  double total_u2 = 0;
  double total_e2 = 0;
  double total_o2 = 0;
  for (int i = 0; i < 230; i++){
    printf("%f,%f\n", total_ya[i], total_ye[i]);
    total_a2 += pow(y[i]-total_ya[i], 2)/100000;
    total_i2 += pow(y[i]-total_yi[i], 2)/100000;
    total_u2 += pow(y[i]-total_yu[i], 2)/100000;
    total_e2 += pow(y[i]-total_ye[i], 2)/100000;
    total_o2 += pow(y[i]-total_yo[i], 2)/100000;
  }

  printf("%f, %f, %f, %f, %f\n", total_a2, total_i2, total_u2, total_e2, total_o2);
  double mint = min5(total_a2, total_i2, total_u2, total_e2, total_o2);
  printf("mint=%f\n", mint);
  if (mint == total_a2){
    printf("a\n");
  }else if(mint == total_i2){
    printf("i\n");
  }else if(mint == total_u2){
    printf("u\n");
  }else if(mint == total_e2){
    printf("e\n");
  }else{
    printf("o\n");
  }

  fclose(wp);
  return 0;
}
