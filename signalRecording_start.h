#pragma once

#include "request.h"
#include <QTimer>
#include "adc.h"
#include "visualQueue.h"

class signalRecording_start : public request
{
	Q_OBJECT

public:
	signalRecording_start(command, connection*, adc*);
	~signalRecording_start();
	void virtual start();	
	void virtual stop();
	int finish();
private: 
	adc* _adc;
	QList<visualQueue*> _listQueue;
	
};
