/**
	Assignment 1 - Sunaina Paudel - 301247952
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
	Define all LED files and Joystick GPIO
*/
#define BGB_LED0 "/sys/class/leds/beaglebone:green:usr0/brightness"
#define BGB_LED1 "/sys/class/leds/beaglebone:green:usr1/brightness"
#define BGB_LED2 "/sys/class/leds/beaglebone:green:usr2/brightness"
#define BGB_LED3 "/sys/class/leds/beaglebone:green:usr3/brightness"
#define BGB_LED "/sys/class/leds/beaglebone:green:usr"

#define BGB_GPIO_UP "/sys/class/gpio/gpio26/value"
#define BGB_GPIO_LEFT "/sys/class/gpio/gpio65/value"
#define BGB_GPIO_DOWN "/sys/class/gpio/gpio46/value"
#define BGB_GPIO_RIGHT "/sys/class/gpio/gpio47/value"



/**
	define the Joystick Positions
	Create getJoyStick for testing purposes -- char print statements
*/
enum Joystick {none, up, left, down, right};
const char* getJoyStick( enum Joystick side) {
	switch (side){
	case none: return "None"; break;
	case up: return "Up"; break;
	case left: return "left";break;
	case down: return "Down";break;
	case right: return "Right";break;
	default:
		printf("Invalid.");
		return "";
	}
}


/**
	Helper Function for main
	sleep function for needed delays/ wait times
*/
void sleep(long seconds,long nanoseconds ) {
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
	return;
}

/**
	Function will change the brightness of the specified LED light
*/
void ChangeLedLight(char* led,char *bright){
	//"0/trigger"
	FILE *pLedbrightFile = fopen(led , "w");
	if (pLedbrightFile == NULL) {
		printf("ERROR OPENING %s.\n", led);
		exit(1);
	}
	int charWrittenLed = fprintf(pLedbrightFile, "%s", bright);
	if(charWrittenLed <=0) {
		printf("ERROR WRITING DATA.\n");
		exit(1);
	}
	fclose(pLedbrightFile);
	return;
}

/**
	Function will randomly select "up" or "down" targets for users.
*/
int UpOrDown(){
	srand ( time(NULL) );
	int temp = rand()%50;
	 if (temp%2 ==0){
		 ChangeLedLight(BGB_LED0, "1");
		 return 1;
	 }
	 else {
		 ChangeLedLight(BGB_LED3, "1");
		 return 0;
	 }
}


/**
	Helper Function for UserChoice()
	Function will check the values at specific gpio pins
	and return boolean (int) specifying if it contains 0 or 1 in file
*/
int checkGPIO(char* gpio){
	int flag = 0;
	FILE *file = fopen( gpio , "r");
	if (file == NULL){
		printf("ERROR OPENING %s.\n", gpio);
		exit(-1);
	}
	const int max_length = 1024;
	char buff[max_length];
	fgets(buff, max_length, file);
	fclose(file);
	//printf("Read: %s\n", buff);

	if (strstr(buff, "0")!=NULL) {
	 flag=1;
	}
	return flag;
}


/**
	Function runs through the User Joystick movement;
	Keeps checking until user makes a selection.
*/
enum Joystick UserChoice(enum Joystick move){
	//enum Joystick current;
	while(move==none){
		if (checkGPIO(BGB_GPIO_UP )==1) {
			move = up;
		}
		if (checkGPIO(BGB_GPIO_LEFT )==1) {
			move =left;
		}
		if (checkGPIO(BGB_GPIO_DOWN )==1) {
			move = down;
		}
		if (checkGPIO(BGB_GPIO_RIGHT )==1) {
			move = right;
		}
	}
	//printf("%s.\n",getJoyStick(move));
	return move;
}


/**
	Funtion flases LED once.
*/
void FlashLED(){
	ChangeLedLight(BGB_LED0, "1");
	ChangeLedLight(BGB_LED1, "1");
	ChangeLedLight(BGB_LED2, "1");
	ChangeLedLight(BGB_LED3, "1");
	sleep(0,  100000000);
	ChangeLedLight(BGB_LED0, "0");
	ChangeLedLight(BGB_LED1, "0");
	ChangeLedLight(BGB_LED2, "0");
	ChangeLedLight(BGB_LED3, "0");
	return;
}


/**
	Function flases LED multiple times
*/
void FlashLED_Incorrect(){
	FlashLED();
	//get trigger file and change to 'timer'
	FILE *pLedtriggerFile = fopen(BGB_LED "0/trigger" , "w");
	if (pLedtriggerFile == NULL) {
		printf("ERROR OPENING %s.\n", BGB_LED "0/trigger");
		exit(1);
	}

	int charWrittenLed = fprintf(pLedtriggerFile, "%s", "timer");
	if(charWrittenLed <=0) {
		printf("ERROR WRITING DATA.\n");
		exit(1);
	}
	fclose(pLedtriggerFile);
	//get trigger file and change to 'timer'
	FILE *pLedtriggerFile1 = fopen(BGB_LED "1/trigger" , "w");
	if (pLedtriggerFile1 == NULL) {
		printf("ERROR OPENING %s.\n", BGB_LED "1/trigger");
		exit(1);
	}

	int charWrittenLed1 = fprintf(pLedtriggerFile1, "%s", "timer");
	if(charWrittenLed1 <=0) {
		printf("ERROR WRITING DATA.\n");
		exit(1);
	}
	fclose(pLedtriggerFile1);
	//get trigger file and change to 'timer'
	FILE *pLedtriggerFile2 = fopen(BGB_LED "2/trigger" , "w");
	if (pLedtriggerFile2 == NULL) {
		printf("ERROR OPENING %s.\n", BGB_LED "2/trigger");
		exit(1);
	}

	int charWrittenLed2 = fprintf(pLedtriggerFile2, "%s", "timer");
	if(charWrittenLed2 <=0) {
		printf("ERROR WRITING DATA.\n");
		exit(1);
	}
	fclose(pLedtriggerFile2);
	//get trigger file and change to 'timer'
	FILE *pLedtriggerFile3 = fopen(BGB_LED "3/trigger" , "w");
	if (pLedtriggerFile3 == NULL) {
		printf("ERROR OPENING %s.\n", BGB_LED "3/trigger");
		exit(1);
	}

	int charWrittenLed3 = fprintf(pLedtriggerFile3, "%s", "timer");
	if(charWrittenLed3 <=0) {
		printf("ERROR WRITING DATA.\n");
		exit(1);
	}
	fclose(pLedtriggerFile3);

	sleep(4, 500000000);
	ChangeLedLight(BGB_LED0, "0");
	ChangeLedLight(BGB_LED1, "0");
	ChangeLedLight(BGB_LED2, "0");
	ChangeLedLight(BGB_LED3, "0");

}

/**
	Main Function
*/
int main(int argc, char **args)
{

	// intializations -- turn off all Leds
	ChangeLedLight(BGB_LED0, "0");
	ChangeLedLight(BGB_LED1, "0");
	ChangeLedLight(BGB_LED2, "0");
	ChangeLedLight(BGB_LED3, "0");
	printf("Hello embedded world, from Sunaina Paudel!\n");
	int NumberOfTimesPressed = 0;
	int CorrectScore = 0;
	int current_target;
	enum Joystick choice;
	choice = none;

	/**
		Beginning of Game.
	*/
	while ( (choice==up) || (choice==down) || (choice==none)){
		choice = none;
		//add the score there
		current_target = UpOrDown(); // 1 up or 0 down
		printf("Press joystick! Current Score (%d / %d)\n", CorrectScore, NumberOfTimesPressed);
		choice = UserChoice(choice);
		if (choice==up && current_target==1){
			printf("correct!\n");
			CorrectScore+= 1;
			FlashLED();
		}
		else if (choice==down && current_target==0){
			printf("correct!\n");
			CorrectScore+=1;
			FlashLED();
		}
		else if (choice==down && current_target==1){
			printf("Incorect!\n");
			FlashLED_Incorrect();
		}
		else if (choice==up && current_target==0){
			printf("Incorect!\n");
			FlashLED_Incorrect();
				}
		NumberOfTimesPressed += 1;

		//give user time
		sleep(1,  500000000);

	}

	/**
		End of Game. Choice is left || right.
	*/
	ChangeLedLight(BGB_LED0, "0");
	ChangeLedLight(BGB_LED3, "0");
	printf("Your final score was: (%d / %d)\n", CorrectScore, NumberOfTimesPressed-1);
	printf("Thanks for playing, goodbye!\n");


	return 0;


}

