#include "cerver.h"

/*
    accept a request. 
    this function default will send a 'hello, world' to client, you can edit you want. (you can makes a HTTP or FTP server you want).
*/

void accept_request(void *none) {
    // get a client-handle from struct LIST accept_list and handle it.
    int client_handle;
    for (;;) {
        if (accept_list.count == 0) pthread_exit(0);
        client_handle = accept_list.data[accept_list.next_read];
        accept_list.count--;
        client_handle++;
        if (client_handle == 0) {
            continue;
        }

        if (accept_list.next_read == 1024) {
            accept_list.next_read = 0;
        }
        char buffer[1024];
        read(client_handle, buffer, sizeof(buffer));
        fprintf(stderr, "%s", buffer);

        char return_buffer[] = "hello, world";
        write(client_handle, buffer, sizeof(buffer));
    }
}