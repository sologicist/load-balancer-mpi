#include <stdio.h>
#include <stdlib.h>
#include <time.h>

time_t timer;

void publish_load(int delay){
    time_t current_time = time(NULL);
    if (difftime(current_time, timer) >= delay){
        printf("It's time\n");
        timer = current_time;
    }
}


int main(int argc, char** argv){
    timer = time(NULL);

    while(1){
        publish_load(5);
    }
}