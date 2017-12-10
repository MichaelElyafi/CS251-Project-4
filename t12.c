#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#include "header.c"
#include "pqutil.c"





int test(int n, int min_at_top) {
  int *ids;
  double *priorities;
  PQ *q;
  double p;
  int id, i;

  gen_pairs(n, &ids, &priorities); 

  init_qs_rt(ids, priorities, n, n, min_at_top, &q);

  for(i=0; i<n; i++) {
    pq_peek_top(q, &id, &p); 
  }

  free(ids);
  free(priorities);

  pq_free(q);
  return 1;
}





int main(int argc, char *argv[]) {
  int n = __N;
  int n2 = __N2;
  int ntrials = __NTRIALS;
  clock_t budget;

  _srand(123);

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  set_ntrials(ntrials);
  cal_big_fudge();
  budget = calibrate_nlogn(n, ntrials);



  START("RUNTIME test of N inserts + N peek ops");

  TIME2(test(n, 1), test(n2, 1), "RUNTIME-TEST: insert+peek", 1, 2.5, 15.0); 

  report();

  END;
}

