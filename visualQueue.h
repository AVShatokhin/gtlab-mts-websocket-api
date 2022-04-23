#pragma once

#include <QObject>
#include <QDebug>

#include "hw/gtl_analog_input.h"



class visualQueue : public QObject
{
	Q_OBJECT

public:
	visualQueue(gtl::analog_data*, int size, int channelId);
	~visualQueue();
	void stop();
	int get_channelId() { return _channelId;  }
	QList<qreal> get_data();

private slots:
	void _data_changed();

private:
	gtl::analog_data* _data;
	QList<qreal>* _localData;
	int _headPointer;
	int _size;
	int _channelId;
};
