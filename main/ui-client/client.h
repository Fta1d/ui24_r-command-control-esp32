//
// Created by mark on 2/3/25.
//

#ifndef CLIENT_H
#define CLIENT_H

#pragma once

#include "esp_wifi.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"

#define WIFI_SSID "MargaritaMark"
#define WIFI_PASSWORD "pocketbook"
#define HOST_IP "192.168.0.119"
#define PORT 8080

#define WIFI_CONNECTED BIT0
#define WIFI_FAILURE BIT1
#define TCP_ERROR BIT2
#define TCP_SUCCESS BIT3
#define MAX_RETRIES 5


void ap_connection(void *arg);

#endif //CLIENT_H
