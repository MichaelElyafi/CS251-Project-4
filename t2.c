#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#include "header.c"
#include "pqutil.c"






/*
 *
 * This particular test does the following:
 *
 *      generates n random priorities which are engineered to
 *      be distinct.
 *          (though this is not a requirement of a priority-queue, 
 *          it is a nice property for testing since there is always a 
 *          unique minimum and so only one correct outcome for various 
 *          operations).
 *
 *      Each priority is associated with a (unique) id in {0..n-1}.
 *
 *      The <id, priority> pairs are inserted in random order (i.e.,
 *        not generally from 0..n-1.) into both:
 *
 *        - the "real" priority queue being tested and
 *        - a slow reference priority queue (of type _PQ *)
 *
 *            (the insertions occur in the same sequence).
 *
 *      Finally, the two queues are emptied via a sequence of
 *      delete_top operations:
 *
 *        - the delete_min ops happen in "lock step" between the two
 *            queues.
 *        - the results of each delete_min pair are compared (they 
 *            should give the same results) and mismatch erros are flagged
 *            with a message.
 *
 *      The two queues are freed.
 *
 */
int test(int n, int min_at_top) {
  int *ids;
  double *priorities;
  PQ *q;
  _PQ *_q;
  int ret;

  gen_pairs(n, &ids, &priorities); 
  init_qs(ids, priorities, n, n, min_at_top, &q, &_q);

  free(ids);
  free(priorities);

  if(!empty_and_check(q, _q) ){
      ret = 0;
      printf("test0:  at least one ERROR (see preceding log\n");
  }
  else {
      ret = 1;
  }

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


  START("N inserts + N delete_top ops; CORRECTNESS-ONLY");

  TEST_RET_MESSAGE(test(n, 1), "CORRECTNESS-ONLY-TEST-MIN-AT-TOP", 1, 5.0); 
  TEST_RET_MESSAGE(test(n, 0), "CORRECTNESS-ONLY-TEST-B-MAX-AT-TOP", 1, 5.0); 
  report();

  END;
}

