#include "plotter_plot.h"

plotter_plot::plotter_plot(command cmd, connection* con, QMap<quint8, request*>* requests)
	: request(cmd, con), _requests(requests)
{
}

plotter_plot::~plotter_plot()
{
}

void plotter_plot::start()
{
	bool __found = false;

	for (auto i = _requests->constKeyValueBegin(); i != _requests->constKeyValueEnd(); i++) {
		plotter_start* __plotter_start = (plotter_start*)(i.base().value());
		command __cmd = __plotter_start->getCmd();
		if ((__cmd.methodId == "plotter.start") && (__cmd.plotterId == _cmd.plotterId)) {
			sendResponse(render::plotter_plot(_cmd.id, NO_ERROR));
			__found = true;
			break;
		}
	}

	if (!__found) {
		// Не найден plotter
		sendResponse(render::plotter_plot(_cmd.id, PLOTTER_NOT_FOUND));
	}
	
	// надо сделать поиск в уже существующем списке запросов с целью найти нужный plotterId
	stop();	
}
