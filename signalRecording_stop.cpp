#include "signalRecording_stop.h"

signalRecording_stop::signalRecording_stop(command cmd, connection* con, QMap<quint8, request*>* requests)
	: request(cmd, con), _requests(requests)
{
}


signalRecording_stop::~signalRecording_stop()
{
}

void signalRecording_stop::start()
{	
	bool __found = false;

	for (auto i = _requests->constKeyValueBegin(); i != _requests->constKeyValueEnd(); i++) {
		signalRecording_start* __signalRecording_start = (signalRecording_start *)(i.base().value());
		command __cmd = __signalRecording_start->getCmd();
		
		qDebug() << "signalRecording_stop: " << __cmd.id;

		if ((__cmd.methodId == "signalRecording.start") && (__cmd.recordingId == _cmd.recordingId)) {			
			sendResponse(render::signalRecording_stop(_cmd.id, __signalRecording_start->finish()));
			__found = true;
			break;
		}
	}

	if (!__found) {
		// Ќе найден или уже остановлен, что как бы одно и то же
		sendResponse(render::signalRecording_stop(_cmd.id, RECORDER_NOT_FOUND));
	}

	

	stop();
}
