#include "visualQueue.h"

visualQueue::visualQueue(gtl::analog_data* data, int size, int channelId)
	: QObject(), _data(data), _headPointer(0), _size(size), _channelId(channelId)
{
	QObject::connect(_data, &gtl::analog_data::data_changed, this, &visualQueue::_data_changed);

	_localData = new QList<qreal>;

	for (int i = 0; i < _size; i++) {
		// инициализация хранилки
		_localData->append(0);
	}


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
	QList<qreal> __temp = QList<qreal>();

	int __head = _headPointer;

	for (int i = 0; i < _localData->size(); i++) {
		__temp.append(_localData->at(__head));
		__head++;
		if (__head == _size) {
			__head = 0;
		}
	}

	return __temp;
}

void visualQueue::_data_changed()
{
	std::vector<qreal> data;
	_data->get_data(std::back_inserter(data));
	for (auto it = data.begin(); it != data.end(); it++) {
		_localData->replace(_headPointer, *it);
		_headPointer++;		
		if (_headPointer == _size) {
			_headPointer = 0;
		}
	}
}
