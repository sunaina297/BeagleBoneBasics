#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <unistd.h>


// for lock -- synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Helper Functions for smaller tasks


// FOR SORT.C
//function to swap values in array for sorting
void Helper_swap(int *xp, int *yp){
  // critical section = swapping values
  pthread_mutex_lock(&mutex);
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
  //end critical section
  pthread_mutex_unlock(&mutex);
  return;
}

/* Function to print an array */
void Helper_printArray(int arr[], int size){
    printf("Sorted array: \n");
    for (int i=0; i < size; i++) {
      printf("%d ", arr[i]);
    }
    printf("with size: %d\n", size);
}



//--------------------------------------------------------------------
// FOR UDP

// checks for digit in string
int Helper_check_digit(char* a){
  for(int i=0; i<10; i++){
    if(isdigit(a[i])){
      return 1;
    }
  }
  return 0;
}

//retruns the digit in string 
int Helper_return_digit_from_string(char* dg){
  char digitOnly[10];
  int j=0;
  for(int i=0; i<7; i++){
    if(isdigit(dg[i])){
      digitOnly[j]=dg[i];
      j+=1;
    }
  }
  int ret = atoi(digitOnly);
  return ret;
}
