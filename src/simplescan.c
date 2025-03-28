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
#include <stdbool.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// BT data array for discovered devices
bt_data_t bt_data_array[MAX_BT_DEVICES] = {0};

extern int scan_devices(bt_data_t *bt_data_array, int *num_devices);
extern bool cmp_device(bt_data_t *scan_bt_data, char *name);
int num_devices = 0;

void * scan_thread(void *arg) {
    uint8_t thread_id = *((uint8_t *)arg);
    uint8_t errCode=0U;

    while (1) {
        printf("Scanning devices\n");
        pthread_mutex_lock(&mutex);
        errCode = scan_devices(bt_data_array, &num_devices);
        if (errCode != ERROR_SUCCESS) {
            printf("Error scanning devices\n");
        }

        printf("Scanning devices done\n");
        if(num_devices == 0){
            pthread_mutex_unlock(&mutex);
            sleep(5);
            printf("Devices not found , rescan again\n");
        }else{
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
            sleep(10);
        }
    }

    return NULL;
}

void * check_devices(void *arg) {
    uint8_t thread_id = *((uint8_t *)arg);
    int errCode = 0U;
    uint8_t count = 0U;
    uint8_t i = 0;
    bool device_found = false;
    char device_name[MAX_NAME_LEN];

    memset(device_name, 0U, MAX_NAME_LEN);

    while (1) {
        pthread_mutex_lock(&mutex);
        if (num_devices < 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        for(i = 0; i < num_devices; i++){
            memset(device_name, 0U, sizeof(device_name));
            bool temp = cmp_device(&bt_data_array[i], device_name); 
            if(temp == true){
                printf("Device Found bt_device->name: %s\n", bt_data_array[i].name);
                printf("Device Found bt_device->addr: %s\n", bt_data_array[i].addr);
                for(i=0;i<3;++i){
                    char command[300]; // Buffer to hold the command
    //                sprintf(command, "espeak -a 200 \"%s\"", device_name);
                    sprintf(command, "flite -t \"%s\"", device_name);
                    system(command);
                    sleep(2);
                    system("mpg123 login_linux.mp3");
                }
            }
        }
        pthread_mutex_unlock(&mutex);
        printf("check_devices done\n");
        sleep(20);
    }

    return NULL;
}


int main(int argc, char **argv) {
    uint8_t num_threads = 2U;
    pthread_t threads[num_threads];
    uint8_t thread_ids[num_threads];

    thread_ids[0] = 0U;
    pthread_create(&threads[0], NULL, scan_thread, &thread_ids[0]);

    sleep(2);

    thread_ids[1] = 1U;
    pthread_create(&threads[1], NULL, check_devices, &thread_ids[1]);


    for (uint8_t i = 0U; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
