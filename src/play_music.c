/*
April 7 - need Handle multiple request for the device found

*/
#include "error_codes.h"
#include "bt_data.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "queueOps.h"

static int play_sound(char *name){
    for(uint8_t i=0U; i<2; i++){
        char command[300]; // Buffer to hold the command
//        sprintf(command, "espeak -a 200 \"%s\"", device_name);
        sprintf(command, "flite -t \"%s\"", device_name);
        system(command);
        system("mpg123 login_linux.mp3");
    }
    return 0;
}

void * play_music(void *arg){
    message_queue_t *queue = (message_queue_t *)arg;

    while (1) {
        printf("play_music dequeue.........\n");
        bt_data_t *item = dequeue(queue);
        play_sound(item->name);
        printf("play_music done\n");
        sleep(2);
    }
    return NULL;
}