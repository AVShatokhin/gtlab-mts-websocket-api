#pragma once

#include <QObject>

#include <iostream>

#include "hw/gtl_hw.h"
#include "hw/gtl_device.h"
#include "math/gtl_math_sum.h"

#include <QTimer>

#include "config.h"

class adc : public QObject
{
	Q_OBJECT

public:
	adc(config*, QObject *parent = nullptr);
	~adc();
	bool init();
	quint8 getADCStatus() { return _ADCStatus;  }
	ADC_state getState();

private:	
	config* _conf;
	quint8 _ADCStatus;	
	gtl::hw::device* _device;
};
