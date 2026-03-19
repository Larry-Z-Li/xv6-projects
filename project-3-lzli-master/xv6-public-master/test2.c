#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
// #include "proc.h"
// #include <unistd.h>     // also defines NULL

void io(int num)
{
    for (int i = 0; i < 400; i++)
    {
        printf(1, "a");
    }
    printf(1, "\n");
}

int main()
{
    int pid;
    pid = fork();

    if (pid > 0)
    {
        for (int i = 0; i < 10; i++)
        {
            pid = fork();
            if (pid == 0)
            {
                io(getpid());
                getpinfo(getpid());
                exit();
            }
            else if (pid < 0)
            {
                printf(1, "Fork failed\n\n");
            }
        }
        sleep(400);
        exit();
    }

}
