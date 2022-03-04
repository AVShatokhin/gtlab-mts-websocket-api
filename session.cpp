#include "session.h"

session::session(parser * par)
	: QObject(), _parser(par)
{
	_requests.clear();

	QObject::connect(par, &parser::newCommand, [=] (command cmd) {			
			if (_checkRequestId(cmd)) {				
				_addRequest(cmd, _router(cmd));
			}
			else {
				_parser->getCurrentConnection()->sendTextMessage(render::ERROR_badRequest_ID(cmd.id));				
			}
		});

	QObject::connect(par, &parser::disconnected, [=] {
		qDebug() << "Session closed";
		deleteLater();
		});
}

session::~session()
{
}

request * session::_router(command cmd)
{
	if (cmd.methodId == "ping") {
		return (request *)(new PingRequest(cmd, _parser->getCurrentConnection()));
	}
	else if (cmd.methodId == "signalRecording.describeChannels") {
		// ��������� ���������� �� ���������� ������� ���
		return (request *)(new describeChannels(cmd, _parser->getCurrentConnection()));
	}
	else if (cmd.methodId == "signalRecording.start") {
		// ������ ������ ������� � ����������� �������

	} 
	else if (cmd.methodId == "signalRecording.stop") {
		// ������ �� ��������� ������

	}
	else if (cmd.methodId == "plotter.start") {
		// �������������� ������ ���������

	}
	else if (cmd.methodId == "plotter.stop") {
		// ��������� ������ ���������

	}
	else if (cmd.methodId == "plotter.plot") {
		// ��������� ������ ���������

	}
	else if (cmd.methodId == "plotter.selectStationaryIntervals") {
		// �������� ������������ ��������� � ���������� �������

	}
	else if (cmd.methodId == "signalTransform") {
		// ��������� �������������� ����� � ��������

	}
	
	return new request(cmd, _parser->getCurrentConnection());	
}

bool session::_checkRequestId(command cmd)
{
	return !_requests.contains(cmd.id);
}

void session::_addRequest(command cmd, request * req)
{
	_requests.insert(cmd.id, req);
	QObject::connect(req, &request::requestCompleted, [=] {
		_requests.remove(cmd.id);		
	});
	req->start();
}
