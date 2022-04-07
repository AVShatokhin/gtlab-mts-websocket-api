#pragma once

#include "request.h"
#include <QTimer>

class plotter_start : public request
{
	Q_OBJECT

public:
	plotter_start(command cmd, connection* con);
	~plotter_start();
	void virtual start();
	int finish();
private: 
	QTimer * _timer;
};
