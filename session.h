#pragma once

#include <QObject>
#include "types.h"

#include "parser.h"
#include "PingRequest.h"
#include "describeChannels.h"
#include "signalRecording_start.h"
#include "signalRecording_stop.h"
#include "plotter_start.h"
#include "plotter_stop.h"
#include "plotter_plot.h"
#include "plotter_selectStationaryIntervals.h"
#include "signalTransform.h"

class session : public QObject
{
	Q_OBJECT

public:
	session(parser *);
	~session();
private:
	request * _router(command);
	parser * _parser;	
	QMap<quint8, request*> _requests;
	bool _checkRequestId(command);
	void _addRequest(command, request *);
};
