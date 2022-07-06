#include "visualQueue.h"

visualQueue::visualQueue(gtl::analog_data* data, int pointsCount, int rmsPointsCount, int channelId)
	: QObject(), 
	_data(data), _pointsCount(pointsCount), _channelId(channelId), _rmsPointsCount(rmsPointsCount),
	_currentSumm(0), _currentRmsPointsCount(0), _localData(new QList<qreal>), _isReady(false)
{
	QObject::connect(_data, &gtl::analog_data::data_changed, this, &visualQueue::_data_changed);
}

visualQueue::~visualQueue()
{	
}

void visualQueue::stop()
{
	QObject::disconnect(_data, &gtl::analog_data::data_changed, this, &visualQueue::_data_changed);
	deleteLater();
}

QList<qreal> visualQueue::get_data()
{
	QList<qreal> __temp = *_localData;
	_localData->clear();
	_isReady = false;
	return __temp;
}

void visualQueue::_calcIncome(qreal data)
{
	_currentSumm += data;
	_currentRmsPointsCount++;

	if (_currentRmsPointsCount == _rmsPointsCount) {
		qreal __finalValue = _currentSumm / _rmsPointsCount;
		_localData->push_back(__finalValue);
		_currentSumm = 0;
		_currentRmsPointsCount = 0;		
	}

	if (_localData->size() == _pointsCount) {
		_isReady = true;
		emit ready();
	}
}

void visualQueue::_data_changed()
{
	std::vector<qreal> data;
	_data->get_data(std::back_inserter(data));
	
	for (auto it = data.begin(); it != data.end(); it++) {
		_calcIncome(*it);
	}	
}
