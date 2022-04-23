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

	QTimer* __timer = new QTimer(this);
	__timer->setInterval(_cmd.visualIntervalMillis);
	__timer->setTimerType(Qt::PreciseTimer);

	

	qDebug() << "signalRecording_start";
	qDebug() << "_cmd.recordingId = " << _cmd.recordingId;
	qDebug() << "_cmd.visualIntervalMillis = " << _cmd.visualIntervalMillis;
	qDebug() << "_cmd.visualFrameSize = " << _cmd.visualFrameSize;
	for (int i = 0; i < _cmd.channels.size(); i++) {
		qDebug() << "_cmd.channels[" << i << "].channelId = " << _cmd.channels[i].channelId;
		qDebug() << "_cmd.channels[" << i << "].gainMultiplier = " << _cmd.channels[i].gainMultiplier;
		qDebug() << "_cmd.channels[" << i << "].recordingPath = " << _cmd.channels[i].recordingPath;
		qDebug() << "_cmd.channels[" << i << "].visualTransformType = " << _cmd.channels[i].visualTransformType;
		_listQueue.push_back(new visualQueue(_adc->getDevice()->ai(_cmd.channels[i].channelId), _cmd.visualFrameSize, _cmd.channels[i].channelId));
		// TODO:
		// выкинуть ошибку если channelId за пределами доступного
		// выкинуть ошибку если АЦП недоступно
		// запись сигнала реализовать
		// коэффициент усиления учесть
		// задавать количество знаков после запятой, чтоб не было E в текстовом представлении числа qreal
	}

	QObject::connect(__timer, &QTimer::timeout, [=] {
		sendResponse(render::signalRecording_start(_cmd.id, 0, &_listQueue));
	});

	__timer->start();
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
