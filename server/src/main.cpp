#include <QCoreApplication>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>
#include <csignal>
#include <unistd.h>

#include <server.h>



void sigHandler(int signum)
{
    if (signum == SIGTERM)
    {
        QCoreApplication::quit();
    }
    if(signum == SIGHUP)
    {
         QCoreApplication::quit();
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Start daemon
       pid_t pid,sid;
       pid = fork();
       if (pid < 0) {
           exit(EXIT_FAILURE);
       }
       if (pid > 0) {
           exit(EXIT_SUCCESS);
       }
       umask(0);
       sid = setsid();
       if (sid < 0) {
          exit(EXIT_FAILURE);
       }
//       if ((chdir("/")) < 0) {
//          exit(EXIT_FAILURE);
//       }

       signal(SIGTERM,sigHandler);
       signal(SIGHUP,sigHandler);

       //start server
       server serv(QHostAddress::Any,5555);

    return a.exec();
}
