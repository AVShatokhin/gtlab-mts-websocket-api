#include "signalRecording_start.h"

signalRecording_start::signalRecording_start(command cmd, connection* con, adc * adc)
	: request(cmd, con), _adc(adc)
{
}

signalRecording_start::~signalRecording_start()
{
}

void signalRecording_start::start()
{
	ADC_state __state = _adc->getState();

	if (__state.deviceState > 0) {
		sendResponse(render::describeChannels(_cmd.id, __state));
		stop();
		return;
	}

	// какой длительностью будет промежуток устреднения ? ситаем его от visualIntervalMillis и visualFrameSize
	float __calculatePeriod = _cmd.visualIntervalMillis / _cmd.visualFrameSize;

	// сколько нужно собирать точек чтоб подготовить одно значение для визуализации ?		
	int __rmsPointsCount = int(__calculatePeriod * _adc->getConfig()->rate / 1000);

	qDebug() << "signalRecording_start";
	qDebug() << "_cmd.recordingId = " << _cmd.recordingId;
	qDebug() << "_cmd.visualIntervalMillis = " << _cmd.visualIntervalMillis;
	qDebug() << "_cmd.visualFrameSize = " << _cmd.visualFrameSize;
	qDebug() << "__calculatePeriod = " << __calculatePeriod;
	qDebug() << "__rmsPointsCount = " << __rmsPointsCount;

	for (int i = 0; i < _cmd.channels.size(); i++) {
		qDebug() << "_cmd.channels[" << i << "].channelId = " << _cmd.channels[i].channelId;
		qDebug() << "_cmd.channels[" << i << "].gainMultiplier = " << _cmd.channels[i].gainMultiplier;
		qDebug() << "_cmd.channels[" << i << "].recordingPath = " << _cmd.channels[i].recordingPath;
		qDebug() << "_cmd.channels[" << i << "].visualTransformType = " << _cmd.channels[i].visualTransformType;
		
		visualQueue* __queue = new visualQueue(
			_adc->getDevice()->ai(_cmd.channels[i].channelId),
			_cmd.visualFrameSize, 
			__rmsPointsCount,
			_cmd.channels[i].channelId
		);		
		
		_listQueue.push_back(__queue);

		QObject::connect(__queue, &visualQueue::ready, [=] {
			QString __sendData = render::signalRecording_start(_cmd.id, 0, &_listQueue);
			if (__sendData == QString("")) return;
			sendResponse(__sendData);
		});
		

		// TODO:
		// выкинуть ошибку если channelId за пределами доступного
		// выкинуть ошибку если АЦП недоступно
		// запись сигнала реализовать
		// коэффициент усиления учесть
		// задавать количество знаков после запятой, чтоб не было E в текстовом представлении числа qreal
	}
}

void signalRecording_start::stop()
{
	for (int i = 0; i < _listQueue.size(); i++) {
		_listQueue[i]->stop();
	}
	_listQueue.clear();

	emit requestCompleted();
	deleteLater();
}

int signalRecording_start::finish()
{
	// завершение работы по внешней команде
	stop();
	return 0;
}
