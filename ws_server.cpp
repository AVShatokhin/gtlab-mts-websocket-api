#include "ws_server.h"

ws_server::ws_server(config * conf, adc * adc): QObject(), _conf(conf)
{
		QWebSocketServer * _server = new QWebSocketServer("ws_server", QWebSocketServer::SslMode::NonSecureMode, this);
		QObject::connect(_server, &QWebSocketServer::newConnection, [=]() 
			{
				new session(
					new parser(
						new connection(
							_server->nextPendingConnection()
						)
					),
					adc
				);

				qDebug() << "New connection";
			}
		);
		qDebug() << "listen() = " << _server->listen(QHostAddress::Any, conf->websocket_port);
		qDebug() << "serverPort() = " << _server->serverPort();
		//qDebug() << "adc->init() = " << adc->init();
}

ws_server::~ws_server()
{
}