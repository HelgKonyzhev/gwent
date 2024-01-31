#include <QCoreApplication>
#include "server.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;
    const auto started = server.serve();

    if (!started)
    {
        qDebug() << "Start up failed";
        return -1;
    }

    return a.exec();
}
