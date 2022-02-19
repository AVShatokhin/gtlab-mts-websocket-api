#include "ws_server.h"

ws_server::ws_server(quint16 port): QObject()
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
		qDebug() << "listen() = " << _server->listen(QHostAddress::Any, port);
		qDebug() << "serverPort() = " << _server->serverPort();
}

ws_server::~ws_server()
{
}