#include "daemon.hpp"
#include "config.hpp"

#include <cstdlib>
#include <memory>
#include <sys/syslog.h>
//#include <thread>
//#include <chrono>
//#include <iostream>
//#include <iterator>




std::unique_ptr<Config> g_config;




void reloadConfig()
{
    if (g_config)
        g_config->reload();
    else
        syslog(LOG_ERR, "Config object is not initialized");
}




void doWork()
{
    if(!g_config) return;
    int interval = g_config->getInt("Interval", 10);
    std::string message = g_config->get("message", "Default message");
    syslog(LOG_INFO, "Working %s (interval=%d)", message.c_str(), interval);
    sleep(interval);
}




int main(int argc, char* argv[])
{
    std::string config_file = (argc == 2) ? argv[1] : DEFAULT_CONFIG_PATH;

    openlog("mydaemon", LOG_PID | LOG_CONS, LOG_DAEMON);

    g_config = std::make_unique<Config>();
    if (!g_config->load(config_file))
    {
        syslog(LOG_ERR, "Failed to load config, exiting");
        closelog();
        return EXIT_FAILURE;
    }

    Daemon daemon;
    daemon.setReloadCallback(reloadConfig);
    daemon.setTermCallback([](){
            syslog(LOG_INFO, "Termination callback");});
    //Daemonization
    if (!daemon.daemonize())
    {
        syslog(LOG_ERR, "Daemonization failed");
        closelog();
        return EXIT_FAILURE;
    }
    daemon.setupSignals();
    daemon.run(doWork);
    closelog();
    return EXIT_SUCCESS;
}
