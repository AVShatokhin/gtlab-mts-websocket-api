#include "plotter_selectStationaryIntervals.h"

plotter_selectStationaryIntervals::plotter_selectStationaryIntervals(command cmd, connection* con, QMap<quint8, request*>* requests)
	: request(cmd, con), _requests(requests)
{
}

plotter_selectStationaryIntervals::~plotter_selectStationaryIntervals()
{
}

void plotter_selectStationaryIntervals::start()
{
	bool __found = false;

	for (auto i = _requests->constKeyValueBegin(); i != _requests->constKeyValueEnd(); i++) {
		plotter_start* __plotter_start = (plotter_start*)(i.base().value());
		command __cmd = __plotter_start->getCmd();
		if ((__cmd.methodId == "plotter.start") && (__cmd.plotterId == _cmd.plotterId)) {
			sendResponse(render::plotter_selectStationaryIntervals(_cmd.id, NO_ERROR));
			__found = true;
			break;
		}
	}

	if (!__found) {
		// �� ������ plotter
		sendResponse(render::plotter_selectStationaryIntervals(_cmd.id, PLOTTER_NOT_FOUND));
	}

	// ���� ������� ����� � ��� ������������ ������ �������� � ����� ����� ������ plotterId
	stop();
}
