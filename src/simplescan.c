#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <pthread.h>
#include "error_codes.h"
#include "bt_data.h"

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// BT data array for discovered devices
bt_data_t bt_data_array[MAX_BT_DEVICES] = {0};

extern int scan_devices(bt_data_t *bt_data_array, int *num_devices);
extern int check_device(bt_data_t *bt_data_array, bt_data_t *bt_device);

void * scan_thread(void *arg) {
    uint8_t thread_id = *((uint8_t *)arg);
    uint8_t errCode=0U;
    int num_devices = 0;

    printf("Hello from thread %d\n", thread_id);
    while (1) {
        printf("Scanning devices\n");
//        pthread_mutex_lock(&mutex);
        errCode = scan_devices(bt_data_array, &num_devices);
        if (errCode != ERROR_SUCCESS) {
            printf("Error scanning devices\n");
        }
//        pthread_cond_signal(&cond);
//        pthread_mutex_unlock(&mutex);
        printf("Scanning devices done\n");
        if(num_devices == 0){
            sleep(10);
            printf("Devices not found , rescan again\n");
        }else{
            sleep(60);
        }
    }

    return NULL;
}

void * check_devices(void *arg) {
    int thread_id = *((int *)arg);
    uint8_t errCode = 0U;
    bt_data_t *bt_device = (bt_data_t *)malloc(sizeof(bt_data_t));

    printf("Hello from thread %d\n", thread_id);
    while (1) {
        printf("checking devices\n");
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        errCode = check_device(bt_data_array, bt_device);
        if (errCode != ERROR_SUCCESS) {
            printf("Error checking devices\n");
        }

        printf("Device Found bt_device->name: %s\n", bt_device->name);
        printf("Device Found bt_device->addr: %s\n", bt_device->addr);

        pthread_mutex_unlock(&mutex);
        printf("check_devices done\n");
        sleep(60);
    }

    return NULL;
}


int main(int argc, char **argv) {
    uint8_t num_threads = 1U;
    pthread_t threads[num_threads];
    uint8_t thread_ids[num_threads];

    thread_ids[0] = 0;
    pthread_create(&threads[0], NULL, scan_thread, &thread_ids[0]);

    sleep(5);

    //thread_ids[1] = 1;
    //pthread_create(&threads[1], NULL, check_devices, &thread_ids[1]);


    for (uint8_t i = 0U; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
