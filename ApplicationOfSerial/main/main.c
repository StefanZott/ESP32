#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUF_SIZE        (1024)
#define BAUD_RATE       (115200)

#define LED_RED (18)
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<LED_RED))

static void configUART();
static void gpioConfig();

static const char *TAG = "ESP32";
const int uart_num = UART_NUM_0;
int len = 0;

void app_main(void) {
	uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

	configUART();

	gpioConfig();

	while(1) {
		len = uart_read_bytes(uart_num, data, (BUF_SIZE - 1), 20 / portTICK_PERIOD_MS);

		if(len > 0) {
			int inInt = atoi((const char*)data);

			ESP_LOGI(TAG, "inInt: %d \n", inInt);

			if(inInt > 0 && inInt < 255) {
				ESP_LOGI(TAG, "GPIO: 1 \n");
				gpio_set_level(LED_RED, 1);
			} else {
				ESP_LOGI(TAG, "GPIO: 0 \n");
				gpio_set_level(LED_RED, 0);
			}
		}
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

	printf("Start configure UART. \n");

	// Configure UART parameters#
	if(uart_param_config(uart_num, &uart_config) != ESP_OK) {
		ESP_LOGI(TAG,"Configuration UART_0 failed!");
	} else {
		ESP_LOGI(TAG,"Configuration UART_0 successful!");
	}


	ESP_LOGI(TAG, "UART install driver.");
	// Install UART driver (we don't need an event queue here)
	// In this example we don't even use a buffer for sending data.
	if(uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0) != ESP_OK) {
		ESP_LOGI(TAG,"Driver installion UART_0 failed!");
	} else {
		ESP_LOGI(TAG,"Driver installaion UART_0 successful!");
	}
}

static void gpioConfig() {
	gpio_config_t io_conf = {};
	//disable interrupt
	io_conf.intr_type = GPIO_INTR_DISABLE;
	//set as output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
	//bit mask of the pins that you want to set,e.g.GPIO18/19
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	//configure GPIO with the given settings
	gpio_config(&io_conf);

//    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
//    //set as input mode
//    io_conf.mode = GPIO_MODE_OUTPUT;
//    gpio_config(&io_conf);

	//create a queue to handle gpio event from isr
//    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
	//start gpio task
	// xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);


}
