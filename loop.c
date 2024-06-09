#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

void printHello() {
    printf("hello world\n");
    exit(0);
}

int main() {
    pid_t child_pid;
    struct user_regs_struct regs;

    // Create a child process
    child_pid = fork();

    if (child_pid == 0) {
        // Child process executes code
        printf("Child process started\n");
        while (1);
        // sleep(10);
        printf("Child process finished\n");
    } else {
        // Wait for the child process
        sleep(1);

        // Parent process traces the child process
        ptrace(PTRACE_ATTACH, child_pid, NULL, NULL);
        waitpid(child_pid, NULL, 0);

        // Get the address of the printHello function
        long printHelloAddr = (long)printHello;

        // Get the registers state of the child process
        ptrace(PTRACE_GETREGS, child_pid, NULL, &regs);

        // Set the rip register to the address of the printHello function
        regs.rip = printHelloAddr;
        // regs.rip = printHelloAddr + 2;
        ptrace(PTRACE_SETREGS, child_pid, NULL, &regs);

        // Detach ptrace and continue the execution of the child process
        ptrace(PTRACE_DETACH, child_pid, NULL, NULL);
    }

    return 0;
}
