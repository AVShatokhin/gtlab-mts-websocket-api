#include <QtCore/QCoreApplication>

#include "ws_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); 

	config* conf = new config("config.json");

    new ws_server(conf, new adc(conf));
    
    return a.exec();
}

