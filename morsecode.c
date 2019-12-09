#include <linux/module.h>
#include <linux/miscdevice.h>		// for misc-driver calls.
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/leds.h>
#include <linux/kfifo.h>
#include "constants.h"

//#error Are we building this?

static char data[DATA_SIZE];
#define FIFO_SIZE 256	// Must be a power of 2.
static DECLARE_KFIFO(echo_fifo, char, FIFO_SIZE);

static ssize_t test = 0;
/******************************************************
 * Parameter
 ******************************************************/
static int dottime = DEFAULT_DOTTIME;

module_param(dottime, int, S_IRUGO);
MODULE_PARM_DESC(dottime, " morse-code time !");

/******************************************************
 * LED
 ******************************************************/

DEFINE_LED_TRIGGER(morse_code);

static void my_led_blink(int on, int off)
{
	led_trigger_event(morse_code, LED_FULL);
	msleep(on);
	led_trigger_event(morse_code, LED_OFF);
	msleep(off);
}

static void led_register(void)
{
	// Setup the trigger's name:
	led_trigger_register_simple("morse-code", &morse_code);
}

static void led_unregister(void)
{
	// Cleanup
	led_trigger_unregister_simple(morse_code);
}



static int bool_dash =0;
static void morse_led_blink(unsigned short val){
	if (val!=0)
{
	//printk(KERN_INFO "THIS IS THE VALUE %d", val);
	int i=3;
	for ( i=3; i>=0; i--){
		unsigned short led = (val & (1 << i)) >> i;
		//printk(KERN_INFO "THIS IS THE test %d", led);
		if (led==1){
			bool_dash++;
		}else {
				if (bool_dash==3){
					// add dash
					my_led_blink(dottime*3, dottime);
					if (!kfifo_put(&echo_fifo, '-')) {
					// Fifo full
					test = -EFAULT;
					}
					bool_dash=0;
				}else {
					while (bool_dash!=0){
						my_led_blink(dottime, dottime);
						if (!kfifo_put(&echo_fifo, '.')) {
						// Fifo full
						test = -EFAULT;
						}
						bool_dash--;
					}
				}
				msleep(dottime);
		}

	}
}
}

// function to handle morse hex to binary and blink led
static void led_up(unsigned short val){

	// space between each letter
	unsigned short firstHEX = (val & 0xF000) >> 12;
	unsigned short secondHEX = (val & 0x0F00) >> 8;
	unsigned short thirdHEX = (val & 0x00F0) >> 4;
	unsigned short forthHEX = (val & 0x000F);

	morse_led_blink(firstHEX);
	morse_led_blink(secondHEX);
	morse_led_blink(thirdHEX);
	morse_led_blink(forthHEX);

	if (!kfifo_put(&echo_fifo, ' ')) {
	// Fifo full
	test = -EFAULT;
	}
	bool_dash=0;
}


/******************************************************
 * Callbacks
 ******************************************************/
static ssize_t my_read(struct file *file,
		char *buff, size_t count, loff_t *ppos)
{
	// Pull all available data from fifo into user buffer
		int num_bytes_read = 0;
		if (kfifo_to_user(&echo_fifo, buff, count, &num_bytes_read)) {
			return -EFAULT;
		}


		return num_bytes_read;  // # bytes actually read.
}



static ssize_t my_write(struct file *file,
		const char *buff, size_t count, loff_t *ppos)
{
	// take from user space
	int buff_idx = 0;

	// for each character
	for (buff_idx = 0; buff_idx < count; buff_idx++) {
		char ch;
		// Get the character
		if (copy_from_user(&ch, &buff[buff_idx], sizeof(ch))) {
			return -EFAULT;
		}

		// Skip special characters:
		if (ch < ' ') {
			continue;
		}

		if (ch == ' '){
			if (!kfifo_put(&echo_fifo, ' ')) {
			// Fifo full
			return -EFAULT;
			}
			if (!kfifo_put(&echo_fifo, ' ')) {
				// Fifo full
				return -EFAULT;
			}
		msleep(dottime*3);
		}


		// Process the character
		if(ch=='a' || ch=='A'){ led_up(morsecode_codes[0]);}
		else if(ch=='b' || ch=='B') {led_up(morsecode_codes[1]);}
		else if(ch=='c' || ch=='C') { led_up(morsecode_codes[2]);}
		else if(ch=='d' || ch=='D') { led_up(morsecode_codes[3]);}
		else if(ch=='e'  || ch=='E') {led_up(morsecode_codes[4]);}
		else if(ch=='f' || ch=='F') {led_up(morsecode_codes[5]);}
		else if(ch=='g' || ch=='G') { led_up(morsecode_codes[6]);}
		else if(ch=='h' || ch=='H') { led_up(morsecode_codes[7]);}
		else if(ch=='i' || ch=='I') { led_up(morsecode_codes[8]);}
		else if(ch=='j' || ch=='J') {led_up(morsecode_codes[9]);}
		else if(ch=='k' || ch=='K') { led_up(morsecode_codes[10]);}
		else if(ch=='l' || ch=='L') { led_up(morsecode_codes[11]);}
		else if(ch=='m' || ch=='M') { led_up(morsecode_codes[12]);}
		else if(ch=='n'  || ch=='N') {led_up(morsecode_codes[13]);}
		else if(ch=='o' || ch=='O') { led_up(morsecode_codes[14]);}
		else if(ch=='p' || ch=='P') { led_up(morsecode_codes[15]);}
		else if(ch=='q' || ch=='Q') { led_up(morsecode_codes[16]);}
		else if(ch=='r'  || ch=='R') {led_up(morsecode_codes[17]);}
		else if(ch=='s' || ch=='S') { led_up(morsecode_codes[18]);}
		else if(ch=='t' || ch=='T') { led_up(morsecode_codes[19]);}
		else if(ch=='u' || ch=='U') { led_up(morsecode_codes[20]);}
		else if(ch=='v' || ch=='V') { led_up(morsecode_codes[21]);}
		else if(ch=='w' || ch=='W') { led_up(morsecode_codes[22]);}
		else if(ch=='x' || ch=='X') { led_up(morsecode_codes[23]);}
		else if(ch=='y' || ch=='Y') { led_up(morsecode_codes[24]);}
		else if(ch=='z' || ch=='Z') { led_up(morsecode_codes[25]);}

	}
	// Return # bytes actually written.
	if (!kfifo_put(&echo_fifo, '\n')) {
	// Fifo full
	return -EFAULT;
	}
	return count;
}

/******************************************************
 * Misc support
 ******************************************************/
// Callbacks:  (structure defined in /linux/fs.h)
struct file_operations my_fops = {
	.owner    =  THIS_MODULE,
	.read     =  my_read,
	.write    =  my_write,
};

// Character Device info for the Kernel:
static struct miscdevice my_miscdevice = {
		.minor    = MISC_DYNAMIC_MINOR,         // Let the system assign one.
		.name     = MY_DEVICE_FILE,             // /dev/.... file.
		.fops     = &my_fops                    // Callback functions.
};


/******************************************************
 * Initialization of Data Function
 ******************************************************/
static void initialize_data(void)
{
	int i = 0;
	for (i = 0; i < DATA_SIZE; i++) {
		data[i] = i + START_CHAR;
	}
}

/******************************************************
 * Driver initialization and exit:
 ******************************************************/
static int __init morsecode_init(void)
{
  int ret;
  printk(KERN_INFO "----> morsecode driver init() file /dev/%s.\n", MY_DEVICE_FILE);

	// Initialize the FIFO.
	INIT_KFIFO(echo_fifo);

  // Register as a misc driver:
	ret = misc_register(&my_miscdevice);

  initialize_data();

  // LED:
	led_register();
  return 0;
}


static void __exit morsecode_exit(void)
{
  printk(KERN_INFO "<---- morsecode driver exit().\n");

  // Unregister misc driver
	misc_deregister(&my_miscdevice);
  // LED:
	led_unregister();

}


// Link our init/exit functions into the kernel's code.
module_init(morsecode_init);
module_exit(morsecode_exit);
// Information about this module:
MODULE_AUTHOR("Sunaina");
MODULE_DESCRIPTION("morsecode driver");
MODULE_LICENSE("GPL"); // Important to leave as GPL.
