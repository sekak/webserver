#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>

using namespace std;

int main() {
    int pipefd[2];
    pid_t pid;
    char message[] = "Hello, child!";

    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        cout << "Child process\n";
        close(pipefd[0]); // Close read end of pipe

        // Redirect stdout to the write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            return 1;
        }

        // Close the original write end of the pipe
        close(pipefd[1]);

        // Execute PHP script
        char *const args[] = {NULL};
        char *const env[] = {const_cast<char*>("index.php"), NULL};
        execve("/usr/bin/php", args, env);

        // If execve fails
        perror("execve");
        return 1;
    } else {
        // Parent process
        cout << "Parent process\n";
        close(pipefd[1]); // Close write end of pipe

        char buffer[256];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));

        if (bytesRead == -1) {
            perror("read");
            return 1;
        } else if (bytesRead == 0) {
            cout << "No data received from child process\n";
        } else {
            // Null-terminate the buffer
            buffer[bytesRead] = '\0';
            cout << "Received message from child: " << buffer << endl;
        }

        close(pipefd[0]); // Close read end of pipe

        // Wait for the child process to terminate
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }

    }

    return 0;
}
