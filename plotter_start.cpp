#include "plotter_start.h"

plotter_start::plotter_start(command cmd, connection* con)
	: request(cmd, con)
{
	 _timer = new QTimer(this);
}

plotter_start::~plotter_start()
{
}

void plotter_start::start()
{
	// ���� ��������� �� ������������� plotterId

	_timer->setInterval(50);
	_timer->setTimerType(Qt::PreciseTimer);

	QObject::connect(_timer, &QTimer::timeout, [=] {
		//qDebug() << "222";
	});

	_timer->start();	
}

int plotter_start::finish()
{
	// ���������� ������ �� ������� �������
	_timer->stop();
	_timer->deleteLater();	
	stop();
	return 0;
}
