#include "cerver.h"

/*
	create 8 threads to accept requests. 
    NOTE: main function will kill this thread if there is not any client to connect. 
*/
void core(void *none) {
    DEBUG_LOG("start to create threads\n");
    int i;
    pthread_t theread_aqs;

    for (i = 0; i != 8; i++) {
        pthread_create(&theread_aqs, NULL, (void *) accept_request, NULL);
        // wait all threads exit (haven't got any clients)
        pthread_join(theread_aqs, NULL);
    }
    // true
    DEBUG_LOG("no clients found, fall asleep\n");
    core_is_sleeping = 1;
    pthread_exit(0);
}
