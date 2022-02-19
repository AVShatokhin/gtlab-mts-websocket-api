#include "connection.h"

connection::connection(QWebSocket * webSocket)
	: QObject(), _connection(webSocket)
{
	QObject::connect(_connection, &QWebSocket::disconnected, [=] { 
		emit disconnected(); 
		deleteLater();
	});
	
	QObject::connect(_connection, &QWebSocket::textMessageReceived, [=](QString message) {
		emit textMessageReceived(message);
	});
}

connection::~connection()
{
}

quint64 connection::sendTextMessage(QString message)
{
	return _connection->sendTextMessage(message);
}
