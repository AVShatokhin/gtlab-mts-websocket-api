#pragma once

#include "request.h"
#include "plotter_start.h"

class plotter_selectStationaryIntervals : public request
{
	Q_OBJECT

public:
	plotter_selectStationaryIntervals(command, connection*, QMap<quint8, request*>*);
	~plotter_selectStationaryIntervals();
	void virtual start();
private:
	QMap<quint8, request*>* _requests;
};
