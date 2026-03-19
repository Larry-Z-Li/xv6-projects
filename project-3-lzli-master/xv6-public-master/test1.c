#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

void io()
{
    for(int i = 0; i < 30; i++)
    {
        printf(1,"a");
    }
}

void cpu()
{
    unsigned long long x = 2;
    for (unsigned long long i = 1; i < 1000000000; i++)
    {
        if(i > 5000000)
        {
            x = x * 9/10;
        }
        else
        {
            x = x * 10/9 + 1;
        }
    }
    
}
int main()
{

    int pid = fork();
    if (pid > 0)
    {
        io();
        wait();
        getpinfo(getpid());
        exit();
    }
    else
    {
        io();
        getpinfo(getpid());
        exit();
    }
}