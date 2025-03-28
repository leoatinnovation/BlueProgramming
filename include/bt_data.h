#ifndef __BT_DATA_H__
#define __BT_DATA_H__

#include <stdint.h>

#define MAX_BT_DEVICES 32
#define MAX_BT_DEVICES_PREDEFINED 2

typedef struct bt_data {
    char addr[19];
    char name[248];
    uint8_t rssi;
    uint8_t flags;
    
} bt_data_t;

#endif
