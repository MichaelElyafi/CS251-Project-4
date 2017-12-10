#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "pq.h"

#include "header.c"
#include "pqutil.c"




// char *Desc= "correctness test: N inserts + N get_priorities"

int test(int n, int min_at_top) {
  int *ids;
  double *priorities;
  PQ *q;
  double p;
  int id, i;
  static int first=1;

  gen_pairs(n, &ids, &priorities); 

  init_qs_rt(ids, priorities, n, n, min_at_top, &q);

  for(id=0; id<n; id++) {
    p = rand_priority(id, n);
    pq_change_priority(q, id, p); 
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

  _srand(123);

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  set_ntrials(ntrials);



  START("RUNTIME test of N inserts + N change_priority ops");

  TIME2(test(n, 1), test(n2, 1), "RUNTIME-TEST: insert+change_priority", 1, 2.5, 15.0); 

  report();

  END;
}

