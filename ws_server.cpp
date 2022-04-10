#include "ws_server.h"

ws_server::ws_server(config * conf, adc * adc): QObject(), _conf(conf), _adc(adc)
{
		QWebSocketServer * _server = new QWebSocketServer("ws_server", QWebSocketServer::SslMode::NonSecureMode, this);
		QObject::connect(_server, &QWebSocketServer::newConnection, [=]() 
			{
				new session(
					new parser(
						new connection(
							_server->nextPendingConnection()
						)
					)
				);

				qDebug() << "New connection";
			}
		);
		qDebug() << "listen() = " << _server->listen(QHostAddress::Any, conf->websocket_port);
		qDebug() << "serverPort() = " << _server->serverPort();
		qDebug() << "adc->init() = " << _adc->init();
}

ws_server::~ws_server()
{
}