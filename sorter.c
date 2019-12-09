#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "headers/sort.h"
#include "headers/udp.h"
#include "headers/beagle.h"


/// Begin/end the background thread which sorts random permutations.
void Sorter_startSorting(void) {

  pthread_t tid[2];
  // Create attributes
  //controls how thread functions
	pthread_attr_t attr;
	pthread_attr_init(&attr);

  pthread_attr_t attr2;
	pthread_attr_init(&attr2);

  pthread_create(&tid[0], &attr2, UDP_Listen, NULL);
  pthread_create(&tid[1], &attr, bubbleSort, NULL);
  //Wait for thread sort to finish work


  for (int i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}
  return;
}



// STOP COMANND FUNCTION
void Sorter_stopSorting(void) {
  stopSorting();
  stopSocket();
  I2C_end();

}
