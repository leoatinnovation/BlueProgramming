#include <stdio.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "error_codes.h"
#include "bt_data.h"
#include "queueOps.h"


char device_name[MAX_NAME_LEN];

// BT data array for discovered devices
bt_data_t bt_data_array[MAX_BT_DEVICES] = {0};

// Conditional variable 
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Mutex variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int num_devices = 0U;

extern message_queue_t queue;

extern void *scan_thread(void *arg);
extern void *check_devices(void *arg);
extern void *play_music(void *arg);
extern void init_queue(message_queue_t *queue);

void * keyboard_input(void *arg) {
    uint8_t thread_id = *((uint8_t *)arg);
    int errCode = 0U;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    printf("\nEnter string below [ctrl + d] to quit\n");

    while ((read = getline(&line, &len, stdin)) != -1) {
        if (read > 0) {
            printf("\nRead %zd chars from stdin: %s", read, line);
        }
        printf("Enter string below [ctrl + d] to quit\n");
    }

    free(line);  // Free memory allocated by getline
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t threads[MAX_THREADS];
    uint8_t thread_ids[MAX_THREADS] = {0, 1, 2, 3};

    init_queue(&queue);

    pthread_create(&threads[0], NULL, scan_thread, &thread_ids[0]);
    sleep(2);
    pthread_create(&threads[1], NULL, check_devices, &queue);
    sleep(2);
    pthread_create(&threads[2], NULL, keyboard_input, &thread_ids[2]);
    sleep(2);
    pthread_create(&threads[2], NULL, play_music, &queue);    

    for (uint8_t i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
