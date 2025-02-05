#include "main.h"

EventGroupHandle_t s_socket_event;

void app_main(void) {
    s_socket_event = xEventGroupCreate();
    xTaskCreatePinnedToCore(ap_connection, "ap-connection", 4096, NULL, 2, NULL, 0);

    ESP_LOGI("main", "Waiting for server to connect...");
    xEventGroupWaitBits(s_socket_event, SOCKET_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    ESP_LOGI("main", "Server connected! Starting ADC...");
    xTaskCreatePinnedToCore(adc_setup, "adc-setup", 2048, NULL, configMAX_PRIORITIES - 1, NULL, 1);
}
