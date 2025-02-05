//
// Created by mark on 1/30/25.
//

#include "adc_setup.h"

#include "../main.h"

#define VOL_TO_DAT(x) ((float)(x) / 3023.0)

static adc_oneshot_unit_handle_t s_adc_handle;
static adc_cali_handle_t s_cali_handle;

static void s_read_adc_average(int *value, esp_err_t *ret) {
    int sum = 0;
    for (int i = 0; i < NUM_SAMPLES; i++) {

        *ret = adc_oneshot_get_calibrated_result(s_adc_handle, s_cali_handle, ADC_CHANNEL, value);
        if (*ret == ESP_OK) {
            sum += *value;
        } else {
            ESP_LOGE("adc", "Failed to read ADC data: %s", esp_err_to_name(*ret));
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    *value = sum / NUM_SAMPLES;
}

static void s_adc_configure (void) {
    const adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &s_adc_handle));

    const adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = GLOBAL_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc_handle, ADC_CHANNEL, &config));

    ESP_LOGI("adc", "Configured ADC on channel %d", ADC_CHANNEL);
}

static void s_adc_calibration_configure (void) {
    const adc_cali_line_fitting_config_t cali_conf = {
        .unit_id = UNIT,
        .atten = GLOBAL_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_conf, &s_cali_handle));

    ESP_LOGI("adc", "Calibrated ADC on channel %d", ADC_CHANNEL);
}

static void s_potentiometer_task(void) {
    int voltage;
    int prev_voltage = 0, offset = 142, treshold = 30;

    while (1) {
        // esp_err_t ret = adc_oneshot_get_calibrated_result(s_adc_handle, s_cali_handle, ADC_CHANNEL, &voltage);
        // esp_err_t ret = adc_oneshot_read(s_adc2_handle, ADC_CHANNEL, &voltage);
        esp_err_t ret;
        s_read_adc_average(&voltage, &ret);
        voltage -= offset;
        int current_voltage = voltage;

        if (ret == ESP_OK) {
            int diff = current_voltage - prev_voltage;
            NEG_TO_POS(diff);

            if (diff > treshold)
                ESP_LOGI("adc", "ADC%d Channel[%d] Raw Data: %.4f", UNIT + 1, ADC_CHANNEL, VOL_TO_DAT(voltage));
        }

        prev_voltage = current_voltage;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void adc_setup(void *arg) {
    s_adc_configure();
    s_adc_calibration_configure();

    s_potentiometer_task();
}