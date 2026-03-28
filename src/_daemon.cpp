#include "daemon.hpp"
#include <alloca.h>
#include <cstdlib>
#include <fstream>
#include <csignal>
#include <sched.h>
#include <sys/syslog.h>

#include <iostream>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

volatile sig_atomic_t g_run_flag = 1;

void signal_handler(int signum)
{
    if(signum == SIGTERM)
    {
        syslog(LOG_INFO, "Recieved SIGTERM, shutting down");
        g_run_flag = 0;
    }
    else if (signum == SIGHUP)
    {
        syslog(LOG_INFO, "Recieved SIGHUP, reread config");
        g_run_flag = 0;

    }
}

void demonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        std::cerr << "First fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    
    //Code in child process:
    //Make new session
    if(setsid() < 0)
    {
        std::cerr << "setsid failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Extra fork()
    pid = fork();
    if (pid < 0)
    {
        std::cerr << "Second fork failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    if (chdir("/") < 0)
    {
        std::cerr << "chdir() failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    umask(0);

    //Close all descriptors
    int max_fd = sysconf(_SC_OPEN_MAX);
    if (max_fd == -1) max_fd = 1024;
    for (int fd = 0; fd < max_fd; fd++)
    {
        close(fd);
    }

    //Redirect std in out err streams in dev/null
    int dev_null = open("/dev/null", O_RDWR);
    if (dev_null != -1)
    {
        dup2(dev_null, STDIN_FILENO);
        dup2(dev_null, STDOUT_FILENO);
        dup2(dev_null, STDERR_FILENO);
        if (dev_null > STDERR_FILENO) close(dev_null);    
    }
}

void init_daemon()
{

}

void run_daemon()
{
    int counter = 0;
    while (g_run_flag)
    {
        syslog(LOG_INFO, "Daemon is running, counter = %d", counter++);
        sleep(10);
    }
    syslog(LOG_INFO, "Daemon stopped");
}



















