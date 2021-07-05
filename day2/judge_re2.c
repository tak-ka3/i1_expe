#include <assert.h>
#include <complex.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
# define v_min 50
# define v_max 4000

typedef short sample_t;

double min5(double a, double b, double c, double e){
  double m = a;
  if (m > b){
    m = b;
  }
  if (m > c){
    m = c;
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
  int v_cnt = (v_max - v_min)/ 5;
  (void)argc;
  long n = atol(argv[1]);
  char *filename = argv[2];
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
  double y[v_cnt];
  for (int i = 0; i < v_cnt; i++){
    y[i] = 0;
  }
  int cnt_while = 0; // 何回yが足されたか数える
  int second_cnt = 0;
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
          // fprintf(wp2, "%ld %f \n", 44100/n*i, cabs(Y[i]));
          if (44100 / n *i > v_min && 44100/n*i < v_max+5){
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
    second_cnt++;
  }


  double total_a2 = 0;
  double total_i2 = 0;
  double total_u2 = 0;
  double total_e2 = 0;
  double total_o2 = 0;
  double total_y = 0; // 振幅の合計を求める
  for (int i = 0; i < v_cnt; i++){
    total_y += y[i];
  }
  // int v_cnt = (v_max - v_min)/ 5;
  for (int i = 0; i < v_cnt; i++){
    y[i] = y[i]/total_y*100;
    fprintf(wp2, "%d %f \n", v_min+i*5, pow(y[i],2));
  }

  int fra[v_cnt]; // あくまで周波数を数える担当
  double ya[4096];
  double total_ya[v_cnt];
  int ya_len = sizeof(total_ya)/sizeof(total_ya[0]);
  for (int i = 0; i < ya_len; i++){
    total_ya[i] = 0;
  }
  int ind = 0;
  int cnta = 0;
  while(fscanf(wpa, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > v_min && *(fra+ind) < v_max+5){
      total_ya[ind+1] += ya[ind];
      // printf("%d, ya[ind] = %lf, %lf\n", *(fra+ind), ya[ind], total_ya[ind]);
      if (ind+1 ==v_cnt){
          ind = -1;
          cnta++;
      }
      ind++;
    }
    // printf("%d,%lf,%lf,%lf\n", ind, total_ya[ind],total_ya[0],total_ya[1]);
  }

  double total_yi[v_cnt];
  int yi_len = sizeof(total_yi)/sizeof(total_yi[0]);
  int y_len = sizeof(ya)/sizeof(ya[0]);
  
  for (int i = 0; i < yi_len; i++){
    total_yi[i] = 0;
  }
  ind = 0;
  int cnti = 0;
  double yi[4096];
  while(fscanf(wpi, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > v_min && *(fra+ind) < v_max+5){
      total_yi[ind+1] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==v_cnt){
          ind = -1;
          cnti++;
      }
      ind++;
      // printf("fra+ind=%d, ind=%d, v_max=%d, v_min=%d\n", *(fra+ind), ind, v_max, v_min);
    }
  }

  double total_yu[v_cnt];
  int yu_len = sizeof(total_yu)/sizeof(total_yu[0]);
  for (int i = 0; i < yu_len; i++){
    total_yu[i] = 0;
  }
  ind = 0;
  int cntu = 0;
  while(fscanf(wpu, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > v_min && *(fra+ind) < v_max+5){
      total_yu[ind+1] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==v_cnt){
          ind = -1;
          cntu++;
      }
      ind++;
    }
    // printf("fra+ind=%d, ind=%d, v_max=%d, v_min=%d\n", *(fra+ind), ind, v_max, v_min);
  }

  double total_ye[v_cnt];
  int ye_len = sizeof(total_ye)/sizeof(total_ye[0]);
  for (int i = 0; i < ye_len; i++){
    total_ye[i] = 0;
  }
  ind = 0;
  int cnte = 0;
  while(fscanf(wpe, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > v_min && *(fra+ind) < v_max+5){
      total_ye[ind+1] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==v_cnt){
          ind = -1;
          cnte++;
      }
      ind++;
    }
  }

  double total_yo[v_cnt];
  int yo_len = sizeof(total_yo)/sizeof(total_yo[0]);
  for (int i = 0; i < yo_len; i++){
    total_yo[i] = 0;
  }
  ind = 0;
  int cnto = 0;
  while(fscanf(wpo, "%d %lf\n", fra+ind, ya+ind) != EOF){
    if (*(fra+ind) > v_min && *(fra+ind) < v_max+5){
      total_yo[ind+1] += ya[ind];
      // printf("%d, %lf\n", *(fra+ind), *(ya+ind));
      if (ind+1 ==v_cnt){
          ind = -1;
          cnto++;
      }
      ind++;
    }
  }

  for (int i = 0; i < v_cnt; i++){
    // printf("v_min+i*5=%d, y[i]=%f\n", v_min+i*5, pow(y[i], 2));
    total_a2 += pow(pow(y[i], 2)-total_ya[i], 2);
    total_i2 += pow(pow(y[i], 2)-total_yi[i], 2);
    total_u2 += pow(pow(y[i], 2)-total_yu[i], 2);
    total_e2 += pow(pow(y[i], 2)-total_ye[i], 2);
    total_o2 += pow(pow(y[i], 2)-total_yo[i], 2);
  }

  double low2 = 10.0;
  for (int i = 0; i < v_cnt; i++){
    if (*(fra+i) < 800 && low2 > pow(y[i], 2)){
      low2 = pow(y[i], 2);
    }
    else if (*(fra+i) > 835 && *(fra+i) < 890){
      if (pow(y[i], 2) - low2 > 0.04){
        printf("e\n");
        return 0;
      }
    }
  }
  double low_a = 100;
  double high_a = 0;
  for (int i = 0; i < v_cnt; i++){
    if (*(fra+i) > 195 && *(fra+i) < 305){
      if (low_a > pow(y[i], 2)){
        low_a = pow(y[i], 2);
      }
      if (high_a < pow(y[i], 2)){
        high_a = pow(y[i], 2);
      }
    }
  }
  // printf("%f-%f\n", high_a, low_a);
  // if (high_a - low_a > 0.3){
  //   printf("aaa\n");
  //   return 0;
  // }

  // printf("%f, %f, %f, %f, %f\n", total_a2, total_i2, total_u2, total_e2, total_o2);
  double mint = min5(total_a2, total_i2, total_u2, total_o2);
  // printf("mint=%f\n", mint);
  if (mint == total_a2){
    printf("a\n");
  }else if(mint == total_i2){
    printf("i\n");
  }else if(mint == total_u2){
    printf("u\n");
  }else{
    printf("o\n");
  }

  fclose(wp);
  return 0;
}
