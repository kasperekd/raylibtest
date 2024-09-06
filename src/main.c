// gcc src/main.c -o my_program -Ilib/include -Llib/lib -Wl,-rpath=./lib/lib
// -lraylib -lm -lpthread -ldl -lrt

#include <pthread.h>

#include "myRay.h"

int main(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, win_main, NULL);

    pthread_joid(thread);
    return 0;
}