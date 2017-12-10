
/*
 * basic description:  
 *
 *     macros and calibration code for building auto-graders
 *     checking for both expected behavior and meeting
 *     runtime requirements.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int TestNum = 1;
int NPassed = 0;
int NFailed = 0;

float Score = 0;
float MaxPts = 0;
float ThisScore = 0;
float ThisMaxPts = 0;

float MaxPtsAuto= 0;
float MaxPtsManual= 0;

float PtsPer = 1.0;

char *CurrFunc;
char *CurrDesc;

typedef struct {
  int a, b, c;
} JUNK;

// #define TIME_TRIALS 100000

int TIME_TRIALS = 100;

void set_ntrials(int ntrials) {
  TIME_TRIALS = ntrials;
}

static unsigned long __last_rand;
static unsigned long __a = 1664525;
static unsigned long __m = 4294967296;
static unsigned long __c = 1013904223;

void _srand(long x) {
  __last_rand = x;
}

unsigned long _rand(){

  __last_rand = (__a*__last_rand + __c) % __m;

  return __last_rand;
}



#define TA_MESSAGE(msg) { \
	printf("  #### TA-TODO : #####\n"); \
	printf("  ####    %s\n", msg); \
	printf("  #### END TA-TODO ###\n"); } \
	

#define TEST_RET(func_call, answer, pts) { \
	int ret; \
	printf("  ###  TEST %i:  %s; %.2f points\n", TestNum, "\""#func_call"\"",  pts); \
	ret = func_call;  \
	printf("     Returned Value      : %i\n", ret); \
	printf("     Correct Return Value: %i\n", answer); \
	if(ret == answer) {NPassed++; ThisScore+=pts; Score += pts; printf("     PASSED\n");} \
	else {NFailed++; printf("     FAILED\n");} \
	printf("     Cummulative Score(%s): %.2f\n", CurrDesc, ThisScore); \
	printf("  ###  END TEST %i\n\n", TestNum); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	TestNum++; \
}

#define TEST_RET_MESSAGE(func_call, msg, answer, pts) { \
	int ret; \
	printf("  ###  TEST %i:   %.2f points\n", TestNum,  pts); \
	printf("          DESC: %s\n", msg); \
	ret = func_call;  \
	printf("     Returned Value      : %i\n", ret); \
	printf("     Correct Return Value: %i\n", answer); \
	if(ret == answer) {NPassed++; ThisScore+=pts; Score += pts; printf("     PASSED\n");} \
	else { NFailed++; printf("     FAILED\n");} \
	printf("  ###  END TEST %i\n\n", TestNum); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	printf("     Score So Far For This Test Suite: %.2f / %.2f\n", ThisScore, MaxPts); \
	TestNum++; \
}

#define TIME2(func_callA, func_callB, desc, answer, max_ratio, pts) { \
	int i, retA, retB; \
  double ratio; \
	clock_t start, end, diffA, diffB; \
	printf("  ###  TEST %i (TIMED):  %s; %.2f points\n", TestNum, "\""#func_callA"\"",  pts); \
  printf("  DESCRIPTION: %s\n", desc); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	ThisMaxPts += pts; \
	start  = clock(); \
	for(i=0; i<TIME_TRIALS; i++) { \
	  retA = func_callA; \
	  if(retA != answer) break; \
	} \
	end = clock(); \
	diffA = end-start; \
	if(retA != answer){ \
	    printf("     FAILED; \n"); \
	    printf("     Returned Value      : %i\n", retA); \
	    printf("     Correct Return Value: %i\n", answer); \
	    NFailed++; \
	} \
  else { \
    start = clock(); \
    for(i=0; i<TIME_TRIALS; i++) { \
      retB = func_callB; \
        if(retB != answer) break; \
    } \
    end = clock(); \
    if(retB != answer){ \
          printf("     FAILED; \n"); \
          printf("     Returned Value      : %i\n", retB); \
          printf("     Correct Return Value: %i\n", answer); \
          NFailed++; \
    } \
    else { \
      diffB = end - start; \
      ratio = ((double)(diffB))/(diffA); \
      if(ratio > max_ratio) { \
          printf("     FAILED \n"); \
          printf("     TOO SLOW!\n"); \
          printf("     MAX RATIO T(2N)/T(N):       %f\n", max_ratio); \
          printf("     OBSERVED RATIO T(2N)/T(N):  %f\n", ratio); \
          printf("     (NUM-TRIALS:       %i\n )", TIME_TRIALS); \
          NFailed++; \
      } \
	    else { \
          printf("     MAX RATIO T(2N)/T(N):       %f\n", max_ratio); \
          printf("     OBSERVED RATIO T(2N)/T(N):  %f\n", ratio); \
	        Score += pts; \
	        NPassed++; \
	        ThisScore += pts; \
      } \
	  } \
	} \
	printf("     Score So Far For Function: %.2f / %.2f\n", ThisScore, MaxPts); \
	printf("  ###  END TEST %i\n\n", TestNum); \
	TestNum++;\
}

#define TIME(func_call, desc, answer, max_time, pts) { \
	int i, ret; \
	clock_t start, end, diff; \
	start  = clock(); \
	printf("  ###  TEST %i (TIMED):  %s; %.2f points\n", TestNum, "\""#func_call"\"",  pts); \
  printf("  DESCRIPTION: %s\n", desc); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	ThisMaxPts += pts; \
	for(i=0; i<TIME_TRIALS; i++) { \
	  ret = func_call; \
	  if(ret != answer) break; \
	} \
	end = clock(); \
	diff = end-start; \
	if(ret != answer){ \
	    printf("     FAILED; \n"); \
	    printf("     Returned Value      : %i\n", ret); \
	    printf("     Correct Return Value: %i\n", answer); \
	    NFailed++; \
	} \
	else if(diff > max_time) { \
	    printf("     FAILED \n"); \
	    printf("     TOO SLOW!\n"); \
	    printf("     MAX-TIME ALLOWED:  %ld\n", max_time); \
	    printf("     TIME TAKEN:        %ld\n", diff); \
	    printf("     (NUM-TRIALS:       %i\n", TIME_TRIALS); \
	    NFailed++; \
	} \
	else { \
	    printf("     PASSED\n"); \
	    printf("     MAX-TIME ALLOWED:  %ld\n", max_time); \
	    printf("     TIME TAKEN:        %ld\n", diff); \
	    Score += pts; \
	    NPassed++; \
	    ThisScore += pts; \
	} \
	printf("     Score So Far For This Test Suite: %.2f / %.2f\n", ThisScore, MaxPts); \
	printf("  ###  END TEST %i\n\n", TestNum); \
	TestNum++;\
}


#define TIME_ONCE(func_call, msg, answer, max_time, pts) { \
	int i, ret; \
	clock_t start, end, diff; \
	start  = clock(); \
	printf("  ###  TEST %i (TIMED):   %.2f points\n", TestNum,  pts); \
	printf("        DESC: %s\n",  msg); \
	MaxPts += pts; \
	MaxPtsAuto += pts; \
	  ret = func_call; \
	end = clock(); \
	if(ret != answer){ \
	    printf("     FAILED; \n"); \
	    printf("     Returned Value      : %i\n", ret); \
	    printf("     Correct Return Value: %i\n", answer); \
	} \
	diff = end-start; \
	if(diff > max_time) { \
	    printf("     FAILED \n"); \
	    printf("     TOO SLOW!\n"); \
	    printf("     MAX-TIME ALLOWED:  %ld\n", max_time); \
	    printf("     TIME TAKEN:        %ld\n", diff); \
	    NFailed++; \
	} \
	else { \
	    printf("     PASSED\n"); \
	    printf("     MAX-TIME ALLOWED:  %ld\n", max_time); \
	    printf("     TIME TAKEN:        %ld\n", diff); \
	    Score += pts; \
	    NPassed++; \
	    ThisScore += pts; \
	} \
	printf("     Score So Far For This Test Suite: %.2f / %.2f\n", ThisScore, MaxPts); \
	printf("  ###  END TEST %i\n\n", TestNum); \
	TestNum++;\
}
	
	
#define MANUAL(func_call, expected, pts) { \
	printf("  ###  TEST %i:  %s; %.2f points\n", TestNum, "\""#func_call"\"",  pts); \
	printf("  MANUAL TA TODO:  DETERMINE IF OUTPUT MATCHES EXPECTED AND ASSIGN SCORE\n\n");\
	printf("    EXPECTED OUTPUT: \n\n"); \
	printf("        %s \n\n", expected); \
	printf("    ACTUAL OUTPUT: \n\n"); \
	func_call; \
	printf("    \n END STUDENT OUTPUT: \n\n"); \
	printf("     TA REPORTED SCORE: ____ / %.2f\n", pts); \
	printf("  ###  END TEST %i\n\n", TestNum); \
	TestNum++;\
	MaxPts += pts; \
	MaxPtsManual += pts; \
}


	
	


#define START(desc) { \
      printf("~~~~ BEGIN TEST SUITE.  DESC: %s ~~~~~\n", desc); \
      ThisScore = 0.0; \
      TestNum = 1; \
      CurrDesc = desc; \
}
	

	

#define END { \
    printf("~~~~ END TESTS OF %s ~~~~~\n", CurrDesc); \
    printf("~~~~     AUTO__SCORE:      %.2f / %.2f\n",  ThisScore, MaxPts); \
    ThisScore = 0.0; ThisMaxPts=0.0; \
    cleanup(); \
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"); }

#define END_MANUAL { \
    printf("~~~~ END TESTS OF %s ~~~~~\n", CurrDesc); \
    printf("~~~~   TA TODO:  ENTER TOTAL POINTS FOR MANUAL TESTS ABOVE\n\n"); \
    printf("~~~~   MANUAL__SCORE(%21s):   ______ / %.2f\n", CurrDesc,  ThisMaxPts); \
    ThisScore = 0.0; ThisMaxPts=0.0; \
    cleanup(); \
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n"); }

void report() {
	printf("\n\n #####   TESTS_COMPLETED ######  \n");
	printf("\n\n    FINAL RESULTS OF TESTS:  \n");

	printf("\n\n        AUTOSCORED:    %.2f / %.2f\n", Score, MaxPtsAuto);

	printf("\n\n          AUTO TESTS PASSED:  %i\n", NPassed);
	printf("          AUTO TESTS FAILED:  %i\n", NFailed);

	if(MaxPtsManual > 0.0){
		printf("\n\n        POINTS NEEDING HUMAN EVAL:   %.2f \n\n", MaxPtsManual);
		printf("\n\n        TA INSTRUCTIONS:  TALLY HUMAN-SCORED TOTAL BELOW\n\n");

		printf("\n\n            HUMAN-EVAL:    ______ / %.2f\n\n", MaxPtsManual);

		printf("\n\n            GRAND-TOTAL:    ______ / %.2f\n\n", MaxPtsManual+MaxPts);

		printf("\n\n            TA DOING EVALUATION:  ____________________________\n\n");
	}
}

int baseline(){
  return 1;
}

void cleanup(){
int i;
}

  

void parray(int a[], int n) {
int i;

  printf("[ ");
  for(i=0; i<n; i++) {
    printf(" %i ", a[i]);
  }
  printf("]\n");
}


  

  
  
int arrays_eq(int *a, int *b, int n) {
int i;

  for(i=0; i<n; i++){ 
	if(a[i] != b[i])
	  return 0;
  }
  return 1;
}


int num_occ(int a[], int n, int x) {
  int matches=0;
  int i;

  for(i=0; i<n; i++) {
    if(a[i]==x) matches++;
  }
  return matches;
}

int same_contents_arr(int a[], int b[], int n) {
int i, x;

  for(i=0; i<n; i++) {
	x = a[i];
	if(num_occ(a, n, x) != num_occ(b, n, x))
		return 0;
  }
  return 1;
}






clock_t Fudge = 1;


void cal_unit_fudge() {
  Fudge = 1;
}
void cal_big_fudge() {
  Fudge = 12;
}
void cal_huge_fudge() {
  Fudge = 24;
}
void cal_custom_fudge(clock_t fudge) {
  Fudge = fudge;
}



clock_t clk_granularity() {
  int i;
  clock_t start, end;

  start = clock();
  do{
    end = clock();
  }
  while(end==start);

  return end-start;
}

void check_budget(clock_t budget) {
  clock_t units;

  clock_t g = clk_granularity();
  units = budget/g;
  if(units < 1000) {
    printf("++++++ WARNING  WARNING  WARNING  ++++++\n");
    printf("++++                                ++++\n");
    printf("       runtime budget may be too small\n");
    printf("       for measured clock resolution:\n");
    printf("          budget amounts to only %lu measurable time units\n", units);
    printf("RECOMMENDATION:  increase problem size and/or\n");
    printf("                 trials (preferably trials)\n");
    printf("++++++        END WARNING         ++++++\n\n");
  }

}

clock_t calibrate_constant(int ntrials) {
clock_t start, end, diff;
JUNK * a;
int i;

  start = clock();
  while(ntrials) {
    a = malloc(sizeof(JUNK));
    a->a = 1;
    a->b = 1;
    a->c = 1;
    free(a);
    ntrials--;
  }
  end = clock();

  diff = end - start;
  check_budget(Fudge*diff);
  return Fudge * diff;
}

clock_t calibrate_linear(int n, int ntrials) {
  clock_t start, end, diff;
  JUNK **a;
  int i;

  start = clock();
  while(ntrials) {
    a = malloc(n*sizeof(JUNK *));
    for(i=0; i<n; i++) {
      a[i] = malloc(sizeof(JUNK));
      a[i]->a = 1;
      a[i]->b = 1;
      a[i]->c = 1;
    }
    for(i=0; i<n; i++) {
      free(a[i]);
    }
    free(a);
    ntrials--;
  }
  end = clock();

  diff = end - start;
  check_budget(Fudge*diff);
  return Fudge * diff;
}
  
void logn(int idx, JUNK **a) {

  a[idx]->a = 3*idx;
  a[idx]->b = 3*idx+a[idx]->a;
  a[idx]->c = (a[idx]->a)*(a[idx]->b);
  
  if(idx > 0)
    logn(idx/2, a);
}


clock_t calibrate_nlogn(int n, int ntrials) {
  clock_t start, end, diff;
  JUNK **a;
  int i, k;

  start = clock();
  while(ntrials) {
    a = malloc(n*sizeof(JUNK *));
    for(i=0; i<n; i++) {
      a[i] = malloc(sizeof(JUNK));
    }
    for(i=0; i<n; i++) {
      k = n;
      logn(n-1, a);
    }
    for(i=0; i<n; i++) {
      free(a[i]);
    }
    free(a);
    ntrials--;
  }
  end = clock();

	diff = end - start;
  check_budget(Fudge*diff);
  return Fudge * diff;
}
  




  



