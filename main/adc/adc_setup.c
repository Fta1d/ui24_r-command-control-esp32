//
// Created by mark on 1/30/25.
//

#include "adc_setup.h"

static void s_adc_configure (void) {
    const adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_2,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &s_adc2_handle));

    const adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = GLOBAL_ATTEN,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(s_adc2_handle, ADC_CHANNEL, &config));
}

static void s_adc_calibration_configure (void) {
    const adc_cali_line_fitting_config_t cali_conf = {
        .unit_id = ADC_UNIT_2,
        .atten = GLOBAL_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,
    };

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_conf, &s_cali_handle));
}

static void s_potentiometer_task(void* arg) {
    int voltage, current_voltage;
    int prev_voltage = 0, offset = 128, treshold = 30;

    while (1) {
        esp_err_t ret = adc_oneshot_get_calibrated_result(s_adc2_handle, s_cali_handle, ADC_CHANNEL, &voltage);
        current_voltage = voltage - offset;

        if (ret == ESP_OK) {
            int diff = current_voltage - prev_voltage;
            NEG_TO_POS(diff);

            if (diff > treshold)
                ESP_LOGI("adc", "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_2 + 1, ADC_CHANNEL, voltage - offset);
        } else {
            ESP_LOGE("adc", "Failed to read ADC data: %s", esp_err_to_name(ret));
        }

        prev_voltage = current_voltage;

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void adc_setup() {
    s_adc_configure();
    s_adc_calibration_configure();

    s_evt_queue = xQueueCreate(10, sizeof(int));

    xTaskCreate(s_potentiometer_task, "potentiometer_task", 2048, NULL, 2, NULL);
}