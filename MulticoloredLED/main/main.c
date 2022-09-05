/* LEDC (LED Controller) basic example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

	RGB LED:

        	1
        	|
   	   _____|_____
  	  |     |     |
	 _|_R  _|_G  _|_B
 	 \_/   \_/   \_/
  	  |     |     |
  	  2     3     4
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include <time.h>

#define LEDC_TEST_CH_NUM 	3
#define LEDC_RED_CHANNEL 	LEDC_CHANNEL_0
#define LEDC_RED_GPIO 		19
#define LEDC_GREEN_CHANNEL 	LEDC_CHANNEL_1
#define LEDC_GREEN_GPIO		18
#define LEDC_BLUE_CHANNEL 	LEDC_CHANNEL_2
#define LEDC_BLUE_GPIO		5

#define LEDC_TEST_DUTY         (4000)
#define LEDC_TEST_FADE_TIME    (3000)

int nRandonNumber = 0;

void app_main(void)
{
	srand(time(NULL));

	// Prepare and then apply the LEDC PWM timer configuration
	ledc_timer_config_t ledc_timer = {
		.speed_mode       = LEDC_HIGH_SPEED_MODE,
		.timer_num        = LEDC_TIMER_0,
		.duty_resolution  = LEDC_TIMER_13_BIT, // Set duty resolution to 13 bits
		.freq_hz          = 5000,  // Frequency in Hertz. Set frequency at 5 kHz
		.clk_cfg          = LEDC_AUTO_CLK
	};
	ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

	ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
			{
				.channel 	= 	LEDC_RED_CHANNEL,
				.duty		=	0,
				.gpio_num	=	LEDC_RED_GPIO,
				.speed_mode	=	LEDC_HIGH_SPEED_MODE,
				.hpoint		=	0,
				.timer_sel	=	LEDC_TIMER_0
			},
			{
				.channel 	= 	LEDC_GREEN_CHANNEL,
				.duty		=	0,
				.gpio_num	=	LEDC_GREEN_GPIO,
				.speed_mode	=	LEDC_HIGH_SPEED_MODE,
				.hpoint		=	0,
				.timer_sel	=	LEDC_TIMER_0
			},
			{
				.channel 	= 	LEDC_BLUE_CHANNEL,
				.duty		=	0,
				.gpio_num	=	LEDC_BLUE_GPIO,
				.speed_mode	=	LEDC_HIGH_SPEED_MODE,
				.hpoint		=	0,
				.timer_sel	=	LEDC_TIMER_0
			},
	};

	// Set LED Controller with previously prepared configuration
	for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
		ledc_channel_config(&ledc_channel[ch]);
	}

	// Initialize fade service.
	ledc_fade_func_install(0);

	while (1) {

		for (int ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
			nRandonNumber = rand()%(4000 - 0) + 0;
			printf("CHANNEL: %d, DUTY: %d \n", ch, nRandonNumber);

			ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, nRandonNumber);
			ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
		}
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

