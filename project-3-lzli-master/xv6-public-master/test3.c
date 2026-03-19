#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
// #include "proc.h"
void io()
{
    for(int i = 0; i < 100; i++)
    {
        // printf(1,"a");
        sleep(1);
    }
    printf(1, "\n");
}



int main()
{
    int pid = fork();
    if (pid > 0)
    {
        wait();
        exit();
    }
    else
    {
        io();
        getpinfo(getpid());
        exit();
    }
}