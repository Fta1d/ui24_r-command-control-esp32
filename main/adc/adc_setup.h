#ifndef ADC_SETUP_H
#define ADC_SETUP_H

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_event.h"
#include "esp_log.h"

#define ADC_CHANNEL ADC_CHANNEL_6
#define UNIT ADC_UNIT_1
#define GLOBAL_ATTEN ADC_ATTEN_DB_12
#define NUM_SAMPLES 10

#define NEG_TO_POS(x) if (x < 0) x = -x

void adc_setup(void *arg);


#endif //ADC_SETUP_H
