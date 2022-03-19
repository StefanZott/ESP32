#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<18) | (1ULL<<19) | (1ULL<<13) | (1ULL<<21) | (1ULL<<4) | (1ULL<<15) | (1ULL<<12) | (1ULL<<14) | (1ULL<<27) | (1ULL<<26))
#define HIGH 1
#define LOW 0

static void configGPIO();
static void barGraphDisplay(int ledOn);

static int ledPins[] = {19,18,13,21,4,15,12,14,27,26}; // an array of pin numbers to which LEDs are attached
int lengthFromArray = 0;

void app_main(void)
{

	configGPIO();

	lengthFromArray = sizeof(ledPins) / sizeof(ledPins[0]);

	while(1) {
		// makes the "i"th LED of LED bar graph bright in turn
		for (int i = 0; i < lengthFromArray; i++) {
			barGraphDisplay(i);
		}
		// makes the "i"th LED of LED bar graph bright in reverse order
		for (int i = lengthFromArray; i > 0; i--) {
			barGraphDisplay(i - 1);
		}
	}
}

static void configGPIO(){
	gpio_config_t io_conf;
	//disable interrupt
	io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//disable pull-down mode
	io_conf.pull_down_en = 0;
	//disable pull-up mode
	io_conf.pull_up_en = 0;
	//configure GPIO with the given settings
	gpio_config(&io_conf);
}

static void barGraphDisplay(int ledOn) {
	// make the "ledOn"th LED of bar graph LED on and the others off
	for (int j = 0; j < lengthFromArray; j++) {
		printf("LED: %d is", ledPins[j]);
		if (j == ledOn){
			printf(" %d.\n", LOW);
			gpio_set_level(ledPins[j], LOW);
		} else{
			printf(" %d.\n", HIGH);
			gpio_set_level(ledPins[j], HIGH);
		}
	}
	vTaskDelay(250 / portTICK_PERIOD_MS);
}
