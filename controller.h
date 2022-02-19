#pragma once

#include <QObject>

#include "connection.h"

class controller : public QObject
{
	Q_OBJECT

public:
	controller(QObject *parent = nullptr);
	~controller();

	void registerConnection(connection *);

private:
	QList<connection*> _connections;

};
