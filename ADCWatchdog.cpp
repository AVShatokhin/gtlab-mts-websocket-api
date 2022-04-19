#include "ADCWatchdog.h"

ADCWatchdog::ADCWatchdog(adc * adc)
	: QObject()
{
	QTimer* __timer = new QTimer(this);
	__timer->setInterval(1000);
	__timer->setTimerType(Qt::PreciseTimer);

	QObject::connect(__timer, &QTimer::timeout, [=] {
		quint8 __ADCstatus = adc->getADCStatus();
		if ((__ADCstatus == ADC_NOT_INITED) or
			(__ADCstatus == ADC_CRITICAL_FAULT) or
			(__ADCstatus == ADC_START_FAILED)) {
			qDebug() << "adc->init() = " << adc->init();
		}
		
	});

	__timer->start();
}

ADCWatchdog::~ADCWatchdog()
{
}
