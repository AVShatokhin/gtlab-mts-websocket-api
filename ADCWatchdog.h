#pragma once

#include <QObject>
#include <QTimer>

#include "adc.h"

class ADCWatchdog : public QObject
{
	Q_OBJECT

public:
	ADCWatchdog(adc *);
	~ADCWatchdog();
};
