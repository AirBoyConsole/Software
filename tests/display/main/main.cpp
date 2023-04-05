#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "generic_display.hpp"
#include "ili9341_display.hpp"
#include "esp_log.h"

const uint16_t image_data_bluestone[4096] = {
    0x39c7, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x39c7, 0x39c7, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x39c7, 0x39c7, 0x000e, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x2965, 
    0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x2104, 0x2104, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x2104, 0x2104, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x2104, 
    0x0000, 0x0014, 0x0014, 0x0014, 0x0011, 0x0014, 0x000f, 0x0014, 0x0014, 0x0012, 0x000f, 0x0012, 0x0012, 0x000f, 0x0012, 0x0012, 0x000f, 0x0014, 0x000f, 0x000e, 0x000e, 0x000b, 0x0000, 0x2104, 0x0017, 0x0014, 0x0017, 0x0015, 0x000f, 0x0014, 0x0017, 0x0012, 0x000e, 0x000e, 0x0012, 0x0011, 0x0014, 0x0014, 0x0011, 0x000e, 0x0011, 0x0014, 0x0017, 0x0014, 0x0011, 0x0012, 0x0014, 0x0017, 0x0017, 0x0014, 0x0012, 0x000e, 0x0000, 0x2104, 0x0015, 0x0017, 0x0014, 0x0012, 0x0014, 0x0015, 0x0015, 0x0012, 0x000f, 0x0000, 
    0x0000, 0x0014, 0x0012, 0x000f, 0x0011, 0x0012, 0x0012, 0x0012, 0x000f, 0x000f, 0x0014, 0x0012, 0x0014, 0x000c, 0x0012, 0x0012, 0x000f, 0x000e, 0x000f, 0x000f, 0x000e, 0x000c, 0x0000, 0x2104, 0x0017, 0x0017, 0x0014, 0x0012, 0x0012, 0x0012, 0x000e, 0x0012, 0x000b, 0x000e, 0x000b, 0x000b, 0x000f, 0x0014, 0x0014, 0x000c, 0x000e, 0x000e, 0x000c, 0x000e, 0x000e, 0x000b, 0x000e, 0x0011, 0x0011, 0x000e, 0x000c, 0x000e, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0014, 0x000f, 0x0012, 0x0014, 0x0014, 0x000b, 0x0000, 
    0x0000, 0x0014, 0x0011, 0x0014, 0x000e, 0x0018, 0x0018, 0x0012, 0x000f, 0x000f, 0x000f, 0x0012, 0x0011, 0x0011, 0x000f, 0x000c, 0x000b, 0x000f, 0x000e, 0x000b, 0x000b, 0x000c, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0015, 0x0014, 0x0015, 0x0017, 0x0017, 0x0015, 0x0014, 0x000f, 0x000f, 0x0000, 
    0x0000, 0x0014, 0x0011, 0x000f, 0x0014, 0x0018, 0x0018, 0x000c, 0x000f, 0x000f, 0x000f, 0x0014, 0x0012, 0x0012, 0x0012, 0x0011, 0x000e, 0x000e, 0x000f, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0015, 0x0017, 0x0014, 0x0015, 0x0014, 0x000f, 0x000f, 0x000c, 0x000b, 0x0000, 
    0x0000, 0x0014, 0x0012, 0x0012, 0x0015, 0x0015, 0x0014, 0x0014, 0x0014, 0x0012, 0x0014, 0x0014, 0x0014, 0x0012, 0x0012, 0x000e, 0x000e, 0x000c, 0x000b, 0x000c, 0x000e, 0x000c, 0x0000, 0x2104, 0x0014, 0x0017, 0x0017, 0x0017, 0x0015, 0x0014, 0x0012, 0x0012, 0x0017, 0x0012, 0x0012, 0x0015, 0x0012, 0x0012, 0x0015, 0x0017, 0x0017, 0x0014, 0x0014, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0012, 0x0012, 0x000c, 0x2104, 0x2104, 0x0015, 0x0012, 0x0012, 0x0014, 0x000f, 0x0011, 0x0011, 0x000c, 0x000c, 0x0000, 
    0x0000, 0x0014, 0x0014, 0x0012, 0x0014, 0x0015, 0x0015, 0x0014, 0x0012, 0x000f, 0x000c, 0x0014, 0x0014, 0x0012, 0x0011, 0x0011, 0x000f, 0x000c, 0x000c, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x0017, 0x001b, 0x0018, 0x0015, 0x0014, 0x0017, 0x0014, 0x0012, 0x0012, 0x0012, 0x0015, 0x0015, 0x0014, 0x0014, 0x0014, 0x0012, 0x0014, 0x0000, 0x2104, 0x0014, 0x0012, 0x0012, 0x0011, 0x0012, 0x000f, 0x000c, 0x0000, 0x2104, 0x0015, 0x0012, 0x0012, 0x0014, 0x000e, 0x0011, 0x0011, 0x0011, 0x000c, 0x0000, 
    0x0000, 0x0014, 0x0012, 0x000f, 0x0012, 0x0014, 0x0014, 0x0011, 0x0012, 0x000f, 0x0012, 0x000e, 0x000e, 0x0011, 0x0011, 0x0011, 0x000e, 0x000f, 0x000f, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x0018, 0x0017, 0x0018, 0x0012, 0x0015, 0x0014, 0x0014, 0x0017, 0x0012, 0x0012, 0x0014, 0x000f, 0x0014, 0x000f, 0x0012, 0x0014, 0x000f, 0x0000, 0x2104, 0x0014, 0x0012, 0x0012, 0x0012, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0015, 0x0017, 0x0014, 0x0012, 0x0014, 0x0014, 0x0011, 0x0011, 0x0008, 0x0000, 
    0x0000, 0x0014, 0x0011, 0x0011, 0x0011, 0x0014, 0x000f, 0x000f, 0x000f, 0x0014, 0x0014, 0x000e, 0x000b, 0x000c, 0x000c, 0x000e, 0x000c, 0x000f, 0x000f, 0x000f, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x0015, 0x0015, 0x0012, 0x0012, 0x0012, 0x0015, 0x0014, 0x0014, 0x0014, 0x0014, 0x0012, 0x0011, 0x000c, 0x000f, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0012, 0x000f, 0x000f, 0x000b, 0x000b, 0x0000, 0x2104, 0x0017, 0x000f, 0x000f, 0x0012, 0x0014, 0x0014, 0x0012, 0x000b, 0x0008, 0x0000, 
    0x0000, 0x0012, 0x000f, 0x0011, 0x0014, 0x0011, 0x0011, 0x000e, 0x0012, 0x0014, 0x0014, 0x000e, 0x000e, 0x000c, 0x000b, 0x000c, 0x000c, 0x000c, 0x000f, 0x000f, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0017, 0x0015, 0x0015, 0x0015, 0x0012, 0x0012, 0x0015, 0x0017, 0x0017, 0x0014, 0x0014, 0x000c, 0x000c, 0x0011, 0x000f, 0x000f, 0x000c, 0x000b, 0x0000, 0x2104, 0x0014, 0x0014, 0x0012, 0x000f, 0x0014, 0x000f, 0x000b, 0x0000, 0x2104, 0x0017, 0x000f, 0x0017, 0x0014, 0x000f, 0x0012, 0x0012, 0x000b, 0x0008, 0x0000, 
    0x0000, 0x0012, 0x0011, 0x0012, 0x000f, 0x000f, 0x0014, 0x0011, 0x000c, 0x000f, 0x000b, 0x000f, 0x000c, 0x000b, 0x000c, 0x000f, 0x0008, 0x000b, 0x0008, 0x0008, 0x000c, 0x0008, 0x0000, 0x2104, 0x0014, 0x0017, 0x0014, 0x0014, 0x0015, 0x0015, 0x0015, 0x0015, 0x0017, 0x0015, 0x0015, 0x0014, 0x000c, 0x000c, 0x000e, 0x0014, 0x000c, 0x000c, 0x0008, 0x0000, 0x2104, 0x0014, 0x000f, 0x0011, 0x0012, 0x000f, 0x000b, 0x000b, 0x0000, 0x2104, 0x0017, 0x0017, 0x000f, 0x000f, 0x000c, 0x000c, 0x000f, 0x000f, 0x0008, 0x0000, 
    0x0000, 0x0012, 0x000f, 0x0012, 0x000f, 0x000c, 0x0011, 0x0011, 0x000f, 0x000c, 0x000f, 0x000c, 0x000f, 0x000c, 0x0008, 0x000e, 0x000e, 0x0008, 0x0008, 0x0008, 0x000c, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x0014, 0x0014, 0x0012, 0x0017, 0x0012, 0x0012, 0x0014, 0x0015, 0x0015, 0x0014, 0x0012, 0x0012, 0x000f, 0x000f, 0x000b, 0x000b, 0x000c, 0x0000, 0x2104, 0x0014, 0x0011, 0x0011, 0x0011, 0x000e, 0x000e, 0x000c, 0x0000, 0x2104, 0x0017, 0x000f, 0x000e, 0x000c, 0x000f, 0x000c, 0x000f, 0x000f, 0x0008, 0x0000, 
    0x0000, 0x0012, 0x0012, 0x000f, 0x000f, 0x000f, 0x000e, 0x000c, 0x000b, 0x000b, 0x000c, 0x000f, 0x000c, 0x000e, 0x000c, 0x0008, 0x0008, 0x000e, 0x0008, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x001b, 0x0018, 0x0011, 0x0011, 0x0012, 0x0012, 0x0011, 0x000f, 0x000f, 0x000f, 0x0011, 0x0012, 0x0011, 0x000f, 0x000c, 0x000c, 0x0008, 0x0000, 0x2104, 0x0014, 0x0014, 0x0011, 0x0011, 0x000e, 0x000e, 0x000c, 0x0000, 0x2104, 0x0017, 0x000f, 0x000f, 0x000e, 0x000b, 0x000b, 0x000b, 0x000b, 0x0008, 0x0000, 
    0x0000, 0x0012, 0x000f, 0x0012, 0x000c, 0x000c, 0x000e, 0x000c, 0x000f, 0x000c, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0014, 0x0017, 0x0018, 0x0018, 0x0011, 0x0011, 0x0011, 0x0014, 0x0014, 0x000f, 0x0011, 0x0011, 0x000f, 0x0011, 0x000e, 0x000f, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x000f, 0x0012, 0x0014, 0x000b, 0x000b, 0x000b, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0014, 0x0017, 0x0014, 0x0014, 0x0012, 0x0011, 0x0011, 0x0014, 0x0014, 0x0014, 0x0011, 0x0011, 0x0012, 0x000c, 0x000c, 0x000f, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0012, 0x0014, 0x000b, 0x000c, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 
    0x0014, 0x0017, 0x0015, 0x0014, 0x0015, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0015, 0x0014, 0x0014, 0x0014, 0x0014, 0x0000, 0x0015, 0x0015, 0x0015, 0x0014, 0x0014, 0x0012, 0x0012, 0x0000, 0x0014, 0x0015, 0x0012, 0x0014, 0x0011, 0x0012, 0x0014, 0x0014, 0x0014, 0x0014, 0x000f, 0x0012, 0x0012, 0x000f, 0x000f, 0x000b, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0012, 0x0014, 0x0012, 0x0014, 0x0012, 0x000e, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0000, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 
    0x0017, 0x0015, 0x0014, 0x0012, 0x0014, 0x0017, 0x0015, 0x0014, 0x0011, 0x0014, 0x0014, 0x000f, 0x0014, 0x0012, 0x000f, 0x0000, 0x0015, 0x0015, 0x0012, 0x0017, 0x0017, 0x000f, 0x0017, 0x0000, 0x0014, 0x0014, 0x0014, 0x0014, 0x0017, 0x0014, 0x0012, 0x0014, 0x0014, 0x0014, 0x0011, 0x000c, 0x000e, 0x000f, 0x0008, 0x0008, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0012, 0x0012, 0x000f, 0x000e, 0x000f, 0x000e, 0x000b, 0x0000, 0x2104, 0x0014, 0x0015, 0x0011, 0x000f, 0x0000, 0x0017, 0x0017, 0x0014, 0x0015, 0x0017, 
    0x0017, 0x0015, 0x0012, 0x0012, 0x0011, 0x0014, 0x0014, 0x0011, 0x000f, 0x0012, 0x0012, 0x000f, 0x000f, 0x0012, 0x0012, 0x0000, 0x0015, 0x0017, 0x0017, 0x0015, 0x0012, 0x000f, 0x000f, 0x0000, 0x0014, 0x0015, 0x0012, 0x0014, 0x0017, 0x0014, 0x0011, 0x000f, 0x0011, 0x0011, 0x000c, 0x000f, 0x000f, 0x000e, 0x0008, 0x0008, 0x000c, 0x0008, 0x0008, 0x0000, 0x2104, 0x0012, 0x0012, 0x000b, 0x000b, 0x000b, 0x000c, 0x0008, 0x0000, 0x2104, 0x0014, 0x0015, 0x0015, 0x000f, 0x0000, 0x0017, 0x0014, 0x0015, 0x0015, 0x0015, 
    0x0015, 0x0014, 0x0012, 0x0015, 0x0015, 0x0012, 0x0012, 0x000f, 0x000f, 0x000f, 0x0012, 0x000f, 0x000f, 0x000f, 0x000f, 0x0000, 0x0015, 0x0017, 0x0015, 0x0015, 0x0012, 0x0012, 0x000c, 0x0000, 0x0014, 0x0015, 0x0014, 0x0014, 0x0014, 0x0011, 0x0014, 0x000f, 0x000f, 0x000c, 0x000e, 0x000e, 0x000b, 0x000b, 0x000b, 0x000c, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0012, 0x000f, 0x000b, 0x000c, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0014, 0x0011, 0x000f, 0x000b, 0x0000, 0x0017, 0x0017, 0x0017, 0x0015, 0x0015, 
    0x0014, 0x0011, 0x0012, 0x0015, 0x0017, 0x0015, 0x000f, 0x000f, 0x0012, 0x0012, 0x000f, 0x0012, 0x000b, 0x000f, 0x000f, 0x0000, 0x0015, 0x0017, 0x0012, 0x000f, 0x0012, 0x0012, 0x000c, 0x0000, 0x0014, 0x0014, 0x0014, 0x0012, 0x0012, 0x0012, 0x0012, 0x000f, 0x000c, 0x000b, 0x000c, 0x000c, 0x000b, 0x000c, 0x000c, 0x000b, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0012, 0x000f, 0x000c, 0x000b, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0014, 0x000b, 0x000c, 0x000b, 0x0000, 0x0017, 0x0017, 0x0015, 0x0015, 0x0015, 
    0x0014, 0x0014, 0x0011, 0x0012, 0x0015, 0x001a, 0x000f, 0x0012, 0x000f, 0x000f, 0x000c, 0x000c, 0x000e, 0x000f, 0x000f, 0x0000, 0x0015, 0x0012, 0x0017, 0x000f, 0x000f, 0x000f, 0x000c, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0017, 0x0017, 0x0014, 0x0014, 0x0012, 
    0x0015, 0x0011, 0x000f, 0x0011, 0x0012, 0x0014, 0x0012, 0x000f, 0x000f, 0x0012, 0x000c, 0x000e, 0x000f, 0x000b, 0x000b, 0x0000, 0x0015, 0x0012, 0x0012, 0x000c, 0x000f, 0x000f, 0x000b, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x2104, 0x2104, 0x0017, 0x0014, 0x0014, 0x0012, 0x0012, 
    0x0015, 0x0014, 0x0011, 0x0012, 0x000f, 0x0012, 0x000f, 0x0012, 0x0012, 0x000f, 0x000e, 0x000f, 0x000c, 0x000f, 0x000b, 0x0000, 0x0015, 0x0017, 0x0012, 0x0011, 0x000c, 0x000b, 0x000c, 0x0000, 0x2104, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0015, 0x0012, 0x0014, 0x2104, 0x2104, 0x0015, 0x0018, 0x0015, 0x0018, 0x0014, 0x0014, 0x0014, 0x0012, 0x0000, 0x2104, 0x0017, 0x0015, 0x0012, 0x0012, 0x0012, 
    0x0015, 0x0015, 0x0012, 0x0011, 0x0015, 0x0012, 0x0012, 0x0012, 0x0012, 0x000f, 0x000f, 0x000e, 0x000b, 0x000b, 0x000b, 0x0000, 0x0014, 0x0017, 0x000f, 0x0011, 0x0011, 0x000c, 0x000b, 0x0000, 0x2104, 0x0015, 0x0017, 0x0015, 0x0014, 0x0017, 0x0017, 0x0017, 0x0015, 0x0017, 0x0014, 0x0015, 0x0014, 0x0017, 0x0015, 0x000f, 0x0014, 0x0017, 0x0017, 0x0017, 0x000f, 0x0014, 0x000b, 0x0000, 0x2104, 0x0015, 0x0018, 0x0015, 0x0018, 0x0018, 0x0014, 0x0012, 0x0011, 0x0000, 0x2104, 0x0017, 0x0017, 0x0012, 0x0012, 0x0012, 
    0x0015, 0x0014, 0x0011, 0x000f, 0x0011, 0x0015, 0x0015, 0x000c, 0x000f, 0x0012, 0x0011, 0x0011, 0x000e, 0x000b, 0x000c, 0x0000, 0x0014, 0x000f, 0x000c, 0x000e, 0x000e, 0x000b, 0x000c, 0x0000, 0x2104, 0x0015, 0x0017, 0x0014, 0x0015, 0x0018, 0x001b, 0x001b, 0x0017, 0x001a, 0x001b, 0x0014, 0x0017, 0x0014, 0x0014, 0x0017, 0x0017, 0x0014, 0x0014, 0x0014, 0x0014, 0x000f, 0x000b, 0x0000, 0x2104, 0x0015, 0x0018, 0x0018, 0x0018, 0x0018, 0x0014, 0x0011, 0x000f, 0x0000, 0x2104, 0x0017, 0x0015, 0x0017, 0x0014, 0x0014, 
    0x0014, 0x0012, 0x000f, 0x0011, 0x0014, 0x0015, 0x0015, 0x0014, 0x0012, 0x000f, 0x0011, 0x0011, 0x000e, 0x000b, 0x000c, 0x0000, 0x0012, 0x000f, 0x000b, 0x000b, 0x000b, 0x000b, 0x000c, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0017, 0x001a, 0x001a, 0x001b, 0x0017, 0x001b, 0x001a, 0x001a, 0x0015, 0x000f, 0x0014, 0x000f, 0x000f, 0x0014, 0x000f, 0x000f, 0x0014, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0018, 0x0014, 0x0011, 0x0014, 0x000c, 0x0000, 0x2104, 0x0017, 0x0017, 0x0017, 0x0014, 0x0014, 
    0x0015, 0x0012, 0x0015, 0x0012, 0x0012, 0x0014, 0x0015, 0x0014, 0x000c, 0x000f, 0x000e, 0x000e, 0x000e, 0x000b, 0x000b, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0015, 0x0017, 0x0017, 0x0015, 0x0015, 0x0018, 0x0018, 0x0014, 0x0014, 0x001a, 0x001a, 0x0014, 0x0014, 0x000f, 0x000f, 0x000f, 0x000e, 0x0014, 0x000f, 0x000f, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0018, 0x0014, 0x0014, 0x0014, 0x0014, 0x0011, 0x000c, 0x0000, 0x2104, 0x0017, 0x0017, 0x0017, 0x0015, 0x0015, 
    0x0014, 0x0011, 0x0011, 0x0014, 0x0014, 0x0012, 0x0014, 0x0014, 0x000c, 0x000e, 0x000b, 0x000c, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0015, 0x0015, 0x0017, 0x0015, 0x0015, 0x0015, 0x0014, 0x0014, 0x0014, 0x0017, 0x0014, 0x0014, 0x0014, 0x000f, 0x0011, 0x0011, 0x000f, 0x000f, 0x000b, 0x000b, 0x000b, 0x000c, 0x0000, 0x2104, 0x0015, 0x0018, 0x0018, 0x0014, 0x0014, 0x0011, 0x000f, 0x000e, 0x0000, 0x2104, 0x0017, 0x0014, 0x0014, 0x0017, 0x0015, 
    0x0014, 0x0012, 0x0011, 0x0014, 0x0014, 0x0014, 0x0012, 0x000f, 0x000e, 0x000c, 0x000b, 0x000c, 0x000c, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0015, 0x0015, 0x0015, 0x0014, 0x0014, 0x2104, 0x2104, 0x0015, 0x0014, 0x0014, 0x0017, 0x0015, 0x0011, 0x0011, 0x0014, 0x0017, 0x0015, 0x000f, 0x0014, 0x0014, 0x000f, 0x0011, 0x0011, 0x0011, 0x000e, 0x000e, 0x000e, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0018, 0x0018, 0x0012, 0x0012, 0x0014, 0x0014, 0x000b, 0x0000, 0x2104, 0x0017, 0x0014, 0x0014, 0x0015, 0x0015, 
    0x0012, 0x0011, 0x0012, 0x0011, 0x0014, 0x0014, 0x000f, 0x000c, 0x000c, 0x000b, 0x000b, 0x000b, 0x000c, 0x0008, 0x000c, 0x0000, 0x2104, 0x0015, 0x0015, 0x0011, 0x0014, 0x0012, 0x0011, 0x0000, 0x2104, 0x0015, 0x0017, 0x0015, 0x0017, 0x0015, 0x0011, 0x0011, 0x0014, 0x0014, 0x0014, 0x000f, 0x0014, 0x0014, 0x0014, 0x000e, 0x0011, 0x0011, 0x000b, 0x000e, 0x000e, 0x000e, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0018, 0x0012, 0x0012, 0x0012, 0x0014, 0x000b, 0x0000, 0x2104, 0x0017, 0x0017, 0x0012, 0x0012, 0x000f, 
    0x0012, 0x0011, 0x0011, 0x000f, 0x0011, 0x0011, 0x000f, 0x000c, 0x000c, 0x000e, 0x000f, 0x000f, 0x000c, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0014, 0x0014, 0x0012, 0x0011, 0x0012, 0x0000, 0x2104, 0x0015, 0x0017, 0x0017, 0x0014, 0x0015, 0x0015, 0x0015, 0x0012, 0x0014, 0x000f, 0x000f, 0x000f, 0x0014, 0x0014, 0x000e, 0x000b, 0x000c, 0x000b, 0x000c, 0x000e, 0x000e, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0011, 0x0015, 0x0014, 0x0014, 0x000c, 0x0000, 0x2104, 0x0017, 0x0011, 0x0011, 0x0012, 0x0012, 
    0x0012, 0x0011, 0x0014, 0x0012, 0x000e, 0x000f, 0x000f, 0x000e, 0x000c, 0x000e, 0x000f, 0x000f, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0015, 0x0014, 0x0012, 0x0011, 0x000e, 0x000c, 0x0000, 0x2104, 0x0015, 0x0017, 0x0014, 0x000f, 0x0014, 0x0015, 0x0015, 0x0014, 0x0014, 0x000f, 0x000e, 0x000f, 0x000c, 0x000e, 0x000e, 0x000e, 0x000b, 0x0008, 0x0008, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0018, 0x0011, 0x0015, 0x0015, 0x0012, 0x000c, 0x0000, 0x2104, 0x0017, 0x0011, 0x0011, 0x000f, 0x000f, 
    0x0012, 0x0014, 0x0012, 0x0011, 0x0011, 0x000f, 0x000c, 0x000c, 0x000b, 0x0009, 0x0009, 0x000e, 0x000c, 0x000b, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0011, 0x0015, 0x0011, 0x000b, 0x0000, 0x2104, 0x0015, 0x0014, 0x0014, 0x0017, 0x000f, 0x0014, 0x0014, 0x0012, 0x000f, 0x000e, 0x000f, 0x000f, 0x000c, 0x000c, 0x000e, 0x000e, 0x000c, 0x0008, 0x0008, 0x000c, 0x000c, 0x0008, 0x0000, 0x2104, 0x0015, 0x0014, 0x0018, 0x0014, 0x0011, 0x0012, 0x0012, 0x000b, 0x0000, 0x2104, 0x0017, 0x0017, 0x000f, 0x000f, 0x000f, 
    0x000f, 0x0012, 0x0012, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x000b, 0x0009, 0x0009, 0x000b, 0x000b, 0x000c, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0015, 0x000e, 0x000e, 0x000c, 0x0000, 0x2104, 0x0015, 0x000f, 0x0017, 0x000f, 0x0014, 0x0014, 0x000f, 0x000f, 0x000e, 0x000c, 0x000f, 0x000f, 0x000f, 0x000c, 0x000c, 0x000b, 0x000b, 0x000c, 0x000c, 0x000c, 0x0009, 0x0008, 0x0000, 0x2104, 0x0015, 0x0018, 0x0014, 0x0014, 0x0012, 0x000b, 0x000e, 0x000c, 0x0000, 0x2104, 0x0017, 0x0014, 0x0014, 0x0012, 0x0012, 
    0x000f, 0x0011, 0x0011, 0x000f, 0x000e, 0x000f, 0x000f, 0x000f, 0x000b, 0x000b, 0x000c, 0x000b, 0x000b, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0015, 0x0015, 0x0014, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0014, 0x0017, 0x000f, 0x0014, 0x0014, 0x000e, 0x000c, 0x000c, 0x000b, 0x000f, 0x000f, 0x000f, 0x000c, 0x000b, 0x000b, 0x000b, 0x000b, 0x0009, 0x0009, 0x0009, 0x0008, 0x0000, 0x2104, 0x0015, 0x0018, 0x0014, 0x0012, 0x000f, 0x0012, 0x000e, 0x000b, 0x0000, 0x2104, 0x0017, 0x0014, 0x0014, 0x0012, 0x0011, 
    0x000f, 0x0011, 0x000c, 0x000e, 0x000f, 0x000c, 0x000f, 0x000f, 0x000b, 0x000c, 0x000b, 0x0008, 0x000b, 0x000c, 0x000b, 0x0000, 0x2104, 0x0015, 0x0012, 0x0012, 0x000b, 0x0011, 0x000c, 0x0000, 0x2104, 0x0015, 0x0014, 0x000f, 0x0014, 0x0014, 0x000f, 0x000b, 0x000b, 0x000b, 0x000b, 0x000c, 0x000c, 0x000c, 0x000c, 0x000b, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0018, 0x0014, 0x000f, 0x0012, 0x0012, 0x000f, 0x000c, 0x0000, 0x2104, 0x0017, 0x0012, 0x0014, 0x0014, 0x000f, 
    0x000f, 0x0012, 0x000f, 0x000c, 0x000f, 0x000c, 0x000e, 0x000b, 0x000c, 0x0008, 0x0008, 0x000b, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0012, 0x0012, 0x0015, 0x000b, 0x000c, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0014, 0x0015, 0x0014, 0x000f, 0x000c, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0017, 0x0012, 0x0011, 0x0011, 0x000f, 
    0x000f, 0x0011, 0x0012, 0x0011, 0x000e, 0x000f, 0x000e, 0x000b, 0x000b, 0x000b, 0x0008, 0x000c, 0x0008, 0x000b, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0011, 0x0015, 0x0011, 0x000b, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0014, 0x0015, 0x0015, 0x0014, 0x000f, 0x000b, 0x000c, 0x0008, 0x0000, 0x2104, 0x0017, 0x0017, 0x0011, 0x0011, 0x000f, 
    0x000f, 0x0012, 0x0011, 0x000f, 0x000c, 0x000e, 0x000f, 0x000e, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0011, 0x0011, 0x000b, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x2104, 0x2104, 0x0014, 0x0015, 0x0014, 0x0017, 0x0014, 0x000f, 0x000c, 0x0008, 0x0000, 0x2104, 0x0017, 0x0012, 0x0012, 0x000f, 0x0011, 
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0015, 0x0015, 0x000f, 0x000f, 0x000b, 0x000b, 0x0000, 0x2104, 0x0014, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0015, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0014, 0x0012, 0x0011, 0x000f, 0x0017, 0x0012, 0x0014, 0x000f, 0x0000, 0x2104, 0x0014, 0x0018, 0x0014, 0x0014, 0x0017, 0x0011, 0x000b, 0x0008, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
    0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x000b, 0x000b, 0x000e, 0x000e, 0x0011, 0x0011, 0x0011, 0x000f, 0x000e, 0x000c, 0x2104, 0x2104, 0x0014, 0x0015, 0x000f, 0x000f, 0x000f, 0x000b, 0x0000, 0x2104, 0x0015, 0x0017, 0x0018, 0x0018, 0x0017, 0x0014, 0x0014, 0x0015, 0x0015, 0x0018, 0x0017, 0x0012, 0x0012, 0x0014, 0x0014, 0x0012, 0x000f, 0x0011, 0x000f, 0x0011, 0x0012, 0x000f, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0014, 0x0011, 0x0011, 0x000b, 0x0008, 0x0000, 0x2104, 0x0011, 0x0012, 0x0011, 0x0011, 0x0000, 
    0x2104, 0x0012, 0x0014, 0x0015, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x2104, 0x2104, 0x0014, 0x0014, 0x0011, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0015, 0x0017, 0x0018, 0x0018, 0x0018, 0x0015, 0x0015, 0x0015, 0x0015, 0x0017, 0x0017, 0x000f, 0x000f, 0x000f, 0x000f, 0x0014, 0x0014, 0x0014, 0x0012, 0x0012, 0x000f, 0x000c, 0x0000, 0x2104, 0x0014, 0x0018, 0x0014, 0x000f, 0x000e, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0012, 0x0012, 0x000f, 0x000f, 0x0000, 
    0x2104, 0x0014, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0015, 0x0014, 0x0015, 0x0017, 0x0014, 0x0014, 0x0014, 0x0017, 0x0000, 0x2104, 0x0014, 0x0014, 0x0012, 0x000b, 0x000c, 0x000c, 0x0000, 0x2104, 0x0015, 0x0017, 0x0017, 0x0017, 0x0017, 0x001a, 0x001a, 0x0015, 0x0015, 0x0015, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x0012, 0x000f, 0x000f, 0x000e, 0x0012, 0x000c, 0x0000, 0x2104, 0x0014, 0x0015, 0x0011, 0x000e, 0x000c, 0x000b, 0x000e, 0x0008, 0x0000, 0x2104, 0x0012, 0x000f, 0x000e, 0x000e, 0x0000, 
    0x2104, 0x0014, 0x0017, 0x0015, 0x0015, 0x001a, 0x001a, 0x0015, 0x0017, 0x0017, 0x0017, 0x0012, 0x0014, 0x0017, 0x0017, 0x0000, 0x2104, 0x0014, 0x0012, 0x0011, 0x000b, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0017, 0x0017, 0x0015, 0x0017, 0x001a, 0x001a, 0x001a, 0x000f, 0x0014, 0x0012, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x000e, 0x000c, 0x000e, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0012, 0x000e, 0x000f, 0x000b, 0x0000, 
    0x2104, 0x0017, 0x0017, 0x0017, 0x0015, 0x001a, 0x001a, 0x0017, 0x0012, 0x0012, 0x0014, 0x0017, 0x0017, 0x0012, 0x0015, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0014, 0x0015, 0x0017, 0x0015, 0x0017, 0x0017, 0x001a, 0x001a, 0x0017, 0x0014, 0x0014, 0x0012, 0x000f, 0x000e, 0x000e, 0x000c, 0x000c, 0x000b, 0x000f, 0x000f, 0x000f, 0x000c, 0x0000, 0x2104, 0x0011, 0x000f, 0x000f, 0x000b, 0x000b, 0x000b, 0x000c, 0x0008, 0x0000, 0x2104, 0x0011, 0x000f, 0x000e, 0x000b, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0015, 0x0014, 0x0015, 0x0015, 0x0015, 0x0012, 0x0012, 0x0012, 0x0017, 0x0014, 0x0011, 0x0011, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0014, 0x0014, 0x0015, 0x0018, 0x0012, 0x0012, 0x0017, 0x0014, 0x0017, 0x0014, 0x0014, 0x0014, 0x0014, 0x000f, 0x000f, 0x000c, 0x000c, 0x000f, 0x000c, 0x000b, 0x0009, 0x0009, 0x0000, 0x2104, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2104, 0x0011, 0x000e, 0x000b, 0x000c, 0x0000, 
    0x2104, 0x0017, 0x0017, 0x0015, 0x0012, 0x0011, 0x0015, 0x0015, 0x0014, 0x0012, 0x0012, 0x0014, 0x0017, 0x0017, 0x0014, 0x0000, 0x2104, 0x0017, 0x0017, 0x0015, 0x0014, 0x0014, 0x0012, 0x2104, 0x2104, 0x0014, 0x0014, 0x0018, 0x0018, 0x0012, 0x0012, 0x0012, 0x0015, 0x0014, 0x0014, 0x0014, 0x000f, 0x000f, 0x0012, 0x0011, 0x000e, 0x000c, 0x000c, 0x000b, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x2104, 0x0000, 
    0x2104, 0x0017, 0x0017, 0x0012, 0x0014, 0x0017, 0x0015, 0x0011, 0x0014, 0x0015, 0x0015, 0x0015, 0x000f, 0x0014, 0x0014, 0x0000, 0x2104, 0x0017, 0x000f, 0x0017, 0x0011, 0x0012, 0x0011, 0x0000, 0x2104, 0x0014, 0x0017, 0x0017, 0x0017, 0x0015, 0x0012, 0x0012, 0x0017, 0x0017, 0x0017, 0x0014, 0x000f, 0x000f, 0x000f, 0x000f, 0x000c, 0x000c, 0x000c, 0x000b, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0015, 0x0015, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0017, 0x0000, 
    0x2104, 0x0017, 0x0017, 0x0017, 0x0017, 0x0014, 0x0011, 0x0017, 0x0011, 0x0015, 0x000f, 0x000f, 0x0014, 0x0011, 0x0014, 0x0000, 0x2104, 0x0017, 0x0014, 0x0015, 0x0015, 0x0012, 0x000f, 0x0000, 0x2104, 0x0014, 0x0015, 0x0014, 0x0014, 0x0015, 0x0015, 0x0015, 0x0017, 0x0017, 0x0017, 0x0017, 0x0014, 0x000f, 0x0011, 0x000e, 0x000b, 0x000c, 0x000c, 0x000c, 0x000e, 0x000b, 0x0009, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0014, 0x0015, 0x0015, 0x0015, 0x0015, 0x0014, 0x0011, 0x0017, 0x0014, 0x0014, 0x000f, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0017, 0x0012, 0x0014, 0x0014, 0x0017, 0x0015, 0x0012, 0x000f, 0x0009, 0x000f, 0x0014, 0x0014, 0x0000, 0x2104, 0x0017, 0x0017, 0x000f, 0x0014, 0x000f, 0x000e, 0x0000, 0x2104, 0x0014, 0x0017, 0x0014, 0x0014, 0x0014, 0x0012, 0x0014, 0x0017, 0x0017, 0x0017, 0x0017, 0x0014, 0x0014, 0x0014, 0x000b, 0x000b, 0x000b, 0x000b, 0x000c, 0x000e, 0x000e, 0x000c, 0x0000, 0x2104, 0x0015, 0x0014, 0x0015, 0x001a, 0x001a, 0x0015, 0x0015, 0x0015, 0x0012, 0x0014, 0x0014, 0x0014, 0x0012, 0x000f, 0x0000, 
    0x2104, 0x0017, 0x0015, 0x0012, 0x0012, 0x0017, 0x001b, 0x0017, 0x0014, 0x0011, 0x0014, 0x000f, 0x000f, 0x0014, 0x0014, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x0014, 0x000f, 0x0014, 0x0000, 0x2104, 0x0014, 0x0017, 0x0017, 0x0014, 0x0014, 0x0017, 0x0011, 0x0011, 0x0017, 0x0017, 0x0017, 0x0014, 0x0014, 0x0014, 0x000e, 0x000b, 0x000b, 0x000c, 0x000b, 0x0009, 0x0009, 0x0009, 0x0000, 0x2104, 0x0015, 0x0017, 0x0017, 0x001a, 0x0018, 0x0018, 0x0015, 0x0015, 0x0011, 0x0011, 0x0014, 0x0012, 0x000f, 0x000c, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0017, 0x0014, 0x001b, 0x421f, 0x001b, 0x0012, 0x0012, 0x0014, 0x0012, 0x0011, 0x000f, 0x0011, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x000f, 0x0014, 0x000e, 0x0000, 0x2104, 0x0014, 0x0014, 0x0017, 0x0012, 0x0012, 0x000f, 0x000f, 0x000f, 0x0011, 0x0014, 0x0014, 0x0014, 0x000f, 0x000f, 0x000f, 0x000c, 0x000c, 0x000b, 0x000b, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0012, 0x0017, 0x0012, 0x0018, 0x0018, 0x0015, 0x0015, 0x0014, 0x0014, 0x000f, 0x000f, 0x000f, 0x000b, 0x0000, 
    0x2104, 0x0017, 0x0015, 0x0017, 0x0015, 0x0017, 0x001b, 0x0017, 0x0014, 0x000f, 0x0014, 0x0014, 0x0009, 0x0009, 0x000c, 0x0000, 0x2104, 0x0017, 0x0014, 0x0015, 0x0015, 0x000f, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0017, 0x0014, 0x0011, 0x0011, 0x000f, 0x000f, 0x000f, 0x0011, 0x000f, 0x0011, 0x000b, 0x000b, 0x0012, 0x0012, 0x000e, 0x000e, 0x000b, 0x0009, 0x0008, 0x0008, 0x0000, 0x2104, 0x0015, 0x0017, 0x0015, 0x0012, 0x0012, 0x0012, 0x0014, 0x0014, 0x000f, 0x000f, 0x000f, 0x000f, 0x000b, 0x000c, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0017, 0x0015, 0x0015, 0x0015, 0x0011, 0x0011, 0x0011, 0x000f, 0x000e, 0x0009, 0x0009, 0x000c, 0x0000, 0x2104, 0x0017, 0x0017, 0x0015, 0x0012, 0x0011, 0x000c, 0x0000, 0x2104, 0x0014, 0x0012, 0x0017, 0x0017, 0x0012, 0x0017, 0x000f, 0x000f, 0x0011, 0x000f, 0x000f, 0x0011, 0x000f, 0x000b, 0x0012, 0x0012, 0x0011, 0x000e, 0x000e, 0x000c, 0x000b, 0x000c, 0x0000, 0x2104, 0x0015, 0x0015, 0x0014, 0x0012, 0x0012, 0x0012, 0x0014, 0x000f, 0x000e, 0x000c, 0x000f, 0x000b, 0x000b, 0x000c, 0x0000, 
    0x2104, 0x0014, 0x0017, 0x0015, 0x0015, 0x0014, 0x000f, 0x000f, 0x0012, 0x0014, 0x0012, 0x0012, 0x000f, 0x000c, 0x000c, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x0011, 0x0011, 0x000f, 0x0000, 0x2104, 0x0014, 0x0017, 0x0012, 0x0014, 0x0017, 0x0014, 0x0011, 0x000f, 0x000e, 0x000e, 0x0011, 0x0011, 0x000b, 0x000b, 0x000f, 0x0012, 0x0012, 0x000f, 0x000e, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0012, 0x0014, 0x0012, 0x0012, 0x0014, 0x000f, 0x000e, 0x000c, 0x000e, 0x000f, 0x000b, 0x000b, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0017, 0x0015, 0x0011, 0x0011, 0x0015, 0x000f, 0x0014, 0x0014, 0x0012, 0x000e, 0x000f, 0x000b, 0x0000, 0x2104, 0x0017, 0x0014, 0x0017, 0x000f, 0x000f, 0x000f, 0x0000, 0x2104, 0x0014, 0x0017, 0x0014, 0x0014, 0x0014, 0x0012, 0x0011, 0x0014, 0x0014, 0x000e, 0x000e, 0x000b, 0x000e, 0x000b, 0x000b, 0x000f, 0x000f, 0x000f, 0x000c, 0x000b, 0x000b, 0x0008, 0x0000, 0x2104, 0x0015, 0x0015, 0x0017, 0x0015, 0x0011, 0x0015, 0x0015, 0x0012, 0x000f, 0x000c, 0x0012, 0x0012, 0x000b, 0x0008, 0x0000, 
    0x2104, 0x0017, 0x0014, 0x0017, 0x0014, 0x0012, 0x0011, 0x000f, 0x000c, 0x0011, 0x0014, 0x0014, 0x0012, 0x000e, 0x000c, 0x0000, 0x2104, 0x0017, 0x0017, 0x0014, 0x000c, 0x000c, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0014, 0x0012, 0x0012, 0x000f, 0x0011, 0x0014, 0x0014, 0x0014, 0x000e, 0x000c, 0x000e, 0x000e, 0x000b, 0x0009, 0x0009, 0x0009, 0x000c, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0017, 0x0017, 0x0015, 0x0011, 0x0014, 0x0015, 0x0015, 0x0015, 0x0012, 0x000f, 0x0012, 0x0012, 0x000f, 0x0008, 0x0000, 
    0x2104, 0x0015, 0x0014, 0x0011, 0x0011, 0x0011, 0x0014, 0x000c, 0x0009, 0x000c, 0x0011, 0x0012, 0x0012, 0x000e, 0x000e, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x000f, 0x000e, 0x000b, 0x0000, 0x2104, 0x0014, 0x0015, 0x0014, 0x0012, 0x0012, 0x0017, 0x0014, 0x000f, 0x0014, 0x0014, 0x000b, 0x000b, 0x000e, 0x000e, 0x000c, 0x000b, 0x0008, 0x0008, 0x000b, 0x0008, 0x0008, 0x0008, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x0011, 0x0014, 0x0014, 0x0015, 0x0015, 0x0012, 0x000f, 0x000f, 0x000f, 0x000f, 0x0008, 0x0000, 
    0x2104, 0x0015, 0x0014, 0x000e, 0x000c, 0x0011, 0x000c, 0x0014, 0x000f, 0x000f, 0x0012, 0x000b, 0x000c, 0x000b, 0x000b, 0x0000, 0x2104, 0x0015, 0x0014, 0x000c, 0x000c, 0x000c, 0x000b, 0x0000, 0x2104, 0x0014, 0x0015, 0x0017, 0x000f, 0x0017, 0x0017, 0x0014, 0x000b, 0x000c, 0x000c, 0x000c, 0x000b, 0x000b, 0x0009, 0x0009, 0x0009, 0x0008, 0x0008, 0x0009, 0x000b, 0x0008, 0x0008, 0x0000, 0x2104, 0x0017, 0x0015, 0x0014, 0x0017, 0x0014, 0x0014, 0x000f, 0x0011, 0x000e, 0x000e, 0x000f, 0x000f, 0x000b, 0x0008, 0x0000, 
    0x2104, 0x0014, 0x0014, 0x0011, 0x0011, 0x000c, 0x0011, 0x000b, 0x000c, 0x000b, 0x000c, 0x000e, 0x000c, 0x000e, 0x000c, 0x0000, 0x2104, 0x0015, 0x0014, 0x0011, 0x000e, 0x000c, 0x000c, 0x0000, 0x2104, 0x0014, 0x0014, 0x0015, 0x0017, 0x0014, 0x000f, 0x0012, 0x000b, 0x000c, 0x000c, 0x000c, 0x000b, 0x000b, 0x0008, 0x0008, 0x000e, 0x000e, 0x000c, 0x0008, 0x0009, 0x0008, 0x000b, 0x0000, 0x2104, 0x0017, 0x0017, 0x0017, 0x0012, 0x0014, 0x000f, 0x000f, 0x000e, 0x000f, 0x000f, 0x000e, 0x000c, 0x0008, 0x000c, 0x0000, 
    0x2104, 0x0014, 0x0012, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000e, 0x000e, 0x000f, 0x000f, 0x000f, 0x2104, 0x2104, 0x0014, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x2104, 0x2104, 0x0014, 0x0014, 0x0012, 0x0014, 0x0012, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000c, 0x000b, 0x000b, 0x000f, 0x000f, 0x000f, 0x000f, 0x000b, 0x000b, 0x000e, 0x2104, 0x2104, 0x0017, 0x0014, 0x0014, 0x000f, 0x0012, 0x0014, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000e, 0x000e, 0x000f, 0x0000, 
    0x39c7, 0x0011, 0x0011, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x39c7, 0x39c7, 0x0011, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x39c7, 0x39c7, 0x0014, 0x0018, 0x0011, 0x0011, 0x0011, 0x0011, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x39c7, 0x39c7, 0x0017, 0x0011, 0x0011, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x2104, 
    0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x52aa, 0x39c7
};

static const char *TAG = "display";

extern "C" void app_main(void)
{
    airboy::bus_cfg_t config{
        .bus_bits = {13, 12, 11, 10, 9, 8, 7, 6},
        .CLK = 16, 
        .DC = 17,
        .CS = 18,
        .RST = 14,
        .RD = 15,
        .BL = 2,
        .clock = 40000000
    };

    ESP_LOGI(TAG, "wolne: %d", heap_caps_get_free_size(MALLOC_CAP_DMA));

    airboy::GenericDisplay *display = new airboy::ILI9341Display(&config);

    ESP_LOGI(TAG, "wolne: %d", heap_caps_get_free_size(MALLOC_CAP_DMA));

    ESP_LOGI(TAG, "wejscie");
    //ESP_LOGI(TAG, "typ %s", typeid(display->io).name());
    ESP_LOGI(TAG, "adress %p", &display->io);

    ESP_LOGI(TAG, "wiadomosc");

    uint16_t buffer[4] = {0x4567, 0x6789, 0x3333, 0x4444};

    /*for (int pixel = 0; pixel < 320 *240 *2; pixel++)
    {
        display->a_buffer[pixel] = buffer[2];
    }*/

    ESP_LOGI(TAG, "adress p bufora %p", &display->a_buffer);
    ESP_LOGI(TAG, "adress k bufora %p", &display->a_buffer[240 * 320 * 2]);

    //display->drawFrame();
}