#pragma once

#include "request.h"
#include <QTimer>

class signalRecording_start : public request
{
	Q_OBJECT

public:
	signalRecording_start(command, connection*);
	~signalRecording_start();
	void virtual start();	
	int finish();
};
