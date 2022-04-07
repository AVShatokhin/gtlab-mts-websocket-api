#include "signalRecording_start.h"

signalRecording_start::signalRecording_start(command cmd, connection* con)
	: request(cmd, con)
{
}

signalRecording_start::~signalRecording_start()
{
}

void signalRecording_start::start()
{
	// надо проверять на существование recorderId ?

	QTimer* __timer = new QTimer(this);
	__timer->setInterval(1000);
	__timer->setTimerType(Qt::PreciseTimer);

	QObject::connect(__timer, &QTimer::timeout, [=] {
		qDebug() << "111";
		});

	__timer->start();
}

int signalRecording_start::finish()
{
	// завершение работы по внешней команде

	stop();
	return 0;
}
