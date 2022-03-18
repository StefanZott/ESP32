#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define HIGH 1
#define LOW 0

// an array of pin numbers to which LEDs are attached
int ledPins[] = { 19,18,13,21,4,15,12,14,27,26 };
static int lengthFromArray = 0;

static void barGraphDisplay(int ledOn);

void app_main(void)
{
	lengthFromArray = sizeof(ledPins) / sizeof(ledPins[0]);

	for (int i = 0; i < lengthFromArray; ++i) {
		 gpio_pad_select_gpio(ledPins[i]);
		 gpio_set_direction(ledPins[i], GPIO_MODE_OUTPUT);
	}

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

static void barGraphDisplay(int ledOn) {
	// make the "ledOn"th LED of LED bar graph on and the others off
	for (int i = 0; i < lengthFromArray; i++) {
		printf("GPIO: %d, ",ledPins[i]);
		if (i == ledOn){
			printf("HIGH\n");
			gpio_set_level(ledPins[i], HIGH);
		} else {
			printf("LOW\n");
			gpio_set_level(ledPins[i], LOW);
		}
	}
	vTaskDelay(100 / portTICK_PERIOD_MS);
}
