#include <QtCore/QCoreApplication>
#include "ws_server.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    new ws_server(8888);
    
    return a.exec();
}
