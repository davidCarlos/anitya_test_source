#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define SHELL "/bin/sh"
#define EXIT_FAILIRE -1
#define BUFFER_SIZE 1000

void get_timestamp(struct timeval start, struct timeval end, double times[]){

    double cpuTimeUsed = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;

    times[0] = (int) cpuTimeUsed/60;
    times[1] = (cpuTimeUsed - times[0] * 60);
}

int main(int argc, char *argv[])
{
        char msg_part[BUFFER_SIZE];
        char readbuffer_sleepy[BUFFER_SIZE];
        char msg_counter[3];
        int time_to_sleep;

        struct timeval start, end;
        double times[2];
        gettimeofday(&start, NULL);

        char complete_msg[BUFFER_SIZE];

        int msg = 1;
        while(1){
            strcpy(msg_part, ": Mensagem ");
            sprintf(msg_counter, "%d", msg);
            strcat(msg_part, msg_counter);
            strcat(msg_part, " do filho dorminhoco");
            gettimeofday(&end, NULL);

            get_timestamp(start, end, times);
            sprintf(complete_msg, "%.0lf:%.2lf", times[0], times[1]);
            strcat(complete_msg, msg_part);

            write(1, complete_msg, BUFFER_SIZE);
            msg++;

            time_to_sleep = 0 + (rand() % 2);
            sleep(time_to_sleep);
        }
    exit(0);
}
