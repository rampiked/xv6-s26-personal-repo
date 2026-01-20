#include "types.h"
#include "user.h"

typedef int pid_t;

int main(int argc, char *argv[])
{
    int fdParent[2];
    int fdChild[2];
    char pingBuffer[5];
    char pongBuffer[5];

    if (pipe(fdParent) < 0) {  //Create pipe.
        printf(2, "Error, parent pipe failed.");
        exit();
    }

    if (pipe(fdChild) < 0) {  //Create pipe.
        printf(2, "Error, child pipe failed.");
        exit();
    }

    pid_t pid = fork();
    if (pid == 0) {
        //Child process.
        close(fdParent[1]);    // child never writes to parent→child pipe
        close(fdChild[0]);     // child never reads from child→parent pipe

        read(fdParent[0], pingBuffer, 4);
        pingBuffer[4] = 0;
        printf(1, "%d: received %s\n", getpid(), pingBuffer);
        write(fdChild[1], "pong", 4);
        close(fdChild[1]);
        exit();
    } else {
        //Parent process.
        close(fdParent[0]);    // parent never reads from parent→child pipe
        close(fdChild[1]);     // parent never writes to child→parent pipe


        write(fdParent[1], "ping", 4);
        read(fdChild[0], pongBuffer, 4);
        pongBuffer[4] = 0;
        printf(1, "%d: received %s\n", getpid(), pongBuffer);
        close(fdParent[1]);
        wait();
    }
  exit();
}