#include "adc.h"

adc::adc(config* conf, QObject* parent)
	: QObject(parent), _conf(conf)
{	
	_ADCStatus = ADC_OK;

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
	if (_device->start(_conf->id, _conf->rate)) {		
		return true;
	}
	else {
		_ADCStatus = ADC_START_FAILED;
		return false;
	}
	
}

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
//    //������� ������ hardware. �������� - ���� � ����� � ���������.
//    gtl::hw::hw hw("../../.output/.hwplugins/debug");
//
//    //�������� ������ ��������� ���������.
//    QStringList devices = hw.devices();
//
//    qDebug() << devices;
//
//    //������� ����������. �������� ��� ����������.
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
//        //������ ��������� �������.
//        d->ai(0)->set_iepe(true);
//        d->ai(0)->set_sensitivity(0.001);
//
//        //��������� ����������. ������ �������� - id, ������ - ������� �������������
//        qDebug() << d->start("0", 128000);
//
//        //� ���������� ����� ����� ��������, �������������� ��� ���������� ��������� �������, ��������������� ������. ���, ������� � �.�.
//        //��� ������, ����������� ����� ����������� ���������� ������. ��� �������.
//        //�������� - ����� ���������� ������. � ������ ������ ������� ����� ����������.
//        sum = new gtl::math::sum(d->ai(3));
//
//        //���� ��������� ����� ������.
//        QObject::connect(d, &gtl::hw::device::received_data, [=]() {qDebug() << "data recieved" << sum->value(); });
//    }
//
//    return a.exec();
//
//}
