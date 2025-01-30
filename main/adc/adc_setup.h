//
// Created by mark on 1/30/25.
//

#ifndef ADC_SETUP_H
#define ADC_SETUP_H

#pragma once

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_event.h"
#include "esp_log.h"

#define ADC_CHANNEL ADC_CHANNEL_3
#define GLOBAL_ATTEN ADC_ATTEN_DB_12

#define NEG_TO_POS(x) if (x < 0) x = -x

static QueueHandle_t s_evt_queue = NULL;
static adc_oneshot_unit_handle_t s_adc2_handle;
static adc_cali_handle_t s_cali_handle;

void adc_setup();


#endif //ADC_SETUP_H
