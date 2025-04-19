#ifndef __BT_DATA_H__
#define __BT_DATA_H__

#include <stdint.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct message_queue message_queue_t;

#define MAX_BT_DEVICES 32
#define MAX_BT_DEVICES_PREDEFINED 3
#define MAX_NAME_LEN   200

#define MAX_THREADS 4

typedef struct bt_data {
    char addr[19];
    char name[248];
    uint8_t rssi;
    uint8_t flags;
    
} bt_data_t;

extern char device_name[MAX_NAME_LEN];

// BT data array for discovered devices
extern  bt_data_t bt_data_array[MAX_BT_DEVICES];

// Conditional variable 
extern pthread_cond_t cond;
extern pthread_cond_t play_cond;

// Mutex variable
extern pthread_mutex_t mutex;
extern pthread_mutex_t play_mutex;

extern int num_devices;
extern bool device_found;
extern message_queue_t queue;

#endif
