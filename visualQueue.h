#pragma once

#include <QObject>
#include <QDebug>

#include "hw/gtl_analog_input.h"



class visualQueue : public QObject
{
	Q_OBJECT

public:
	visualQueue(gtl::analog_data*, int size, int rmsPointCount, int channelId);
	~visualQueue();
	void stop();
	int get_channelId() { return _channelId;  }
	QList<qreal> get_data();
	bool isReady() { return _isReady;  }

signals:
	void ready();

private slots:
	void _data_changed();

private:
	gtl::analog_data* _data;
	QList<qreal>* _localData;
	void _calcIncome(qreal);	
	int _channelId;

	int _pointsCount;
	qreal _currentSumm;
	int _currentRmsPointsCount;	
	int _rmsPointsCount;

	bool _isReady;
};
