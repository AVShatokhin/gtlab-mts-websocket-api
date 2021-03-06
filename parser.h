#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>

#include "connection.h"
#include "const.h"

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
	void parse_signalRecordingStart(command *, QJsonObject *);
	void parse_signalRecordingStop(command*, QJsonObject*);
	void parse_plotterStart(command*, QJsonObject*);
	void parse_signalTransform(command*, QJsonObject*);

	connection* _currentConnection;

signals:
	void disconnected();
	void newCommand(command);
};
