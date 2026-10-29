#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipe1[2], pipe2[2], pipe3[2];

    if (pipe(pipe1) == -1) {
        perror("Error creating pipe1");
        return 1;
    }
    
    if (pipe(pipe2) == -1) {
        perror("Error creating pipe2");
        return 1;
    }
    
    if (pipe(pipe3) == -1) {
        perror("Error creating pipe3");
        return 1;
    }

    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Error fork1");
        return 1;
    }
    
    if (pid1 == 0) {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        
        execl("./child1", "./child1", NULL);
        perror("Error exec1");
        exit(1);
    }

    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Error fork 2");
        return 1;
    }

    if (pid2 == 0) {
        dup2(pipe2[0], 0);
        dup2(pipe3[1], 1);
        
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);
        
        execl("./child2", "./child2", NULL);
        perror("Error exec2");
        exit(1);
    }


    pipe(pipe1); pipe(pipe2); pipe(pipe3);

    if (fork() == 0) {
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);
        
        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);

        execl("./child1", "./child1", NULL);
        exit(1);
    }

    if (fork() == 0) {
        dup2(pipe2[0], 0);
        dup2(pipe3[1], 1);

        close(pipe1[0]); close(pipe1[1]);
        close(pipe2[0]); close(pipe2[1]);
        close(pipe3[0]); close(pipe3[1]);

        execl("./child2", "./child2", NULL);
        exit(1);
    }

    close(pipe1[0]); 
    close(pipe2[0]); close(pipe2[1]); 
    close(pipe3[1]);

    printf("Write text:\n");
    char buffer[1024];

    while (fgets(buffer, 1024, stdin) != NULL) {
        write(pipe1[1], buffer, strlen(buffer));

        int n = read(pipe3[0], buffer, 1024 - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("Itog: %s", buffer);
        }
    }

    close(pipe1[1]); close(pipe3[0]);
    wait(NULL); wait(NULL);
    return 0;
}
