#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "error_codes.h"

int scan_devices()
{
    int errCode = 0;
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
    printf("Reading remote name \n");
    for (i = 0; i < num_rsp; i++) {
        ba2str(&(hci_query_info+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name(sock, &(hci_query_info+i)->bdaddr, sizeof(name), 
            name, 0) < 0)
        strcpy(name, "[unknown]");
        printf("%s  %s\n", addr, name);
    }

done:
    free( hci_query_info );
    close( sock );
    return errCode;
}
