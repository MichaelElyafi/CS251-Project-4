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
  _PQ *_q;
  int ret=1;

  gen_pairs(n, &ids, &priorities); 
  init_qs(ids, priorities, n, n, min_at_top, &q, &_q);

  free(ids);
  free(priorities);

  if(!empty_via_rem_by_id(q, _q))
    ret = 0;

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


  START("N inserts + N remove-by-id ops; CORRECTNESS-ONLY");

  TEST_RET_MESSAGE(test(n, 1), "CORRECTNESS-ONLY-TEST", 1, 10.0); 

  report();

  END;
}

