#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()
#include <pthread.h>
#include <ctype.h>
#include "headers/sorter.h"
#include "headers/sort.h"
#include "headers/helper.h"

#define MSG_MAX_LEN 1024
#define PORT 12345
static char messageTx[MSG_MAX_LEN];
static int* soc_ptr;
static int UDPFlag = 1;

// processes the incoming command and send reply
void process_received_message(char* r_msg){

  if (strncmp(r_msg, "help" , 4)==0){
    //return summary of commands
    sprintf(messageTx, "%s\n",
    "\n\
    Accepted Commands: \n\
    count        -- display number of array sorted\n\
    get length   -- display length of array currently being sorter\n\
    get array    -- display the full array being Sorted\n\
    get #        -- display the # element of array currently being sorted\n\
    stop         -- stop prgram\n\
    ");
  }
  else if(strncmp(r_msg, "count" , 5)==0) {
    sprintf(messageTx, "Count of Arrays Sorted: %llu\n", Sort_getNumberArraysSorted());
  }
  else if(strncmp(r_msg, "get length" , 10)==0) {
    sprintf(messageTx, "Array Length: %d\n", Sort_getNumberArrayLength());
  }
  else if(strncmp(r_msg, "get array" , 9)==0) {
    //char* test = Sort_returnArray();
    sprintf(messageTx, "%s\n", ".");
  }
  else if(strncmp(r_msg, "get" , 3)==0 && Helper_check_digit(r_msg)) {
    int index = Helper_return_digit_from_string(r_msg);
    int len = Sort_getNumberArrayLength();
    if(index < len && index>0){
      int val = Sort_getArrayAtIndex(index-1);
      sprintf(messageTx,"Array at index: %d is %d\n", index, val);
    } else {
      sprintf(messageTx,
      "Provided integer is out of bounds. Please choose integers between 1 and %d", len);
    }
  }
  else if(strncmp(r_msg, "stop" , 4)==0) {
    sprintf(messageTx, "%s\n", "Closing Program, Press Enter to exit!");
    Sorter_stopSorting();
  }
  else {
    sprintf(messageTx, "%s\n", "invalid command");
  }

  return;
}



// UDP main function for socket
void* UDP_Listen(void* arg)
{
	printf("Connect using: \n");
	printf("    netcat -u 127.0.0.1 %d\n", PORT);

  //init
  // what port I want to open
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short

	// Create the socket for UDP && Bind
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
  soc_ptr = &socketDescriptor;

	while (UDPFlag==1) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		struct sockaddr_in sinRemote;
		unsigned int sin_len = sizeof(sinRemote);
		char messageRx[MSG_MAX_LEN];

    //received message
		int bytesRx = recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

		// Make it null terminated (so string functions work):
		int terminateIdx = (bytesRx < MSG_MAX_LEN) ? bytesRx : MSG_MAX_LEN - 1;
		                     messageRx[terminateIdx] = 0;
		                   printf("Command : \n\n'%s'\n",  messageRx);


    //process received command
    process_received_message(messageRx);


    // Transmit a reply:
		sin_len = sizeof(sinRemote);
		sendto( socketDescriptor,
			messageTx, strlen(messageTx),
			0,(struct sockaddr *) &sinRemote, sin_len);


	}
  pthread_exit(0);
  close(socketDescriptor);
  return NULL;
}



// on stop - ends socket communication
void stopSocket(void){
  UDPFlag = 0;
  return;
}
