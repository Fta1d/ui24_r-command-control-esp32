#include "client.h"
#include <string.h>

#define UI_COMMAND_FORMAT "%s^i.%d.%s^%.4f" //6 float

extern int ex_sock;

void ui_command_manager(const char *cmd_type, uint8_t chanel, const char *cmd, float value) {
    char *request = (char *)malloc(strlen(cmd_type) + strlen(cmd) + 14);

    sprintf(request, UI_COMMAND_FORMAT, cmd_type, chanel, cmd, value);
    ESP_LOGI("ui_command_manager", "%s", request);

    int err = send(ex_sock, request, strlen(request), 0);
    if (err < 0) {
        ESP_LOGE("socket", "Error occurred during sending: errno %d", errno);
        free(request);
        return;
    }
    ESP_LOGI("socket", "%d bytes sent", err);
}