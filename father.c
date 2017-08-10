#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include "headers/father.h"

#define SHELL "/bin/sh"
#define EXIT_FAILIRE -1
#define BUFFER_SIZE 1000


void get_timestamp(struct timeval start, struct timeval end, double times[]){

    double cpuTimeUsed = ((end.tv_sec  - start.tv_sec) * 1000000u + end.tv_usec - start.tv_usec) / 1.e6;

    times[0] = (int) cpuTimeUsed/60;
    times[1] = (cpuTimeUsed - times[0] * 60);
}

void write_to_file(char text[])
{
    FILE *f = fopen("output.txt", "a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    fprintf(f, text);
    fprintf(f, "\n");
    fclose(f);
}

int create_children(pid_t sleepy_children_pid, pid_t active_children_pid)
{
    struct timeval start, end;
    double times[2];

    char complete_msg_sleepy[BUFFER_SIZE];
    char complete_msg_active[BUFFER_SIZE];

    int sleepy_children[2];
    pipe(sleepy_children);
    int active_children[2];
    pipe(active_children);

    sleepy_children_pid = fork();
    active_children_pid = fork();

        if ((active_children_pid == -1) && (sleepy_children_pid == -1))
        {
            printf("Error making process! \n");
            return(-1);
        }

            /* Nesse ponto eu to no processo filho */
            if(!(sleepy_children_pid)){
                close(sleepy_children[0]);
                dup2(sleepy_children[1], STDOUT_FILENO);
                execve("./sleepy_children", NULL, NULL);
            }

          if(!active_children_pid){
              close(active_children[0]);
              dup2(active_children[1], STDOUT_FILENO);
              execve("./active_children", NULL, NULL);
          }

            else{
                time_t begin, final;
                double seconds;
                time(&begin);

                    close(sleepy_children[1]);
                    close(active_children[1]);
                    dup2(active_children[0], STDIN_FILENO);
                    dup2(sleepy_children[0], STDIN_FILENO);
                while(1){
                    char readbuffer_sleepy[BUFFER_SIZE];
                    char readbuffer_active[BUFFER_SIZE];

                    time(&final);
                    seconds = difftime(final, begin);
                    gettimeofday(&start, NULL);


                    read(sleepy_children[0], readbuffer_sleepy, BUFFER_SIZE);

                    fd_set fds;
                    struct timeval timeout;
                    timeout.tv_sec = 2;
                    timeout.tv_usec = 0;
                    FD_ZERO(&fds);
                    FD_SET(active_children[0], &fds);

                    int stdin_descriptor = select(active_children[0]+1, &fds, NULL, NULL, &timeout);
                   if(stdin_descriptor == 1)
                       read(active_children[0], readbuffer_active, BUFFER_SIZE);

                    gettimeofday(&end, NULL);

                    get_timestamp(start, end, times);
                    sprintf(complete_msg_sleepy, "%.0lf:%.2lf: ", times[0], times[1]);
                    strcat(complete_msg_sleepy, readbuffer_sleepy);

                    get_timestamp(start, end, times);
                    sprintf(complete_msg_active, "%.0lf:%.2lf: ", times[0], times[1]);
                    strcat(complete_msg_active, readbuffer_active);

                    write_to_file(complete_msg_sleepy);

                   if(stdin_descriptor == 1)
                        write_to_file(complete_msg_active);

                   if(seconds >= 30.0)
                       break;

                }
            }
}
