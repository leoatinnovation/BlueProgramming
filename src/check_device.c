#include "error_codes.h"
#include "bt_data.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "queueOps.h"

typedef struct {
    char addr[19];
    char name[248];
} addr_table;

addr_table predefined_devices[MAX_BT_DEVICES_PREDEFINED] = {
//    {"30:BB:7D:7A:32:EA", "Saravanan"},
    {"30:BB:7D:7A:32:EB", "Saravanan"},
    {"D4:CB:CC:91:E1:AE", "Gautham"},
    {"E0:DD:C0:E0:BB:3E", "Sara VIVO"}
};

static bool cmp_device(bt_data_t *scan_bt_data, char *name){
    bool device_found = false;
    uint8_t j=0;

    for(j = 0; j < MAX_BT_DEVICES_PREDEFINED; j++){
        if (strcmp(predefined_devices[j].addr, scan_bt_data->addr) == 0) {
            device_found = true;
            strncpy(name, predefined_devices[j].name, (sizeof(name)-1));
            name[sizeof(name) - 1] = '\0'; // Ensure null-termination
        }
    }

    return device_found;
}

void * check_devices(void *arg) {
    message_queue_t *queue = (message_queue_t *)arg;
    int errCode = 0U;
    uint8_t count = 0U;
    uint8_t i = 0;
    bool temp = false;


    while (1) {
        pthread_mutex_lock(&mutex);
        if (num_devices <= 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        for(i = 0; i < num_devices; i++){
            memset(device_name, 0U, sizeof(device_name));
            temp = cmp_device(&bt_data_array[i], device_name); 
            if(temp == true){
                printf("check_devices enqueue.........\n");
                enqueue(queue, &bt_data_array[i]);
            }
        }
        pthread_mutex_unlock(&mutex);
        printf("check_devices done\n");
        sleep(20);
    }

    return NULL;
}

