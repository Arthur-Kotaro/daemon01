#include <daemon.hpp>

bool Daemon::daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "First fork failed");
        return false;
    }
    if(pid > 0) exit(EXIT_SUCCESS);
    
    if(setsid() < 0)
    {
        syslog(LOG_ERR, "setsid failed");
        return false;
    }

    pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "Second fork failed");
        return false;
    }
    if(pid > 0) exit(EXIT_SUCCESS);
   
    if(chdir("/") < 0)
    {
        syslog(LOG_ERR, "chdird failed");
        return false;
    }
    
    umask(0);
    
    //Close all files descriptors
    int maxFD = sysconf(_SC_OPEN_MAX);
    if(maxFD == -1) maxFD = 1024;
    for (int fd = 0; fd < maxFD; ++fd) close(fd);

    //Redirect std in out err streams in dev/null
    int dev_null = open("/dev/null", O_RDWR);
    if (dev_null != -1)
    {
        dup2(dev_null, STDIN_FILENO);
        dup2(dev_null, STDOUT_FILENO);
        dup2(dev_null, STDERR_FILENO);
        if (dev_null > STDERR_FILENO) close(dev_null);
    }

    return true;
}

void Daemon::setupSignals()
{
    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    //SIGTERM
    sa.sa_handler = [](int) { instance().handleSigTerm();};
    sigaction(SIGTERM, &sa, nullptr);
    
    //SIGHUP
    sa.sa_handler = [](int) { instance().handleSigHup();};
    sigaction(SIGHUP, &sa, nullptr);

    //Ignore SIGINT (Ctrl+C) и SIGPIPE
    signal(SIGINT, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}

//Configs reload callback registration
void Daemon::setReloadCallback(const SignalHandler& callback)
{
    reloadCallback = callback;
}

//Termination callback registration
void Daemon::setTermCallback(const SignalHandler& callback)
{
    termCallback = callback;
}

//Main loop for users code executing
void Daemon::run(const SignalHandler& workFunc)
{
    syslog(LOG_INFO, "Daemon started, entering main loop");
    while (running)
    {
        //Handling of the reload flag
        if(reloadFlag && reloadCallback)
        {
            reloadFlag = false;
            reloadCallback();
        }


//***************************
//EXECUTE USER'S CODE       *
//                          *
        workFunc();
//                          *
//EXECUTE USER'S CODE OVER  *
//***************************


    }
    syslog(LOG_INFO, "Daemon stopped");
}

//Make Singleton
Daemon& Daemon::instance()
{
    static Daemon inst;
    return inst;
}

//Called by SIGTERM
void Daemon::stop()
{
    running = false;
}

void Daemon::handleSigTerm()
{
    syslog(LOG_INFO, "Recieved SIGTERM, shutting down");
    if (termCallback) termCallback();
    running = false;
}

void Daemon::handleSigHup()
{
    syslog(LOG_INFO, "Recieved SIGHUP, will reload config");
    reloadFlag = true;
}
