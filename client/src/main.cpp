#include <QCoreApplication>
#include <QDebug>
#include <QFile>

#include <client.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString arg1(argv[1]);
    if(argc < 2 || argc > 2) {
        qInfo() << "Unknown key. Enter the --help key for opening help";
        return 0;
    } else
    {
        if(arg1 == QLatin1String("--help"))
        {
            qInfo() << "Enter the name file to send to server";
            return 0;
        }

        QFile file(arg1);
        if(file.open(QIODevice::ReadOnly)) {
            client *cl = new client("127.0.0.1",5555, arg1);
        } else {
            qInfo() << "Error. File not open";
            return 0;
        }
    }

    return a.exec();
}
