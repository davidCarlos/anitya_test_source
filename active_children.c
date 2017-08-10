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


void main(){

    char entrada[100];

    struct timeval start, end;
    double times[2];
    gettimeofday(&start, NULL);

    char msg_part_active[BUFFER_SIZE];
    char msg_counter_active[BUFFER_SIZE];
    char complete_msg[BUFFER_SIZE];

    int msg_counter = 1;

	fd_set readfds;
	struct timeval tv;
	int stdin_descriptor;

	while(1){

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_ZERO(&readfds);
	FD_SET(0, &readfds);
    int stdin_descriptor = select(1, &readfds, NULL, NULL, &tv);
	if (stdin_descriptor == 1) {
	    read(STDIN_FILENO, entrada, BUFFER_SIZE);
		strcpy(msg_part_active, ": Mensagem ");
		sprintf(msg_counter_active, "%d", msg_counter);
		strcat(msg_part_active, msg_counter_active);
		strcat(msg_part_active, " do usuario: ");
		strcat(msg_part_active, "<");
		strcat(msg_part_active, entrada);
		strcat(msg_part_active, ">");
		gettimeofday(&end, NULL);
		get_timestamp(start, end, times);
		sprintf(complete_msg, "%.0lf:%.2lf", times[0], times[1]);
		strcat(complete_msg, msg_part_active);
		write(STDOUT_FILENO, complete_msg, BUFFER_SIZE);
        msg_counter++;
    }
	}
close(STDOUT_FILENO);

exit(0);
}
