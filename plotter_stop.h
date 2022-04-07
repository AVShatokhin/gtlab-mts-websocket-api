#pragma once

#include "request.h"
#include "plotter_start.h"

class plotter_stop : public request
{
	Q_OBJECT

public:
	plotter_stop(command cmd, connection* con, QMap<quint8, request*>* requests);
	~plotter_stop();
	void virtual start();
private:
	QMap<quint8, request*>* _requests;
};
