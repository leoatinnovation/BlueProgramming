#include "error_codes.h"
#include "bt_data.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bt_data_t bt_data_array_predefined[MAX_BT_DEVICES_PREDEFINED] = {
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, "Sara Nord 2T", 0, 0},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x01}, "Gautham", 0, 0},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x02}, "Saranya", 0, 0},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x03}, "Predefined Device 4", 0, 0},  
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x04}, "Predefined Device 5", 0, 0},  
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x05}, "Predefined Device 6", 0, 0},    
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x06}, "Predefined Device 7", 0, 0},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x07}, "Predefined Device 8", 0, 0},
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x08}, "Predefined Device 9", 0, 0},  
    {{0x00, 0x00, 0x00, 0x00, 0x00, 0x09}, "Predefined Device 10", 0, 0}    
};


int check_device(bt_data_t *bt_data_array, bt_data_t *bt_device){
    int errCode = ERROR_NO_DEVICE_FOUND;
    int i = 0;
    int j = 0;

    for(i = 0; i < MAX_BT_DEVICES_PREDEFINED; i++){
        for(j = 0; j < MAX_BT_DEVICES; j++){
            if((bt_data_array[j].addr == bt_data_array_predefined[i].addr)
                || (bt_data_array[j].name == bt_data_array_predefined[i].name)){
                errCode = ERROR_DEVICE_FOUND;
                memcpy(bt_device->addr, bt_data_array[j].addr, sizeof(bt_device->addr));
                strncpy((char *)bt_device->name, (char *)bt_data_array[j].name, sizeof(bt_device->name) - 1);
                bt_device->name[sizeof(bt_device->name) - 1] = '\0'; // Ensure null-termination
                goto done;
            }else{
                printf("Unknown device: %s\n", bt_data_array[j].name);
            }
        }
    }

done:
    return errCode;
}