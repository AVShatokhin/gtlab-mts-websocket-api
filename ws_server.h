#pragma once

#include <QObject>
#include <QWebSocketServer>

#include "session.h"

class ws_server : public QObject
{
	Q_OBJECT

public:
	ws_server(config *, adc *);
	~ws_server();
private:
	config* _conf;

};
