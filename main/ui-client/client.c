#include "client.h"

#include "../main.h"

static EventGroupHandle_t s_wifi_event_group;
static int s_retry_num = 0;
static const char *payload = "ALIVE";

int ex_sock;

static void s_wifi_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI("wifi evt handler: ", "Connecting to AP...");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGE("wifi evt handler: ", "Failed to connect, retrying...");
        if (s_retry_num < MAX_RETRIES) {
            ESP_LOGI("wifi evt handler: ", "Reconnecting to AP...");
            esp_wifi_connect();
            s_retry_num++;
        } else {
            ESP_LOGW("wifi evt handler: ", "Max retries exceeded, returning.");
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAILURE);
        }
    }
}

static void s_ip_event_handler(void *arg, esp_event_base_t event_base,
                                int32_t event_id, void *event_data) {
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI("ip event handler: ", "Got IP:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED);
    }
}

esp_err_t wifi_setup() {
    esp_err_t status = WIFI_FAILURE;
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t init_wifi_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_wifi_cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &s_wifi_event_handler,
        NULL,
        &instance_any_id
        ));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &s_ip_event_handler,
        NULL,
        &instance_got_ip
        ));

    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = 1,
                .required = 0,
            },
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("wifi setup: ", "Wifi station configured!");

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED | WIFI_FAILURE,
          pdFALSE,
         pdFALSE,
          portMAX_DELAY);

    if (bits & WIFI_CONNECTED) {
        ESP_LOGI("wifi setup: ", "Connected to AP");
        status = WIFI_CONNECTED;
    } else if (bits & WIFI_FAILURE) {
        ESP_LOGI("wifi setup: ", "Failed to connect to AP");
        status = WIFI_FAILURE;
    } else {
        ESP_LOGE("wifi setup: ", "Unexpected event");
        status = ESP_FAIL;
    }

    return status;
}

esp_err_t server_connect() {
    char host_ip[] = HOST_IP;
    int addr_family = 0;
    int ip_protocol = 0;

    struct sockaddr_in dest_addr;
    inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;

    ex_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (ex_sock < 0) {
        ESP_LOGE("socket: ", "Unable to create a socket: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI("socket: ", "Socket created, connecting to %s:%d", host_ip, PORT);

    int err = connect(ex_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE("socket", "Socket unable to connect!");
        close(ex_sock);
        return ESP_FAIL;
    }

    ESP_LOGI("socket: ", "Socket connected!");
    xEventGroupSetBits(s_socket_event, SOCKET_CONNECTED_BIT);



    while (1) {

        UBaseType_t mem = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI("mem", "free left: %d", mem);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }


    return TCP_SUCCESS;
}

void ap_connection(void *arg) {
    esp_err_t ret = nvs_flash_init(), status = WIFI_FAILURE;

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    status = wifi_setup();
    if (status != WIFI_CONNECTED) {
        ESP_LOGE("main: ", "Failed to initialize wifi ap! Dying...");
        vTaskDelete(NULL);
    }

    status = server_connect();
    if (status != TCP_SUCCESS) {
        ESP_LOGE("main: ", "Failed to connect to Server");
        vTaskDelete(NULL);
    }

    vTaskDelete(NULL);
}