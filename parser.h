#pragma once

#include <QObject>
#include "connection.h"

class parser : public QObject
{
	Q_OBJECT

public:
	parser(connection *);
	~parser();
	void send();

private: 
	void parse(QString);
	connection* _connection;

signals:
	void disconnected();
	void newCommand();
};
