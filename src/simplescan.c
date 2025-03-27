#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <pthread.h>

extern int scan_devices();

void *scan_thread(void *arg) {
    int thread_id = *((int *)arg);
    printf("Hello from thread %d\n", thread_id);
    while (1) {
        printf("Scanning devices\n");
        scan_devices();
//        printf("Sleeping...........\n");
        sleep(60);
    }

    return NULL;
}

void *_thread(void *arg) {
    int thread_id = *((int *)arg);
    printf("Hello from thread %d\n", thread_id);
    while (1) {
        printf("Scanning devices\n");
        scan_devices();
//        printf("Sleeping...........\n");
        sleep(60);
    }

    return NULL;
}


int main(int argc, char **argv) {
    int num_threads = 1;
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, scan_thread, &thread_ids[i]);

    ++i;

    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
