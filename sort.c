#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "headers/helper.h"
#include "headers/beagle.h"

/* File helps build sorter function and returns array values as needed*/

// Variable & DEFs
static long long ARRAY_COUNT=0;
static int ARRAY_LENGTH=1;
//helper functions for bubblesort
static int* curr_array;
// controling the while loop
static int sorterFlag = 1;



//create array of integers dynamically for sorting; initial len == 100
//Initialize the array to be a random permutation of the numbers 1
void* bubbleSort(void* arg){
  while(sorterFlag ==1) {
    curr_array = (int*) malloc(sizeof(int) * ARRAY_LENGTH);

    //initialize arrays
    for(int i=0; i<ARRAY_LENGTH; i++) {
      curr_array[i] = 1;
    }
    //input random values for permutation
    for(int i=0; i<ARRAY_LENGTH; i++) {
      curr_array[i] = rand()%ARRAY_LENGTH + 1;
    }

    //bubble sort
     for (int i=0; i< ARRAY_LENGTH; i++) {
       for (int j = 0; j < ARRAY_LENGTH-i-1; j++) {
         //printf("with arr val: %d\n",arr_struct->arr[j+1] );
         if (curr_array[j] > curr_array[j+1])
            Helper_swap(&curr_array[j], &curr_array[j+1]);
       }
     }
     ARRAY_COUNT+=1;

     //printArray(curr_array, ARRAY_LENGTH);
     free(curr_array);

     //set next size !
     int new_size = Beagle_getArraySize();
     ARRAY_LENGTH = new_size;
  }
     pthread_exit(0);
     return NULL;

}
//-----------------------------------------------------------------------
//-------------------Returning Module Values---------------------------
// Get the size of the array currently being sorted.
// Set the size the next array to sort (don’t change current array)



// Get the number of arrays which have finished being sorted.
long long Sort_getNumberArraysSorted(void){
  return ARRAY_COUNT;
}

//return array length
int Sort_getNumberArrayLength(void){
  return ARRAY_LENGTH;
}


//return array at specified index
int Sort_getArrayAtIndex(int ind){
  return curr_array[ind];
}


// Get a copy of the current (potentially partially sorted) array.
// Returns a newly allocated array and sets 'length' to be the
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
int* Sort_getArrayData(void){
  int *copy_array = malloc(sizeof(*curr_array) * ARRAY_LENGTH);
  memcpy(copy_array, curr_array, sizeof(*curr_array) * ARRAY_LENGTH);
  return copy_array;
}


//helps with shutting down the sorting
void stopSorting(void){
  sorterFlag = 0;
}
