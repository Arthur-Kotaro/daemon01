#pragma once

#include <csignal>
#include <cstdlib>
#include <sys/syslog.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <functional>
#include <sys/types.h>
#include <sys/stat.h>

class Daemon
  {
      using SignalHandler = std::function<void()>;
  
      volatile sig_atomic_t running;
      volatile sig_atomic_t reloadFlag;
      SignalHandler reloadCallback;
      SignalHandler termCallback;
      
      static Daemon& instance();
      
      void handleSigTerm();
      void handleSigHup();
      
      public:
      Daemon() : running(1), reloadFlag(0) {}
      bool static daemonize();
      void setupSignals();
      void setReloadCallback(const SignalHandler& callback);
      void setTermCallback(const SignalHandler& callback);
      void run(const SignalHandler& workFunc);
      void stop();
  };
