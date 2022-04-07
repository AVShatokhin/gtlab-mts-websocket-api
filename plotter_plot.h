#pragma once

#include "request.h"
#include "plotter_start.h"

class plotter_plot : public request
{
	Q_OBJECT

public:
	plotter_plot(command cmd, connection* con, QMap<quint8, request*>* requests);
	~plotter_plot();
	void virtual start();
private: 	
	QMap<quint8, request*>* _requests;
};
