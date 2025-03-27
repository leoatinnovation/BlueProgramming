#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "error_codes.h"
#include "bt_data.h"

int scan_devices(bt_data_t *bt_data_array, int *num_devices)
{
    int errCode = ERROR_SUCCESS;
    inquiry_info *hci_query_info = NULL;
    int max_rsp = 0, num_rsp = 0;
    int dev_id = 0, sock = 0, len = 0, flags = 0;
    int i = 0;
    char addr[19] = { 0 };
    char name[248] = { 0 };

    printf("Getting route \n");
    dev_id = hci_get_route(NULL);
    if (dev_id < 0) {
        printf("\n Error in getting route \n");
        errCode = ERROR_HCI_GET_ROUTE;
        goto done;
    }

    printf("Opening socket \n");
    sock = hci_open_dev( dev_id );
    if (dev_id < 0 || sock < 0) {
        printf("\n Error in opening socket \n");
        errCode = ERROR_SOCKET_OPENING;
        goto done;
    }

    len  = 8;
    max_rsp = 255;
    flags = IREQ_CACHE_FLUSH;
    printf("Allocating memory \n");
    hci_query_info = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    if (hci_query_info == NULL) {
        printf("\n Error in allocating memory \n");
        errCode = ERROR_MEMORY_ALLOCATION;
        goto done;
    }
    printf("Inquiry \n");

    num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &hci_query_info, flags);
    if( num_rsp < 0 ) {
        printf("\n Error in hci_inquiry \n");
        errCode = ERROR_HCI_ENQUIRY;
        goto done;
    }

    printf("num_rsp: %d\n", num_rsp);
    if(num_rsp > MAX_BT_DEVICES){
        printf("\n Error:Max devices reached\n");
        errCode = ERROR_MAX_DEVICES_REACHED;
        goto done;
    }

    printf("Reading remote name \n");
    for (i = 0; i < num_rsp; i++) {
        printf("Reading remote name %d\n", i);
        ba2str(&(hci_query_info+i)->bdaddr, addr);
        strncpy(bt_data_array[i].addr, addr, sizeof(bt_data_array[i].addr));
        printf("bddr: %s\n ", addr);
        printf("bt_data_array: %s\n ", bt_data_array[i].addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(hci_query_info+i)->bdaddr, (sizeof(name) - 1),name, 0) < 0){
            strcpy(name, "[unknown]");
        }
        printf("name: %s\n",name);
        strncpy(bt_data_array[i].name, name, sizeof(bt_data_array[i].name));
        printf("bt_data_array: %s\n ", bt_data_array[i].name);
        bt_data_array[i].name[sizeof(bt_data_array[i].name) - 1] = '\0'; // Ensure null-termination
        
    }
    *num_devices = num_rsp;

done:
    free( hci_query_info );
    close( sock );
    return errCode;
}
