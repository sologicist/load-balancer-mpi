#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../implem/struct.h"
#include <pthread.h>

pthread_mutex_t	mutex;
void *ecriture(){
    int i = 0;
    while (i<20){
        pthread_mutex_lock(&mutex);
        workers[5] = i;
        printf("thread %d\n", i);
        pthread_mutex_unlock(&mutex);
        i++;
    }
} 

int main(int argc, char* argv[]) {
    pthread_mutex_init(&mutex, NULL);
	pthread_t thread;
    pthread_create(&thread, NULL, ecriture, NULL);
    int c;
    int i=0;
    while(i<20){
            pthread_mutex_lock(&mutex);
            c = workers[5];
            printf("%d\n", workers[5]);
            pthread_mutex_unlock(&mutex);
            
            i++;
    }
}
