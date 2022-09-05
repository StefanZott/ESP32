/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "driver/ledc.h"
#include "driver/dac_common.h"

#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;
static const adc_channel_t channel = ADC_CHANNEL_6;     //GPIO34 if ADC1,
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;
static const adc_atten_t atten = ADC_ATTEN_DB_0;
static int dac_value = 0;



void app_main(void)
{
    dac_output_enable(DAC_CHANNEL_1);

    //Configure ADC
	adc1_config_width(width);
	adc1_config_channel_atten(channel, atten);

    //Characterize ADC
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));

    //Continuously sample ADC1
    while (1) {
        uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
			adc_reading += adc1_get_raw((adc1_channel_t)channel);
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

        printf("Analog Value = %d", adc_reading);

        if(adc_reading < 40) {
        	printf(" => Dark\n");
        } else if (adc_reading < 800) {
        	printf(" => Dim\n");
		} else if (adc_reading < 2000) {
			printf(" => Light\n");
		} else if (adc_reading < 3200) {
			printf(" => Bright\n");
		} else {
			printf(" => very Bright\n");
		}

        dac_value = (adc_reading * 255) / 4095;

        dac_output_voltage(DAC_CHANNEL_1, dac_value);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
