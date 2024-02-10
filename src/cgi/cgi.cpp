#include "cgi.hpp"

Cgi::Cgi() {}

Cgi::~Cgi() {}

string Cgi::execute_cgi(Location *location, Config *conf, int fd, string url)
{
    int pipes[2];
    pid_t pid;
    string path;
    string content;

    if (pipe(pipes) == -1)
        perror("pipes");
    if (url.find(".php") != string::npos)
        path = "/usr/bin/php";
    else
        path = "/usr/bin/python";
    pid = fork();
    if (pid == -1)
        perror("fork");
    else if (pid == 0)
    {
        // cout << url << "  " << path <<endl;
        close(pipes[0]);
        dup2(pipes[1], 1);
        execl(path.c_str(), path.c_str(), url.c_str(), NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(-1, &status, 0);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 1)
        {
            return "error";
        }
        close(pipes[1]);
        dup2(pipes[0], 0);
        char buf[BUFFER_SIZE];
        ssize_t bytesRead;
        while ((bytesRead = read(pipes[0], buf, BUFFER_SIZE - 1)) > 0)
        {
            content.append(buf, bytesRead);
        }
        close(pipes[1]);
        close(pipes[0]);
    }
    return content;
}