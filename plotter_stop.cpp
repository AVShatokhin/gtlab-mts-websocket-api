#include "plotter_stop.h"

plotter_stop::plotter_stop(command cmd, connection* con, QMap<quint8, request*>* requests)
	: request(cmd, con), _requests(requests)
{
}

plotter_stop::~plotter_stop()
{
}

void plotter_stop::start()
{
	bool __found = false;

	for (auto i = _requests->constKeyValueBegin(); i != _requests->constKeyValueEnd(); i++) {
		plotter_start* __plotter_start = (plotter_start*)(i.base().value());
		command __cmd = __plotter_start->getCmd();
		if ((__cmd.methodId == "plotter.start") && (__cmd.plotterId == _cmd.plotterId)) {
			sendResponse(render::plotter_stop(_cmd.id, __plotter_start->finish()));
			__found = true;
			break;
		}
	}

	if (!__found) {
		// Ќе найден или уже остановлен, что как бы одно и то же
		sendResponse(render::plotter_stop(_cmd.id, PLOTTER_NOT_FOUND));
	}

	stop();
}
