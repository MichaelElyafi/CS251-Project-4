#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#include "header.c"
#include "pqutil.c"




char *Desc= "basic correctness test: n insertions + n deletes";


/*
 *
 *   just does trivial tests of pq_capacity and pq_size
 *
 */
int test(int n, int min_at_top) {
  int *ids;
  double *priorities;
  PQ *q;
  _PQ *_q;
  int ret=1;
  int id;
  double prio;

  gen_pairs(n, &ids, &priorities); 
  init_qs(ids, priorities, n, n, min_at_top, &q, &_q);

  if(pq_capacity(q) != n){
    printf("  pq_capacity wrong!\n");
    ret = 0;
  }
  if(pq_size(q) != n){
    printf("  pq_size wrong!\n");
    ret = 0;
  }

  pq_delete_top(q, &id, &prio);

  if(pq_size(q) != n-1){
    printf("  pq_size wrong!\n");
    ret = 0;
  }

  free(ids);
  free(priorities);

  pq_free(q);
  _pq_free(_q);
  return ret;
}





int main(int argc, char *argv[]) {
  int n = __NSMALL;
  int n2 = __N2;
  int ntrials = __NTRIALS;

  _srand(123);

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  /**
  set_ntrials(ntrials);

  cal_big_fudge();
  budget = calibrate_linear(n, ntrials);
  **/


  START("TRIVIAL pq_capacity AND pq_size TESTS");

  TEST_RET_MESSAGE(test(n, 1), "CORRECTNESS-ONLY-TEST-capacity+size", 1, 5.0); 
  report();

  END;
}

