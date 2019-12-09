#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "headers/sort.h"

/* file gives acces to beagle bone A2D & I2C */


#define A2D_FILE_VOLTAGE0 "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20
#define REG_DIRA 0x00
#define REG_DIRB 0x01
#define REG_OUTA 0x14
#define REG_OUTB 0x15

// gets amd volt read
static int getVoltage0Reading()
{
  // Open file
  FILE *f = fopen(A2D_FILE_VOLTAGE0, "r");
  if (!f) {
  printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
  printf(" Check /boot/uEnv.txt for correct options.\n");
  exit(-1);
  }
  // Get reading
  int a2dReading = 0;
  int itemsRead = fscanf(f, "%d", &a2dReading);
  if (itemsRead <= 0) {
  printf("ERROR: Unable to read values from voltage input file.\n");
  exit(-1);
  }
  // Close file
  fclose(f);
  return a2dReading;
}

// pwd fnction for voltage
int pwd(int x1, int x2, int y1, int y2, int current){
  double slope = (double) (y2-y1)/(x2-x1);
  //printf("double %f", slope);
  int y_ans = slope*current;
  //printf("%d", y_ans);
  return y_ans;
}


// set config-pins for use to i2c
void config_pin(void){
  char *input1 = "config-pin P9_18 i2c";
  FILE *fp = NULL;
  char *input2 = "config-pin P9_17 i2c";
  FILE *fp2 = NULL;


  fp = popen(input1, "w");

  if (fp == NULL)
  {
      //printf("\nFailed command\n");
      return;
  }
  else
  {
      //printf("\nConfig-pin P9_18 i2c set\n");
  }
  fp2 = popen(input2, "w");

  if (fp2 == NULL)
  {
      //printf("\nFailed command\n");
      return;
  }
  else
  {
      //printf("\nConfig-pin P9_17 i2c set\n");
      return;
  }

  return;
}


// writing a register
static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,unsigned char value)
  {
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2) {
    perror("I2C: Unable to write i2c register.");
    exit(1);
  }
}


// init for I2C
static int initI2cBus(char* bus, int address)
{

  //
  int i2cFileDesc = open(bus, O_RDWR);
  int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
  if (result < 0) {
    perror("I2C: Unable to set I2C device to slave address.");
    exit(1);
  }
  return i2cFileDesc;
}

void I2C_end(void){
  int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);


  // write values into register
  // direction setup
  writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
  writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);



  // Drive an hour-glass looking character
  // (Like an X with a bar on top & bottom)
  writeI2cReg(i2cFileDesc, REG_OUTA, 0x00);
  writeI2cReg(i2cFileDesc, REG_OUTB, 0x00);

  close(i2cFileDesc);
}
// I2C main function
void I2C(int curr_sz){
  // i2c
  // config pin first
  config_pin();
  int i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);


  // write values into register
  // direction setup
  writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
  writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);



  // Drive an hour-glass looking character
  // (Like an X with a bar on top & bottom)
  writeI2cReg(i2cFileDesc, REG_OUTA, 0x2A);
  writeI2cReg(i2cFileDesc, REG_OUTB, 0x54);

  close(i2cFileDesc);


}



// i2c also called within
// setting size and I2C
int Beagle_getArraySize(void){
  while (1) {
    int set_size =0;
    int copy_arr_size = Sort_getNumberArraysSorted();

    //set I2C vals
    I2C(copy_arr_size);

    sleep(1);
    int volt = getVoltage0Reading();
    double voltage = ((double) volt / A2D_MAX_READING) * A2D_VOLTAGE_REF_V;
    printf("Value %5d ==> %5.2fV\n", volt, voltage);

    if (0 <= volt && volt < 500) { set_size = pwd(0, 500, 1, 20, volt);}
    else if( 500 <= volt && volt < 1000) { set_size = pwd(500, 1000, 20, 60, volt) ;}
    else if( 1000 <= volt && volt< 1500) { set_size =pwd(1000, 1500, 60, 120, volt);}
    else if( 1500 <= volt && volt< 2000) { set_size = pwd(1500, 2000, 120, 250, volt);}
    else if( 2000 <= volt && volt< 2500) { set_size =pwd(2000, 2500, 250, 300, volt);}
    else if( 2500 <= volt && volt < 3000) { set_size =pwd(2500, 3000, 300, 500, volt);}
    else if( 3000 <= volt && volt< 3500) { set_size =pwd(3000, 3500, 500, 800, volt);}
    else if( 3500 <= volt && volt< 4000) { set_size =pwd(3500, 4000, 800, 1200, volt);}
    else if( 4000 <= volt && volt < 4100) { set_size =pwd(4000, 4100, 1200, 2100, volt);}
    else { set_size=1;}

    printf("Size of Array: %d\n", set_size);
    return set_size;


  }

}
