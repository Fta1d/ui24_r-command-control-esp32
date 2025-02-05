#ifndef MAIN_H
#define MAIN_H

#pragma once

#include "ui-client/client.h"
#include "adc/adc_setup.h"

#define SOCKET_CONNECTED_BIT BIT0

extern EventGroupHandle_t s_socket_event;

#endif  //MAIN_H