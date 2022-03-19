#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

void app_main(void) {
	int counter = 0;

	while(1) {
		printf("counter: %d\n", counter);
		counter++;
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}
