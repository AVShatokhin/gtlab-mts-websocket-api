#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "connection.h"
#include "types.h"

#include "render.h"

class parser : public QObject
{
	Q_OBJECT


public:
	parser(connection *);
	~parser();
	
	connection* getCurrentConnection() {
		return _currentConnection;
	}
	

private: 
	void parse(QString);
	connection* _currentConnection;

signals:
	void disconnected();
	void newCommand(command);
};
