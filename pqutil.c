
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "pq.h"


#define __N 1000
#define __N 1000
#define __N2 2000
#define __NSMALL 100
#define __NTRIALS 2000

/**************************************************/ 
/*                                                */
/*                   BEGIN                        */
/*                                                */
/*    DECLARATIONS RELATING TO TESTING SETUP      */
/*                                                */
/**************************************************/ 

typedef void (*TEST_FUNC)(int n);

typedef struct {
  TEST_FUNC tfunc;
  char * description;
} TFUNC_STRUCT;

/* INSTRUCTIONS:  
 *
 *    to add a new test function:
 *
 *         o place a "forward delclaration" for it below.
 *
 *         o add an entry to the TestFunctions array for it
 *             including a description string.  Follow the
 *             model for test functions test0 and test1
 *
 *         o (and of course write the function itself).
 *
 *    NOTE:  test functions must take a single int (n) and
 *    have void return type.
 */
void test0(int n);
void test1(int n);

TFUNC_STRUCT TestFunctions [] = 
  {
      { test0, "test0:  tests only insert+delete_top"},
      { test1, "test1:  tests only insert+change_priority+delete_top "}
  };

int NumTests = sizeof(TestFunctions)/sizeof(TFUNC_STRUCT);

/**************************************************/ 
/*                                                */
/*                   END                          */
/*                                                */
/*    DECLARATIONS RELATING TO TESTING SETUP      */
/*                                                */
/**************************************************/ 

/**************************************************/ 
/*                                                */
/*                   BEGIN                        */
/*                                                */
/*    IMPLEMENTATION OF _PQ DATA STRUCTURE        */
/*                                                */
/* Implements exact behavior as PQ in a simple    */
/*   way, but does NOT meet runtime requirements  */
/*                                                */
/* Every function from PQ mirrored here with      */
/*                                                */
/*   prefix of "_"                                */
/*                                                */
/* See pq.h for semantics                         */
/*                                                */
/**************************************************/ 


typedef struct {
  double pr;  // priority
  int    active;
} _ENTRY;

typedef struct _pq_struct {
  _ENTRY *data;
  int cap;
  int min_first;
}_PQ;


_PQ * _pq_create(int capacity, int min_heap) {

  int i;
  _PQ * q = malloc(sizeof(_PQ));

  q->data = malloc(capacity * sizeof(_ENTRY));
  q->cap = capacity;
  q->min_first = min_heap;
  for(i=0; i<q->cap; i++) {
    q->data[i].active = 0;
  }
  return q;
}


void _pq_free(_PQ * pq) {
  free(pq->data);
  free(pq);
}

int _pq_insert(_PQ * pq, int id, double priority){
  if(id < 0 || id >= pq->cap || pq->data[id].active)
    return 0;
  pq->data[id].pr = priority;
  pq->data[id].active = 1;
  return 1;
}



int _pq_change_priority(_PQ * pq, int id, double new_priority) {
  if(id < 0 || id >= pq->cap || !pq->data[id].active)
    return 0;
  pq->data[id].pr = new_priority;
  return 1;
}


int _pq_remove_by_id(_PQ * pq, int id) {
  if(id < 0 || id >= pq->cap || !pq->data[id].active)
    return 0;
  pq->data[id].active = 0;
  return 1;
}

int _pq_get_priority(_PQ * pq, int id, double *priority) {
  if(id < 0 || id >= pq->cap || !pq->data[id].active)
    return 0;
  *priority = pq->data[id].pr;
  return 1;
}

int _pq_delete_top(_PQ * pq, int *id, double *priority) {

  int top_id = -1;     // id of "top entry so-far"
  double top_p;        // associated "top priority so-far"
  double coeff = 1.0;  // for min vs. max @ top
  int i;

  if(!pq->min_first)  // top ~ max
    coeff = -1;

  for(i=0; i<pq->cap; i++) {
    if(pq->data[i].active && (top_id==-1 || coeff*(pq->data[i].pr) < coeff*top_p)){
      top_id = i;
      top_p = pq->data[i].pr;
    }
  }
  if(top_id == -1) // must be empty
    return 0;
  else {
    *priority = top_p;
    *id = top_id;
    pq->data[top_id].active = 0;
    return 1;
  }
}


int _pq_peek_top(_PQ * pq, int *id, double *priority) {
  int success;

  // cheap way to to it:
  //
  //     use delete_top and then re-activate entry

  success = _pq_delete_top(pq, id, priority);
  if(success)
    pq->data[*id].active = 1;  // cheat:  use delete_top and toggle back
                               //    to active.
  return success;
}


int _pq_capacity(_PQ * pq) {
  return pq->cap;
}

// simple scan counting active ids.
int _pq_size(_PQ * pq) {
  int n=0;
  int i;

  for(i=0; i < pq->cap; i++) {
    if(pq->data[i].active)
      n++;
  }
  return n;
}

/**************************************************/ 
/*                                                */
/*                   END                          */
/*                                                */
/*    IMPLEMENTATION OF _PQ DATA STRUCTURE        */
/*                                                */
/**************************************************/ 




/**************************************************/ 
/*                                                */
/*                   BEGIN                        */
/*                                                */
/*    TESTING FUNCTIONS INCLUDING VARIOUS         */
/*    UTILITY FUNCTIONS                           */
/*                                                */
/**************************************************/ 


/* function: empty_and_check
 * desc    : determines if two given queues contain 
 *            identical collections of <id, priority> 
 *            pairs via a sequence of delete_top operations.
 *
 * returns 1/0 for success or failure.
 *
 * detected errors logged as printed messages.
 */
int empty_and_check(PQ *q, _PQ *_q) {
  int id, _id;
  double p, _p;
  int bad;
  int passed = 1;

  if(pq_size(q) != _pq_size(_q) ) {
    printf("empty-and-check ERROR TYPE 1: mismatched q size\n");
    printf("   SUBSEQUENT TESTS WILL BE QUESTIONABLE !!\n");
    passed = 0;
  }


  while(pq_size(q) != 0){
    pq_delete_top(q, &id, &p);
    _pq_delete_top(_q, &_id, &_p);
    bad = 0;
    if(id != _id){ 
        bad = 1;
        printf("empty-and-check ERROR TYPE 2: mismatched IDs\n");
    }
    if(p != _p){ 
        bad = 1;
        printf("empty-and-check ERROR TYPE 3: mismatched priorities\n");
    }
    if(bad) {
      passed = 0;
      printf("        Expected <id=%i; p=%f>; received <id=%i, p=%f>\n", 
          _id, _p, id, p);
    }
  }
  return passed;
}



/**
 * function: change_priorities
 * desc:  performs a sequence of pq_change_priority operations
 *        to both q and _q.
 *
 *        Presumption:  both queues contain exactly same set of
 *        id-priority pairs when called and therefore will also 
 *        have the same contents afterwards.
 *
 *        Priorities are changed by whole numbers generated at random
 *        Canges may be positive or negative.
 *
 *        NOTE:  if gen_pairs was used to create initial data, altering
 *        priorities by whole numbers preserves uniqueness among priorities.
 *
 * RECOMMENDATION:  use this function as a model for other functions
 *       which "do an identical sequence of operations" on both 
 *       a PQ and a _PQ.  Then use empty_and_check to see if they
 *       really have the same contents.
 */
void change_priorities(PQ *q, _PQ *_q) {

  int n=pq_capacity(q);
  int id;

  if(_pq_size(_q) != pq_size(q)){
    printf("change_priorities ERROR TYPE 1 - size mismatch\n");
    printf("    ALL SUBSEQUENT TESTS WILL BE QUESTIONABLE\n");
  }
  for(id=0; id<n; id++) {
    double p, _p;

    if(_pq_get_priority(_q, id, &_p)) {
      if(!pq_get_priority(q, id, &p)) {

        printf("change_priorities ERROR TYPE 2 - ID %i in only one queue?\n", id);
      }
      else {
        if(_p != p) {
            printf("change_priorities ERROR TYPE 3 - priorities for ID %i do not match?\n", id);
        }
        else {


          double delta = _rand()%1000;
          if(_rand()%2)
            delta = -delta;

          p += delta;

          _pq_change_priority(_q,  id, p);
          pq_change_priority(q,  id, p);

        }
      }
    } // end if(_pq_get_priority(...))
  } // end for
}

void change_priorities_rt(PQ *q) {

  int n=pq_capacity(q);
  int id;

  for(id=0; id<n; id++) {
    double p, _p;

      if(pq_get_priority(q, id, &p)) {
          double delta = _rand()%1000;
          if(_rand()%2)
            delta = -delta;

          p += delta;

          pq_change_priority(q,  id, p);
      }
  } // end for
}


/* 
 * SUGGESTIONS:
 *
 *   SOME EMPTY FUNCTION STUBS YOU MIGHT COMPLETE TO HELP
 *   IN BUILDING INTERESTING TEST FUNCTIONS.
 *
 */




// func:  scan_get_priorities
// desc:  calls get_priority for each id in {0..cap-1}
//        on both queues and checks if results match
//
//
// return 1 if they match, zero otherwise.
//
// (unimplemented)
//
int scan_get_priorities(PQ *q, _PQ *_q) {
  double p, _p;

  int n=pq_capacity(q);
  int id;
  int found;

  for(id=0; id<n; id++) {
      found = pq_get_priority(q, id, &p);

      if(found) {
        if(!_pq_get_priority(_q, id, &_p)) {
            return 0;
        }
        if(p != _p)
          return 0;
      }
      else if(_pq_get_priority(_q, id, &_p)) {
            return 0;
      }
  }
  return 1;
}



/*
 * func: empty_via_rem_by_id
 *
 * desc:  scans all ids and calls remove_by_id on both queues 
 *     to see if they both give the same results.  
 *
 *     on success, both queues should be empty.
 *
 *     returns 1 if they match, zero otherwise.
 *
 *  (unimplemented)
 */
int empty_via_rem_by_id(PQ *q, _PQ *_q) {
  int n=pq_capacity(q);
  double p, _p;
  int found;
  int id;

  for(id=0; id<n; id++) {
      found = pq_remove_by_id(q, id);

      if(found) {
        if(!_pq_remove_by_id(_q, id)) {
            return 0;
        }
      }
      else if(_pq_remove_by_id(_q, id)) {
            return 0;
      }
  }
  return 1;

}



/********************************************************/
/* Functions below perform small tests - usually corresponding
 *   to a single priority queue operation.
 *
 *   In general, they perform an identical operation on both
 *   queues and determine if retults match.
 *
 *   Idea: use them to build bigger test suites.
 */


// func: one_del_top
//
// performs one delete_top op on both queues 
//
// expects identical results.  reports success/failure
int one_del_top(PQ *q, _PQ *_q) {
  int success, _success;
  int id, _id;
  double p, _p;
  int bad, passed;

  passed = 1;

  success = pq_delete_top(q, &id, &p);
  _success = _pq_delete_top(_q, &_id, &_p);

  if(success != _success) {
    printf("  one_del_top ERROR TYPE 1 - one queue empty, other nonempty?\n");
    return 0;
  }
  if(!success)  // both empty (match)
    return 1;
  
  // both non-empty - check results.
  bad = 0;
  if(id != _id){ 
    bad = 1;
    printf("  one_del_top ERROR TYPE 2: mismatched IDs\n");
  }
  if(p != _p){ 
    bad = 1;
    printf("  one_del_top ERROR TYPE 3: mismatched priorities\n");
  }
  if(bad) {
    passed = 0;
    printf("        Expected <id=%i; p=%f>; received <id=%i, p=%f>\n", 
        _id, _p, id, p);
  }
  return passed;
}

// func: peek_test
//
// performs one peek op on both queues 
//
// expects identical results.  reports success/failure
int peek_test(PQ *q, _PQ *_q) {
  int id, _id;
  double p, _p;
  int bad, passed;

  passed = 1;

  if(!pq_peek_top(q, &id, &p)) {
     return _pq_peek_top(_q, &_id, &_p);
  } 
  if(!_pq_peek_top(_q, &_id, &_p))
    return 0;  

  // both must have succeeded
  //
  return (id == _id) && (p == _p);

}

double rand_priority(int id, int n) {
  double x;

  x = (double)(_rand() % 100);

  x = x + ((double)id)/((double)n);

  return x;
}

// func: one_insert
//
// performs one insertion into both queues
//
// expects identical results.  reports success/failure
int one_insert(PQ *q, _PQ *_q, int id, double p) {

  if(pq_insert(q, id, p) != _pq_insert(_q, id, p)){
    // printf("  one_insert ERROR TYPE 1 - one failed, other didn't?\n");
    return 0;
  }
  return 1;
}
//
// func: one_remove_by_id
//
// performs one change_priority op on both queues using 
//    id and newp.
//
// expects identical results.  reports success/failure
int one_remove_by_id(PQ *q, _PQ *_q, int id) {

  if(pq_remove_by_id(q, id) != _pq_remove_by_id(_q, id)){
    return 0;
  }
  return 1;
}

// func: one_change_priority
//
// performs one change_priority op on both queues using 
//    id and newp.
//
// expects identical results.  reports success/failure
int one_change_priority(PQ *q, _PQ *_q, int id, double newp) {
  int success, _success;

  success = pq_change_priority(q, id, newp);
  _success =   _pq_change_priority(_q, id, newp);

  if(success != _success) {
    /*
    printf("  one_change_priority ERROR TYPE 1 - one succeeded;other didn't?\n");
    printf("     id: %i\n", id);
    return 0;
    */
  }
  return 1;
}

// func: one_get_priority
//
// performs one get_priority op on both queues using 
//    id 
//
// expects identical results.  reports success/failure
int one_get_priority(PQ *q, _PQ *_q, int id) {
  int success, _success;
  double p, _p;

  success = pq_get_priority(q, id, &p);
  _success =  _pq_get_priority(_q, id, &_p);
 
  if(success != _success) {
    printf("  one_get_priority ERROR TYPE 1 - one succeeded, other failed?\n");
    printf("     id: %i\n", id);
    return 0;
  }
  if(!success)
    return 1;  // id in neither queue (match)

  // id in both - check priorities
  if(p != _p){ 
    printf("  one_get_priority ERROR TYPE 2: mismatched priorities\n");
    printf("        id=%i; retrieved priorities: p=%f ; _p=%f>\n", 
              id, p,  _p);
    return 0;
  }
  return 1;
}


/******************************************************************/
/**** 
 *  MAJOR INITIALIZATION FUNCTIONS BELOW FOR BUILDING PRIORITY QUEUES 
 *  BELOW
 */

/**
 * func:  gen_pairs
 * desc:  allocates and populates two arrays of length n.
 *        The first is a permutation of {0..n-1}
 *
 *        The priorities are generated so that they are all distinct
 *        (idea:  if priorities are distinct, there is only one possible
 *        outcome for most/all operations -- unique minimum, etc.)
 *
 * typical usage:  generate data to pass to init_qs to seed two identical
 *    queues.
 */
void gen_pairs(int n, int **p_ids, double **p_priorities) {

  int *ids = malloc(n*sizeof(int));
  double *priorities = malloc(n*sizeof(double));
  int i, j, tmp;

  for(i=0; i<n; i++) {
    ids[i] = i;
    priorities[i] = _rand()%100 + i/(double)n;
  }
  for(i=0; i<n; i++) {
    j = _rand()%n;
    tmp = ids[i];
    ids[i] = ids[j];
    ids[j] = tmp;
  }
  *p_ids = ids;
  *p_priorities = priorities;
}



/**
 * func:  init_qs ("initialize queues")
 * desc:  creates a PQ and a _PQ and populates both
 *        with the same colection of <id,priority> pairs
 *        as given by the two arrays ids[] and p[].
 *
 *        Pairs <ids[i], p[i]> are inserted into initially 
 *        empty queues.
 *
 *        Upon completion, both queues should have identical
 *        contents.
 *
 *        Note1:  caller passes double pointers.
 */
void init_qs(int ids[], double p[], int n, int cap, 
                int minAtTop, PQ **q, _PQ **_q) {

  int i;
  *q = pq_create(cap, minAtTop);
  *_q = _pq_create(cap, minAtTop);


  for(i=0; i<n; i++) {
    pq_insert(*q, ids[i], p[i]);
    _pq_insert(*_q, ids[i], p[i]);
  }

}

void init_qs_rt(int ids[], double p[], int n, int cap, 
                int minAtTop, PQ **q) {

  int i;
  *q = pq_create(cap, minAtTop);


  for(i=0; i<n; i++) {
    pq_insert(*q, ids[i], p[i]);
  }

}


/**************************************************/ 
/*                                                */
/*                   BEGIN                        */
/*                                                */
/*         "REGISTERED" TEST FUNCTIONS            */
/*                                                */
/* These functions are entry points for tests     */
/*  launched from main and registered in          */
/*  TestFunctions[]                               */
/*                                                */
/* They utilize the utility/helper functions above*/
/*                                                */
/**************************************************/ 

/*
 * func: test0
 *
 * Example of a test function.
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
void test0(int n) {
  int *ids;
  double *priorities;
  PQ *q;
  _PQ *_q;

  gen_pairs(n, &ids, &priorities); 
  init_qs(ids, priorities, n, n, 1, &q, &_q);

  free(ids);
  free(priorities);

  if(!empty_and_check(q, _q) )
      printf("test0:  at least one ERROR (see preceding log\n");
  else 
      printf("test0:  PASSED? (check preceding log)\n");

  pq_free(q);
  _pq_free(_q);
}

/**
 * test1:  another model test.
 *
 *    Setup is the same as test0 EXCEPT:
 *
 *        after performing insertions, a sequence of randomized 
 *        change_priority operations are performed on each queue 
 *        (again, in "lock-step").
 *
 *    The change_priority operations are engineered so that:
 *
 *       - both increases and decreases in priorities can happen.
 *
 *       - the priorities remain unique:
 *
 *            Each initial priority (produced by gen_pairs) is
 *            engineered to have a unique fractional component (after
 *            the decimal place).
 *
 *            The "deltas" in priorities are always whole numbers (integers)
 *            thus preserving the uniqueness of priorities.
 *
 *            (both positive and negative "deltas" are produced)
 *
 *    After these operations, the two queues are emptied and compared.
 *
 */
void test1(int n) {
  int *ids;
  double *priorities;
  PQ *q;
  _PQ *_q;

  gen_pairs(n, &ids, &priorities); 
  init_qs(ids, priorities, n, n, 1, &q, &_q);

  free(ids);
  free(priorities);

  change_priorities(q, _q);

  if(!empty_and_check(q, _q) )
      printf("test1:  at least one ERROR (see preceding log\n");
  else 
      printf("test1:  PASSED? (check preceding log)\n");

  pq_free(q);
  _pq_free(_q);

}

/*
 * func:  run_test
 * desc:  prints banner information and then runs test function 
 *        specified by tnum.
 *
 *        test functions are numbered starting from zero and corresponding
 *        to their index in the global TestFunctions array.
 */
void run_test(int tnum, int n) {

  if(tnum < 0 || tnum >= NumTests){
    printf("ERROR:  no test run - no test number %i exists\n", tnum);
    return;
  }

  printf("------------------------------------------------------\n");
  printf("- STARTING TEST FUNCTION %i                         -\n", tnum);
  printf("- DESCRIPTION:                                       -\n");
  printf("-                                                    -\n");
  printf("  '%s'\n", TestFunctions[tnum].description);
  printf("-                                                    -\n");
  printf("-                                                    -\n");

  TestFunctions[tnum].tfunc(n);
  printf("------------------------------------------------------\n");
  printf("- TEST FUNCTION %i COMPLETE                         -\n", tnum);
  printf("------------------------------------------------------\n\n");

}


/*
 * func:  parse_non-neg_ing
 * desc: simple utility function to aid in parsing command line args
 */
/***
int parse_non_neg_int(int argc, char *argv[], int i) {
  int x;
  
  if(i >= argc) return -1;

  if(sscanf( argv[i], "%i", &x) != 1 || x < 0) {
    printf("  USAGE ERROR:  expected non-neg int; received %s\n", argv[i]);
    printf("     TRY AGAIN... GOODBYE\n");
    abort();
  }
  return x;
}

***/

/* 
 * command line options:
 *
 *
 *    to specify "problem size" (default=10):
 *
 *       -n <N>
 *   
 *    to specify seed for pseudo-random number gen:
 *
 *       -s <SEED>
 *
 *       (default seed retrieved from time(NULL) )
 *
 *    to specify test function to run:
 *
 *       -t <TEST_FUNC_NUMBER>
 *
 *       (test functions are numbered starting from 0)
 *
 *    to run ALL test functions:
 *
 *        -all
 *
 */
/***
int main(int argc, char *argv[]) {
  int tnum=0;
  int t;
  int n=10;
  int i, all_tests;
  unsigned seed;


  seed = time(NULL) % 10000;

  all_tests=0;
  i=1;

  while(i < argc) {
    if(strcmp(argv[i], "-n") == 0) {
      i++;
      n = parse_non_neg_int(argc, argv, i);
    }
    else if(strcmp(argv[i], "-t") == 0) {
      i++;
      tnum = parse_non_neg_int(argc, argv, i);
    }
    else if(strcmp(argv[i], "-s") == 0) {
      i++;
      seed = parse_non_neg_int(argc, argv, i);
    }
    else if(strcmp(argv[i], "-all") == 0) {
      all_tests = 1;
    }
    else {
      printf("Usage err:  unknown switch '%s'\n", argv[i]);
      printf("    ..... GOODBYE\n");
      abort();
    }
    i++;
  }

  srand(seed);

  printf("**********  STARTING TESTS ****************\n");
  printf("*                                         *\n");
  printf("            n:  %i\n", n);
  printf("*                                         *\n");
  printf("   srand seed:  %u\n", seed);
  printf("*                                         *\n");
  printf("    To reproduce this test, use \n");
  printf("*                                         *\n");
  printf("       -s %u\n", seed);
  printf("*                                         *\n");
  printf("    on future runs\n");
  printf("*                                         *\n");
  printf("   Test(s) to be run: \n");
  if(all_tests) {
      printf("         0..%i\n", NumTests);
  }
  else {
      printf("         %i\n", tnum);
  }
  printf("*                                         *\n");
  printf("*******************************************\n\n");

  if(all_tests) {
    for(t = 0; t<NumTests; t++) {
      run_test(t, n);
    }
  }
  else {
    run_test(tnum, n);
  }
  return 0;
}
**/









