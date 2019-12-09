#ifndef _SORT_H_
#define _SORT_H_

// on stop -- stops all sorting
void stopSorting(void);

// should have retrieved array data - not implemented
int* Sort_getArrayData(void);

// gets array val at index given
int Sort_getArrayAtIndex(int ind);

// sorting function -- on loop
void* bubbleSort(void* arg);

// reteives numbers of already sorted arrays
long long Sort_getNumberArraysSorted(void);

// gets current array length 
int Sort_getNumberArrayLength(void);
#endif
