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

		while (!_requests.empty()) {
			_requests.begin().value()->stop();
		}

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
		// получение информации об аналоговых каналах АЦП
		return (request *)(new describeChannels(cmd, _parser->getCurrentConnection()));
	}
	else if (cmd.methodId == "signalRecording.start") {
		// начало записи сигнала с аналогового датчика		
		return (request*)(new signalRecording_start(cmd, _parser->getCurrentConnection()));
	} 
	else if (cmd.methodId == "signalRecording.stop") {
		// сигнал об окончании записи
		return (request*)(new signalRecording_stop(cmd, _parser->getCurrentConnection(), &_requests));				
	}
	else if (cmd.methodId == "plotter.start") {
		// инициализируем сессию рисования
		return (request*)(new plotter_start(cmd, _parser->getCurrentConnection()));
	}
	else if (cmd.methodId == "plotter.stop") {
		// закрываем сессию рисования
		return (request*)(new plotter_stop(cmd, _parser->getCurrentConnection(), &_requests));		
	}
	else if (cmd.methodId == "plotter.plot") {
		// закрываем сессию рисования
		return (request*)(new plotter_plot(cmd, _parser->getCurrentConnection(), &_requests));
	}
	else if (cmd.methodId == "plotter.selectStationaryIntervals") {
		// выбираем стационарные интервалы в записанном сигнале
		return (request*)(new plotter_selectStationaryIntervals(cmd, _parser->getCurrentConnection(), &_requests));
	}
	else if (cmd.methodId == "signalTransform") {
		// выполняем преобразование файла с сигналом
		return (request*)(new signalTransform(cmd, _parser->getCurrentConnection()));
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
