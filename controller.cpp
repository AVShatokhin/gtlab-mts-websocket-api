#include "controller.h"

controller::controller(QObject *parent)
	: QObject(parent)
{
}

controller::~controller()
{
}

void controller::registerConnection(connection* newConnection)
{
	_connections.push_back(newConnection);
	qDebug() << _connections.size();
}
