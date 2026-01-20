#include "types.h"
#include "user.h"

typedef int pid_t;

void print_uint64(uint64 n) {
    char buf[21]; // max 20 digits + null terminator
    int i = 20;
    buf[i--] = 0;

    if (n == 0) {
        buf[i--] = '0';
    } else {
        while (n > 0) {
            buf[i--] = '0' + (n % 10);
            n /= 10;
        }
    }

    printf(1, "%s", &buf[i+1]);
}


int main(int argc, char *argv[])
{
if(argc <= 1){
    printf(2, "Error, missing argument.\n");
    exit();
}
//Obtains initial cycle count.
unsigned int low;
unsigned int high;
unsigned int aux;
asm volatile(
  "rdtscp"
  : "=a"(low), "=d"(high), "=c"(aux)
  :
  :
);
uint64 start_cycles = ((uint64)high << 32) | low;

//Executes program.
pid_t pid = fork();
if (pid == 0) {
    exec(argv[1], argv + 1);
} else {
    // parent process
    wait();
}

//Obtain end cycle count.
asm volatile(
  "rdtscp"
  : "=a"(low), "=d"(high), "=c"(aux)
  :
  :
);
uint64 end_cycles = ((uint64)high << 32) | low;

uint64 total_cycles = end_cycles - start_cycles;

printf(1, "%s took ", argv[1]);
print_uint64(total_cycles);
printf(1, " cycles to run.\n");

  exit();
}