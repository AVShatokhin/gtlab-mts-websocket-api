#include <QtCore/QCoreApplication>

#include "ws_server.h"
#include "ADCWatchdog.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv); 

	config* conf = new config("config.json");
    adc* _adc = new adc(conf);
    
    new ws_server(conf, _adc);
    new ADCWatchdog(_adc);
    
    return a.exec();
}

