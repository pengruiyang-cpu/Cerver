#include "cerver.h"

/*
    accept a request. 
    this function default will send a 'hello, world' to client, you can edit you want. (you can makes a HTTP or FTP server you want).
*/

void accept_request(int *no) {
    DEBUG_LOG_AR("wake-up by core-thread\n", *no);
    // get a client-handle from struct LIST accept_list and handle it.
    int client_handle;
    for (;;) {
        if (accept_list.count == 0) {
            DEBUG_LOG_AR("no client found, fall asleep\n", *no);
            pthread_exit(0);
        }
        client_handle = accept_list.data[accept_list.next_read];
        accept_list.count--;
        accept_list.free++;
        DEBUG_LOG_AR("get a client from accept list\n", *no);
        if (client_handle == 0) {
            continue;
        }

        if (accept_list.next_read == LIST_COUNT) {
            accept_list.next_read = 0;
        }
        char buffer[1024];
        read(client_handle, buffer, sizeof(buffer));
        fprintf(stderr, "%s", buffer);

        char return_buffer[] = "hello, world";
        // '-1' means no ''\0'
        if (write(client_handle, return_buffer, sizeof(return_buffer) - 1) == 0) {
            printf("EMERGENCY: write to client failed\n");
        }
        close(client_handle);
        DEBUG_LOG_AR("accepted a client done\n", *no);
    }
}
