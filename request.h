#pragma once

#include <QObject>

#include "types.h"
#include "connection.h"
#include "render.h"
#include "adc.h"


class request : public QObject
{
	Q_OBJECT

public:
	request(command cmd, connection * con);
	~request();
	command _cmd;
	connection* _connection;

	command getCmd() { return _cmd; }

public slots:
	virtual void start();
	virtual void stop();
	void sendResponse(QString);

signals:
	void requestCompleted();
};
