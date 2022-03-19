#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "soc/uart_struct.h"


#define BUF_SIZE        (1024)
#define BAUD_RATE       (115200)

static void configUART();

static const char *TAG = "ESP32";
const int uart_num = UART_NUM_0;
static int counter = 0;
static int let = 0;
static char buffer[500];

void app_main(void) {
	configUART();

	while(1) {
		printf("counter: %d\n", counter);
		counter++;

		let = uart_read_bytes(uart_num, buffer, sizeof(buffer), 10/portTICK_RATE_MS);
		if(let > 0){
			ESP_LOGI(TAG, "RECEIVE: %s\n", buffer);

		}

		vTaskDelay(500 / portTICK_PERIOD_MS);
	}

}

static void configUART() {
	uart_config_t uart_config = {
		.baud_rate = BAUD_RATE,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.rx_flow_ctrl_thresh = 122,
	};

	ESP_LOGI(TAG, "Start configure UART.");
	// Configure UART parameters
	uart_param_config(uart_num, &uart_config);

	ESP_LOGI(TAG, "UART install driver.");
	// Install UART driver (we don't need an event queue here)
	// In this example we don't even use a buffer for sending data.
	uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);
}
