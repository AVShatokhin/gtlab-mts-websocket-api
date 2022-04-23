#include "adc.h"

adc::adc(config* conf, QObject* parent)
	: QObject(parent), _conf(conf)
{	
	_ADCStatus = ADC_NOT_INITED;

	gtl::hw::hw * __hw = new gtl::hw::hw(_conf->hw_plugins_dir);

	QStringList __devices = __hw->devices();
	
	if (__devices.contains(conf->device, Qt::CaseSensitivity::CaseInsensitive)) {
		_device = __hw->create_device(conf->device);
		if (_device == NULL)
		{
			qDebug() << "error creating device";
			_ADCStatus = ADC_ERROR_CREATING_DEVICE;
		}
	} else {
		_ADCStatus = ADC_PLUGIN_NOT_FOUND;
	}
}

adc::~adc()
{
}

bool adc::init()
{	
	for (auto i = 0; i != _device->count_ai(); i++) {
		_device->ai(i)->set_iepe(true);
		_device->ai(i)->set_sensitivity(1);
	}

	if (_device->start(_conf->id, _conf->rate)) {
		_connStatusChanged = QObject::connect(_device, &gtl::hw::device::status_changed, [=](int state) {
			// если АЦП отключено сигнал приходит раз в секунду
			qDebug() << "Device status changed: " << state;

			if ((state == 0) or (state == 3)) { // undef | failed
				// todo:
				// Выкидывать сигнал при сбое АЦП и принимать его в потоковых методах, работающих непосредственно с АЦП.
				// хотя это не обязательно, им достаточно контроллировать переменную state!

				_ADCStatus = ADC_CRITICAL_FAULT;
				QObject::disconnect(_connStatusChanged);
			}
		});

		_ADCStatus = ADC_OK;

		return true;
	}
	else {
		_ADCStatus = ADC_START_FAILED;		
		return false;
	}
}

ADC_state adc::getState()
{
	ADC_state __state;	

	__state.deviceState = _ADCStatus;
	__state.deviceType = _conf->device;
	__state.samplingRate = 0;
	__state.channelsCount = 0;

	if (_ADCStatus == ADC_OK) {
		__state.samplingRate = _device->rate();
		__state.channelsCount = _device->count_ai();
	}

	return __state;
}



//void adc::_longInit()
//{
//	QTimer* __timer = new QTimer(this);
//	__timer->setInterval(1000);
//	__timer->setSingleShot(true);
//
//	QObject::connect(__timer, &QTimer::timeout, [=] {
//		qDebug() << "adc->init() = " << this->init();
//	});
//
//	__timer->start();
//}

/*
QStringList devices = hw.devices();
qDebug() << devices;

gtl::hw::device* d = hw.create_device("d001");

if (d == NULL)
{
	qDebug() << "error creating device";

}
else {
	qDebug() << "OK";
}

qDebug() << d->count_ai();*/
//qDebug() << d->start("0", 128000);




//
//#include <QCoreApplication>
//#include <QObject>
//
//#include <iostream>
//
//#include "hw/gtl_hw.h"
//#include "hw/gtl_device.h"
//#include "math/gtl_math_sum.h"
//
//
//int main(int argc, char* argv[])
//{
//    QCoreApplication a(argc, argv);
//
//    //создаем объект hardware. параметр - путь к папке с плагинами.
//    gtl::hw::hw hw("../../.output/.hwplugins/debug");
//
//    //получаем список доступных устройств.
//    QStringList devices = hw.devices();
//
//    qDebug() << devices;
//
//    //создаем устройство. параметр имя устройства.
//    gtl::hw::device* d = hw.create_device("d001");
//
//    if (d == NULL)
//    {
//        qDebug() << "error creating device";
//    }
//    else
//    {
//        gtl::math::sum* sum;
//
//
//        //запускаем устройство. первый параметр - id, второй - частота дискретизации
//        qDebug() << d->start("0", 128000);
//
//        //в библиотеке будет класс объектов, предназначеных для вычисления скалярных величин, характеризующих сигнал. скз, частота и т.п.
//        //это объект, вычисляющий сумму поступающих аналоговых данных. для примера.
//        //параметр - канал аналоговых данных. в данном случае входной канал устройства.
//        sum = new gtl::math::sum(d->ai(3));
//
//        //слот получения новых данных.
//        QObject::connect(d, &gtl::hw::device::received_data, [=]() {qDebug() << "data recieved" << sum->value(); });
//    }
//
//    return a.exec();
//
//}
