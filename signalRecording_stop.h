#pragma once

#include "signalRecording_start.h"

class signalRecording_stop : public request
{
	Q_OBJECT

public:
	signalRecording_stop(command, connection*, QMap<quint8, request*> *);
	~signalRecording_stop();
	void virtual start();

private:	
	QMap<quint8, request*> * _requests;

};
