#include "parser.h"

parser::parser(connection * newConnection)
	: QObject(), _connection(newConnection)
{
	QObject::connect(_connection, &connection::textMessageReceived, [=](QString message) {
		qDebug() << message;
		parse(message);
		});

	QObject::connect(_connection, &connection::disconnected, [=] {
		qDebug() << "Disconnected";
		emit disconnected();
		deleteLater();
		});
}

parser::~parser()
{
}

void parser::send()
{
	_connection->sendTextMessage("!!!");
}

void parser::parse(QString)
{
	emit newCommand();
}
