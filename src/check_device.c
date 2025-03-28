#include "error_codes.h"
#include "bt_data.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    char addr[19];
    char name[248];
} addr_table;

addr_table predefined_devices[MAX_BT_DEVICES_PREDEFINED] = {
    {"30:BB:7D:7A:32:EA", "OnePlus Nord 2T 5G"},
    {"D4:CB:CC:91:E1:AE", "Gautham"}
};

bool cmp_device(bt_data_t *scan_bt_data){
    bool device_found = false;
    uint8_t j=0;

    for(j = 0; j < MAX_BT_DEVICES_PREDEFINED; j++){
        if (strcmp(predefined_devices[j].addr, scan_bt_data->addr) == 0) {
            device_found = true;
        }
    }

    return device_found;
}