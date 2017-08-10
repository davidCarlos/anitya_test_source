#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "headers/father.h"


int main(int argc, char *argv[])
{
    pid_t sleepy_children;
    pid_t active_children;


    create_children(sleepy_children, active_children);

    return 0;
}
