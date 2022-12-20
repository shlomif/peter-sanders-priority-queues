// benchmark with (insert insert delMin)^N (insert delMin delMin)^N
// just in time RNG
// version 3: use easier to handle binary heaps


#include <limits.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define DEBUGLEVEL 0
#include "util.h"
//#define KNH
//#define H2
//#define H4
//#define HSLOW

#ifdef KNH
#  include "knheap.C"
#  define HTYPE KNHeap<int, int>
#  define HINIT heap(INT_MAX, -INT_MAX)
#else
#  ifdef KNH_BINDING
#    include "knheap_c_binding.h"
#    include "knheap_c_binding.c"
#  else
#    ifdef H4
#      include "heap4.h"
#      define HTYPE Heap4<int, int>
#      define HINIT heap(INT_MAX, -INT_MAX, n)
#    else
#      ifdef H2
#        include "heap2.h"
#        define HTYPE Heap2<int, int>
#        define HINIT heap(INT_MAX, -INT_MAX, n)
#      else
#        ifdef HSLOW
#          include "heap-CLR.h"
#          define HTYPE Heap2<int, int>
#          define HINIT heap(INT_MAX, -INT_MAX, n)
#        else
#          error must define either: KNH, KNH_BINDING, H2, H4, HSLOW
#        endif
#      endif
#    endif
#  endif
#endif

#define RANDOM(n) (xrand() % (n))
static unsigned int xseed = 0x11223344;
static inline unsigned int xrand(void) {
  return (((xseed = xseed * 214013L + 2531011L) >> 16) & 0x7fffffff);
}

// generate keys
static inline void random_keys(int *keys, int count, int seed)
{
  int save_seed = xseed;
  int *array = (int*)malloc(sizeof(int) * count);
  int length = count, i;
  xseed = seed;
  for (i = 0; i < count; i++) {
    array[i] = i;
  }
  for (i = 0; i < length; i++) {
    int pos = xrand() % count;
    int key = array[pos];
    keys[i] = key;
    array[pos] = array[--count];
  }
  free(array);
  xseed = save_seed;
}

double TI, TD;
double TIR, TDR;
double TIRAND, TDRAND;
double TIRANDR, TDRANDR;
#ifdef KNH_BINDING
#define HEAP_INSERT(k, v) KNHeap__insert(heap, &k, reinterpret_cast<void*>(v))
#define HEAP_REMOVE(k, v) KNHeap__deleteMin(heap, k, v)
#define HEAP_SIZE() KNHeap__getSize(heap)
inline void onePass(void* heap, int n, int curr, int max)
#else
#define HEAP_INSERT(k, v) heap.insert(k, v)
#define HEAP_REMOVE(k, v) heap.deleteMin(k, v)
#define HEAP_SIZE() heap.size()
inline void onePass(HTYPE& heap, int n, int curr, int max)
#endif
{
  TI = 0; TD = 0;
  TIR = 0; TDR = 0;
  TIRAND = 0; TDRAND = 0;
  TIRANDR = 0; TDRANDR = 0;

  int j, k;
#ifdef KNH_BINDING
  void*v;
#else
  int v;
#endif
  unsigned long long insertSum=0, deleteSum=0;

  int *keys = (int*)malloc(sizeof(int) * n);
  if (keys == NULL) {
    std::cout << "failed to allocate keys\n";
  }
  double s_ = cpuTime();
  random_keys(keys, n, 0x11223344);
  double e_ = cpuTime();

  if (curr == 0) {
    printf("(warmup) inserting (r) ...\n");
  } else {
    printf("(%d of %d) inserting (r) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    double s = cpuTime();
    HEAP_INSERT(keys[j], keys[j]);
    double e = cpuTime();
    TIRAND += e - s;
    insertSum += keys[j];
    //std::cout << "seq [" << keys[j] << ", " << keys[j] << "] in\n";
  }
  if (curr == 0) {
    printf("(warmup) deleting (r) ...\n");
  } else {
    printf("(%d of %d) deleting (r) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    double s = cpuTime();
    HEAP_REMOVE(&k, &v);
    double e = cpuTime();
    deleteSum += k;
    TDRAND += e - s;
#ifndef KNH_BINDING
    Assert(k == j);
#endif
    //std::cout << "seq [" << k << ", " << v << "] out\n";
  }

  if (curr == 0) {
    printf("(warmup) inserting (rr) ...\n");
  } else {
    printf("(%d of %d) inserting (rr) ...\n", curr, max-1);
  }
  for (j = n-1;  j != -1;  j--) {
    double s = cpuTime();
    HEAP_INSERT(keys[j], keys[j]);
    double e = cpuTime();
    TIRANDR += e - s;
    insertSum += keys[j];
    //std::cout << "rev seq [" << keys[j] << ", " << keys[j] << "] in\n";
  }
  if (curr == 0) {
    printf("(warmup) deleting (rr) ...\n");
  } else {
    printf("(%d of %d) deleting (rr) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    double s = cpuTime();
    HEAP_REMOVE(&k, &v);
    double e = cpuTime();
    deleteSum += k;
    TDRANDR += e - s;
#ifndef KNH_BINDING
    Assert(k == j);
#endif
    //std::cout << "rev seq [" << k << ", " << v << "] out\n";
  }

  if (curr == 0) {
    printf("(warmup) inserting (s) ...\n");
  } else {
    printf("(%d of %d) inserting (s) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    keys[j] = j+1;
    double s = cpuTime();
    HEAP_INSERT(keys[j], keys[j]);
    double e = cpuTime();
    TI += e - s;
    insertSum += keys[j];
    //std::cout << "seq [" << keys[j] << ", " << keys[j] << "] in\n";
  }
  if (curr == 0) {
    printf("(warmup) deleting (s) ...\n");
  } else {
    printf("(%d of %d) deleting (s) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    double s = cpuTime();
    HEAP_REMOVE(&k, &v);
    double e = cpuTime();
    deleteSum += k;
    TD += e - s;
#ifndef KNH_BINDING
    Assert(k == j+1);
#endif
    //std::cout << "seq [" << k << ", " << v << "] out\n";
  }

  if (curr == 0) {
    printf("(warmup) inserting (sr) ...\n");
  } else {
    printf("(%d of %d) inserting (sr) ...\n", curr, max-1);
  }
  for (j = n-1;  j != -1;  j--) {
    double s = cpuTime();
    HEAP_INSERT(keys[j], keys[j]);
    double e = cpuTime();
    TIR += e - s;
    insertSum += keys[j];
    //std::cout << "rev seq [" << keys[j] << ", " << keys[j] << "] in\n";
  }
  free(keys);
  if (curr == 0) {
    printf("(warmup) deleting (sr) ...\n");
  } else {
    printf("(%d of %d) deleting (sr) ...\n", curr, max-1);
  }
  for (j = 0;  j < n;  j++) {
    double s = cpuTime();
    HEAP_REMOVE(&k, &v);
    double e = cpuTime();
    deleteSum += k;
    TDR += e - s;
#ifndef KNH_BINDING
    Assert(k == j+1);
#endif
    //std::cout << "rev seq [" << k << ", " << v << "] out\n";
  }
  Assert(HEAP_SIZE() == 0);
  Assert(deleteSum == insertSum);
  puts("done");
}


int main(int argc, char **argv)
{
  if (argc == 0) {
    puts("usage: prog <elements> <iterations (default = 1)>");
    return -1;
  }
  if (strlen(argv[1]) >= strlen("10000000000")) {
    printf("elements number length cannot be larger than %zu characters\n", strlen("10000000000"));
    puts("usage: prog <elements> <iterations (default = 1)>");
    return -1;
  }
  int n = atoi(argv[1]);
  if (n < 0) {
    puts("integer overflow from given element count");
    puts("usage: prog <elements> <iterations (default = 1)>");
    return -1;
  }
  int i;
  int repeat = 1;
  if (argc > 2) {
    repeat = atoi(argv[2]);
    if (repeat < 0) {
      puts("iterations must be 0 or greater");
      puts("usage: prog <elements> <iterations (default = 1)>");
      return -1;
    }
  }
  repeat++;

  #define DECLA(p, title) const char * field_##p = title; int len_##p = strlen(title); char ** array_##p = new char*[repeat]; for(int i = 0; i < repeat; i++) array_##p [i] = new char[4096];
  #define DELA(p, unused) for(int i = 0; i < repeat; i++) delete[] array_##p[i]; delete[] array_##p;
  DECLA(a, "iteration");
  DECLA(k, "keys");
  DECLA(b, "insert (r)");
  DECLA(c, "delete (r)");
  DECLA(d, "insert (rr)");
  DECLA(e, "delete (rr)");
  DECLA(f, "insert (s)");
  DECLA(g, "delete (s)");
  DECLA(h, "insert (rs)");
  DECLA(i, "delete (rs)");

  #define M(p) len_##p = max(len_##p, strlen(array_##p [i]))

#ifdef KNH_BINDING
  void * heap = KNHeap__create();
#else
  HTYPE HINIT;
#endif

  for (int i = 0; i < repeat; i++) {
    onePass(heap, n, i, repeat);
    i == 0 ? sprintf(array_a[i], "warmup") : sprintf(array_a[i], "%d", i);
    sprintf(array_k[i], "%d", n);
    sprintf(array_b[i], "%g", TIRAND / CLOCKS_PER_SEC);
    sprintf(array_c[i], "%g", TDRAND / CLOCKS_PER_SEC);
    sprintf(array_d[i], "%g", TIRANDR / CLOCKS_PER_SEC);
    sprintf(array_e[i], "%g", TDRANDR / CLOCKS_PER_SEC);
    sprintf(array_f[i], "%g", TI / CLOCKS_PER_SEC);
    sprintf(array_g[i], "%g", TD / CLOCKS_PER_SEC);
    sprintf(array_h[i], "%g", TIR / CLOCKS_PER_SEC);
    sprintf(array_i[i], "%g", TDR / CLOCKS_PER_SEC);
    M(a);
    M(k);
    M(b);
    M(c);
    M(d);
    M(e);
    M(f);
    M(g);
    M(h);
    M(i);
  }

#ifdef KNH_BINDING
  KNHeap__destroy(heap);
#endif

  #define F(p) len_##p, field_##p
  #define V(p) len_##p, array_##p [i]

  printf(
    "r = random\ns = sequence\nrr = random, reversed\nrs = sequence, reversed\nall fields (except for iteration) are shown in seconds\n\n"
    "%*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s\n",
    F(a), F(k), F(b), F(c), F(d), F(e), F(f), F(g), F(h), F(i)
  );

  for (int i = 0; i < repeat; i++) {
    printf(
      "%*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s  |  %*s\n",
      V(a), V(k), V(b), V(c), V(d), V(e), V(f), V(g), V(h), V(i)
    );
  }


  DELA(b, "insert (random) (seconds)");
  DELA(c, "delete (random) (seconds)");
  DELA(d, "insert (reverse, random) (seconds)");
  DELA(e, "delete (reverse, random) (seconds)");
  DELA(f, "insert (sequence) (seconds)");
  DELA(g, "delete (sequence) (seconds)");
  DELA(h, "insert (reverse, sequence) (seconds)");
  DELA(i, "delete (reverse, sequence) (seconds)");

  return 0;
}

