#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define ledPin 13
#define buttonPin 12
#define HIGH 1
#define LOW 0

void app_main(void) {
	gpio_pad_select_gpio(12);
	gpio_set_direction(12, GPIO_MODE_INPUT);

	gpio_pad_select_gpio(13);
	gpio_set_direction(13, GPIO_MODE_OUTPUT);

	while(1){
		if(gpio_get_level(buttonPin) == HIGH) {
			gpio_set_level(ledPin, LOW);
		} else {
			gpio_set_level(ledPin, HIGH);
		}

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
}
