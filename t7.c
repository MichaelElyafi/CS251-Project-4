#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pq.h"

#include "header.c"
#include "pqutil.c"




// char *Desc= "correctness test: N inserts + N get_priorities"

int test(int n, int min_at_top) {
  int *ids;
  double *priorities;
  PQ *q;
  _PQ *_q;
  double p;
  int id, i;
  static int first=1;
  int failures=0;

  gen_pairs(n, &ids, &priorities); 

  init_qs(ids, priorities, n/2, n, min_at_top, &q, &_q);

  for(i=0; i<n; i++) {

    id = _rand()%(n/2);
    p = rand_priority(id, (n/2));
    if(!(one_change_priority(q, _q, id, p)))
        failures++;

    id = _rand()%(n/2);
    if(!(one_remove_by_id(q, _q, id)))
        failures++;

    id = _rand()%n;
    p = rand_priority(id, n*n);
    if(!one_insert(q, _q, id, p))
      failures++;

    if(!one_del_top(q, _q))
      failures++;

    id = _rand()%n;
    if(!one_get_priority(q, _q, id))
      failures++;
  }

  printf(" MISHMASH TEST: %i pq_ operations performed; %i failed (zero is good!)\n", 
      5*n, failures);
  free(ids);
  free(priorities);
  pq_free(q);

  return failures==0;
}
          





int main(int argc, char *argv[]) {
  int n = __NSMALL;
  int n2 = __N2;
  int ntrials = __NTRIALS;

  _srand(123);
  // srand(time(NULL));

  if(argc > 1)
    n = atoi(argv[1]);
  if(argc > 2)
    ntrials = atoi(argv[2]);

  set_ntrials(ntrials);



  START("1st MISHMASH of randomized pq_ operations");

  // TIME2(test(n, 1), test(n2, 1), "RUNTIME-TEST: insert+delete", 1, 2.5, 10.0); 
  TEST_RET_MESSAGE(test(n, 1), "CORRECTNESS-ONLY-TEST", 1, 5.0); 

  report();

  END;
}

