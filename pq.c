#include "pq.h"
#include <stdio.h>
#include <stdlib.h>

/**
* General description:  priority queue which stores pairs
*   <id, priority>.  Top of queue is determined by priority
*   (min or max depending on configuration).
*
*   There can be only one (or zero) entry for a particular id.
*
*   Capacity is fixed on creation.
*
*   IDs are integers in the range [0..N-1] where N is the capacity
*   of the priority queue set on creation.  Any values outside this
*   range are not valid IDs.
**/
struct pq_node {
	double priority;		//priority queue which stores pairs <id, priority>.Top of queue is determined by priority (min or max depending on configuration).
	int id;					//IDs are integers in the range [0..N-1] where N is the capacity of the priority queue set on creation.Any values outside this range are not valid IDs.
	int location;			//Location of the index
};
typedef struct pq_node NODE;

struct pq_struct{
	int count;				//Counting integer
	int min_heap;			//if 1 (really non - zero), then it is a min - heap if 0, then a max - heap
	int capacity;			//Capacity is fixed on creation.
	NODE **index;			//Index of the heap
	NODE *heap;				//Holds the id and priority in an array
	// YOU DECIDE WHAT TO PUT IN HERE TO ENCAPSULATE
	//   A PRIORITY QUEUE!
};
// YOU IMPLEMENT ALL OF THE FUNCTIONS SPECIFIED IN
//   pq.h BELOW!

/**
* Function: pq_create
* Parameters: capacity - self-explanatory
*             min_heap - if 1 (really non-zero), then it is a min-heap
*                        if 0, then a max-heap
*
* Returns:  Pointer to empty priority queue with given capacity and
*           min/max behavior as specified.
*
*/
PQ * pq_create(int capacity, int min_heap){
	if (capacity <= 0){
		exit(1);
	}
	PQ *pq = malloc(sizeof(struct pq_struct));
	pq->count = 0;
	pq->capacity = capacity;
	pq->min_heap = min_heap;
	pq->heap = malloc(sizeof(NODE)* (pq->capacity + 1));
	pq->heap[0].priority = 0;
	pq->heap[0].id = 0;
	pq->index = malloc(sizeof(NODE)* (pq->capacity));
	int i;
	for (i = 0; i < capacity + 1; i++){
		pq->index[i] = NULL;
	}
	return pq;
}

/**
* Function: pq_free
* Parameters: PQ * pq
* Returns: --
* Desc: deallocates all memory associated with passed priority
*       queue.
*
*/
void pq_free(PQ * pq){
	free(pq->index);
	free(pq->heap);
	free(pq);
}

/**
* Function: bubble_up
* Parameters: PQ * pq			: for the heap
*			  int count			: idx of element to perc-up
* Returns:    pq
*
* pq->heap[count].priority		: heap contents
*/
PQ *bubble_up(PQ * pq, int i){
	double new_priority = pq->heap[i].priority;
	int new_id = pq->heap[i].id;
	int p;
	p = i / 2;
	if (pq->min_heap != 0){
		while (p >= 1 && new_priority < pq->heap[p].priority) {
			pq->heap[i].priority = pq->heap[p].priority;
			pq->heap[i].id = pq->heap[p].id;
			pq->index[pq->heap[p].id] = &(pq->heap[i]);
			i = p;
			p = i / 2;
		}
	}
	if (pq->min_heap == 0){
		while (p >= 1 && new_priority > pq->heap[p].priority) {
			pq->heap[i].priority = pq->heap[p].priority;
			pq->heap[i].id = pq->heap[p].id;
			pq->index[pq->heap[p].id] = &(pq->heap[i]);
			i = p;
			p = i / 2;
		}
	}
	pq->heap[i].priority = new_priority;
	pq->heap[i].id = new_id;
	pq->index[new_id] = &(pq->heap[i]);
	return pq;
}

/**
* Function: bubble_down
* Parameters: PQ * pq			: for the heap
*			  int i				: idx of element to perc-down
* Returns:	  pq
*
* pq->heap[count].priority		: heap contents
*/
PQ *bubble_down(PQ * pq, int i){

	double old_priority;
	int old_id;
	int old_location;

	double new_priority;
	int new_id;
	int new_location;

	int p;
	p = min_max_child_idx(pq, i);
	if (pq->min_heap != 0){
		while (p != -1 && pq->heap[p].priority < pq->heap[i].priority) {
			old_id = pq->heap[i].id;
			old_priority = pq->heap[i].priority;
			old_location = pq->heap[i].location;
			new_id = pq->heap[p].id;
			new_priority = pq->heap[p].priority;
			new_location = pq->heap[p].location;

			pq->heap[p].id = old_id;
			pq->heap[p].priority = old_priority;
			pq->heap[p].location = new_location;
			pq->heap[i].id = new_id;
			pq->heap[i].priority = new_priority;
			pq->heap[i].location = old_location;

			pq->index[new_id] = &(pq->heap[i]);
			pq->index[old_id] = &(pq->heap[p]);
			i = p;
			p = min_max_child_idx(pq, i);
		}
	}
	if (pq->min_heap == 0){
		while (p != -1 && pq->heap[p].priority > pq->heap[i].priority) {
			old_id = pq->heap[i].id;
			old_priority = pq->heap[i].priority;
			old_location = pq->heap[i].location;
			new_id = pq->heap[p].id;
			new_priority = pq->heap[p].priority;
			new_location = pq->heap[p].location;

			pq->heap[p].id = old_id;
			pq->heap[p].priority = old_priority;
			pq->heap[p].location = new_location;
			pq->heap[i].id = new_id;
			pq->heap[i].priority = new_priority;
			pq->heap[i].location = old_location;

			pq->index[new_id] = &(pq->heap[i]);
			pq->index[old_id] = &(pq->heap[p]);
			i = p;
			p = min_max_child_idx(pq, i);
		}
	}
	return pq;
}

/**
* Function: min_max_child_idx
* Parameters: PQ * pq			: for the heap
*			  int count			: idx of element
*
* Returns:	index of smallest child or biggest child of root
*			(if any).
*			if count is a leaf, -1 is returned
*
* pq->heap[count].priority		: heap contents
*/
int min_max_child_idx(PQ * pq, int count) {
	int left, right, min_max;

	left = 2 * count;
	right = left + 1;

	if (left > pq->count){
		return -1;
	}
	if (left <= pq->count){
		min_max = left;
	}
	if (pq->min_heap != 0){
		if (right <= pq->count && pq->heap[left].priority > pq->heap[right].priority){
			min_max = right;
		}
	}
	if (pq->min_heap == 0){
		if (right <= pq->count && pq->heap[left].priority < pq->heap[right].priority){
			min_max = right;
		}
	}
	return min_max;
}

/**
* Function: pq_insert
* Parameters: priority queue pq
*             id of entry to insert
*             priority of entry to insert
* Returns: 1 on success; 0 on failure.
*          fails if id is out of range or
*            there is already an entry for id
*          succeeds otherwise.
*
* Desc: self-explanatory
*
* Runtime:  O(log n)
*
*/
int pq_insert(PQ * pq, int id, double priority){
	if ((pq->count + 1) > pq->capacity || id >= pq->capacity || id < 0 || pq->index[id] != NULL){
		return 0;
	}
	pq->count++;
	pq->heap[pq->count].location = pq->count;
	pq->heap[pq->count].priority = priority;
	pq->heap[pq->count].id = id;
	pq->index[id] = &(pq->heap[pq->count]);
	pq = bubble_up(pq, pq->count);
	pq->heap[pq->count].location = pq->count;
	return 1;
}

/**
* Function: pq_change_priority
* Parameters: priority queue ptr pq
*             element id
*             new_priority
* Returns: 1 on success; 0 on failure
* Desc: If there is an entry for the given id, its associated
*       priority is changed to new_priority and the data
*       structure is modified accordingly.
*       Otherwise, it is a failure (id not in pq or out of range)
* Runtime:  O(log n)
*
*/
int pq_change_priority(PQ * pq, int id, double new_priority){
	if (id >= pq->capacity || id < 0 || pq->index[id] == NULL || new_priority == pq->index[id]->priority){
		return 0;
	}
	if ((pq->min_heap != 0 && new_priority > pq->index[id]->priority) || (pq->min_heap == 0 && new_priority < pq->index[id]->priority)){
		pq->index[id]->priority = new_priority;
		pq = bubble_down(pq, pq->index[id]->location);
	}
	if ((pq->min_heap != 0 && new_priority < pq->index[id]->priority) || (pq->min_heap == 0 && new_priority > pq->index[id]->priority)){
		pq->index[id]->priority = new_priority;
		pq = bubble_up(pq, pq->index[id]->location);
	}
	return 1;
}

/**
* Function: pq_remove_by_id
* Parameters: priority queue pq,
*             element id
* Returns: 1 on success; 0 on failure
* Desc: if there is an entry associated with the given id, it is
*       removed from the priority queue.
*       Otherwise the data structure is unchanged and 0 is returned.
* Runtime:  O(log n)
*
*/
int pq_remove_by_id(PQ * pq, int id){

	if (pq->count == 0 || id >= pq->capacity || id < 0 || pq->index[id] == NULL){
		return 0;
	}
	double old_priority;
	int old_location;
	double new_priority;
	int new_id;
	
	if (pq->index[id] != NULL){
		old_priority = pq->index[id]->priority;
		old_location = pq->index[id]->location;

		new_priority = pq->heap[pq->count].priority;
		new_id = pq->heap[pq->count].id;

		pq->heap[pq->index[id]->location].priority = new_priority;
		pq->heap[pq->index[id]->location].id = new_id;

		pq->heap[pq->count].priority = 0;
		pq->heap[pq->count].id = 0;
		pq->index[new_id]->location = old_location;

		pq->count--;
		if ((pq->min_heap != 0 && new_priority > old_priority) || (pq->min_heap == 0 && new_priority < old_priority)){
			pq = bubble_down(pq, pq->index[new_id]->location);
		}
		if ((pq->min_heap != 0 && new_priority < old_priority) || (pq->min_heap == 0 && new_priority > old_priority)){
			pq = bubble_up(pq, pq->index[new_id]->location);
		}
	}
	pq->index[id] = NULL;
	return 1;
}

/**
* Function: pq_get_priority
* Parameters: priority queue pq
*             elment id
*             double pointer priority ("out" param)
* Returns: 1 on success; 0 on failure
* Desc: if there is an entry for given id, *priority is assigned
*       the associated priority and 1 is returned.
*       Otherwise 0 is returned and *priority has no meaning.
* Runtime:  O(1)
*
*/
int pq_get_priority(PQ * pq, int id, double *priority){
	if (id >= pq->capacity || id < 0 || pq->index[id] == NULL){
		return 0;
	}
	*priority = pq->index[id]->priority;
	return 1;
}

/**
* Function: pq_delete_top
* Parameters: priority queue pq
*             int pointers id and priority ("out" parameters)
* Returns: 1 on success; 0 on failure (empty priority queue)
* Desc: if queue is non-empty the "top" element is deleted and
*       its id and priority are stored in *id and *priority;
*       The "top" element will be either min or max (wrt priority)
*       depending on how the priority queue was configured.
*
*       If queue is empty, 0 is returned.
*
* Runtime:  O(log n)
*
*
*/
int pq_delete_top(PQ * pq, int *id, double *priority){
	if (pq->count == 0 || id < 0){
		return 0;
	}
	*id = pq->heap[1].id;
	*priority = pq->heap[1].priority;
	pq_remove_by_id(pq, pq->heap[1].id);
	return 1;
}

/**
* Function: pq_peek_top
* Parameters: priority queue pq
*             int pointers id and priority ("out" parameters)
* Returns: 1 on success; 0 on failure (empty priority queue)
* Desc: if queue is non-empty information about the "top"
*       element (id and priority) is stored in *id and *priority;
*       The "top" element will be either min or max (wrt priority)
*       depending on how the priority queue was configured.
*
*       The priority queue itself is unchanged (contrast with
*       pq_delete_top).!
*
*       If queue is empty, 0 is returned.
*
* Runtime:  O(1)
*
*/
int pq_peek_top(PQ * pq, int *id, double *priority){
	if (pq->count < 1){
		return 0;
	}
	*id = pq->heap[1].id;
	*priority = pq->heap[1].priority;
	return 1;
}

/**
* Function:  pq_capacity
* Parameters: priority queue pq
* Returns: capacity of priority queue (as set on creation)
* Desc: see returns
*
* Runtime:   O(1)
*
*/
int pq_capacity(PQ * pq){
	return pq->capacity;
}

/**
* Function: pq_size
* Parameters: priority queue pq
* Returns: number of elements currently in queue
* Desc: see above
*
* Runtime:  O(1)
*/
int pq_size(PQ * pq){
	return pq->count;
}