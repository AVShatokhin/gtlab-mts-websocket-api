#include "render.h"

render::render(QObject *parent)
	: QObject(parent)
{
}

render::~render()
{
}

QString render::Ping(quint8 id)
{
	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"id", id},
		{"result", QJsonObject()}
	});

	return __response.toJson();
}

QString render::describeChannels(quint8 id, ADC_state state)
{
	QJsonDocument __response = QJsonDocument();

	if (state.deviceState > 0) {
		QJsonObject __error = {
			{ "code", ADC_FAIL },
			{ "extra", "Failed ADC state: " + QString::number(state.deviceState)},
		};

		__response.setObject({
			{"id", id},
			{"error", __error }
		});
	}
	else {
		QJsonObject __result = {			
			{ "samplingRate", (int)(state.samplingRate)},
			{ "deviceType", state.deviceType },
			{ "channelsCount", state.channelsCount},
		};

		__response.setObject({
			{"id", id},
			{"result", __result }
			});
	}

	return __response.toJson();
}

QString render::signalRecording_start(quint8 id, int errorCode, QList<visualQueue*>* _listQueue)
{
	// TODO:
	// добавить нормальный рендер ошибки

	QJsonObject __result = QJsonObject();
	
	for (auto it = _listQueue->begin(); it != _listQueue->end(); it++) {
		if (((visualQueue*)(*it))->isReady() == false) return QString("");
	}
		
	for (auto it = _listQueue->begin(); it != _listQueue->end(); it++) {
		QList<qreal> __dataTosend = ((visualQueue*)(*it))->get_data();

		//qDebug() << "Data to send: " << __dataTosend.size() << " values";
		
		QJsonArray __data = QJsonArray();
		
		for (auto it = __dataTosend.begin(); it != __dataTosend.end(); it++) {
			//qDebug() << "value = " << *it;
			__data.append(QString::number(* it, (char)103, 10));
		}

		__result.insert(QString::number(((visualQueue*)(*it))->get_channelId()), __data);
	}

	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	}
	else {
		__response.setObject({
			{"id", id},
			{"result", __result}
			});
	}

	return __response.toJson();	
}

QString render::signalRecording_stop(quint8 id, int errorCode)
{
	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	} else {
		__response.setObject({
			{"id", id},
			{"result", QJsonObject()}
			});
	}

	return __response.toJson();
}

QString render::plotter_stop(quint8 id, int errorCode)
{
	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	}
	else {
		__response.setObject({
			{"id", id},
			{"result", QJsonObject()}
			});
	}

	return __response.toJson();
}

QString render::plotter_plot(quint8 id, int errorCode)
{
	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	}
	else {
		__response.setObject({
			{"id", id},
			{"result", QJsonObject()}
			});
	}

	return __response.toJson();
}

QString render::plotter_selectStationaryIntervals(quint8 id, int errorCode)
{
	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	}
	else {
		__response.setObject({
			{"id", id},
			{"result", QJsonObject()}
			});
	}

	return __response.toJson();
}

QString render::signalTransform(quint8 id, int errorCode)
{
	QJsonDocument __response = QJsonDocument();

	if (errorCode > 0) {
		QJsonObject __error = { { "code", errorCode} };

		__response.setObject({
			{"id", id},
			{"result", QJsonObject()},
			{"error", __error}
			});

	}
	else {
		__response.setObject({
			{"id", id},
			{"result", QJsonObject()}
			});
	}

	return __response.toJson();
}



QString render::ERROR_badRequest_ID(quint8 id)
{
	QJsonObject __extra;

	__extra.insert("id", QJsonValue((int)id));
	__extra.insert("descr", QJsonValue("ID already exists"));

	QJsonObject __error;

	__error.insert("code", QJsonValue(DUPLICATE_REQUEST_ID));
	__error.insert("extra", __extra);

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"id", id},
		{"error", __error }
	});
	
	return __response.toJson();
}

QString render::ERROR_unknownMethod(quint8 id, QString methodId)
{
	QJsonObject __extra;

	__extra.insert("methodId", QJsonValue(methodId)); 
	__extra.insert("descr", QJsonValue("Unknown method"));

	QJsonObject __error;

	__error.insert("code", QJsonValue(UNKNOWN_METHOD));
	__error.insert("extra", __extra);

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"id", id},
		{"error", __error }
		});

	return __response.toJson();
}

QString render::ERROR_notEnoughParameters()
{
	QJsonObject __extra;

	__extra.insert("descr", QJsonValue("Not enough parameters"));

	QJsonObject __error;

	__error.insert("extra", __extra);
	__error.insert("code", QJsonValue(NOT_ENOUGH_PARAMETERS));

	QJsonDocument __response = QJsonDocument();

	__response.setObject({		
		{"error", __error }
		});

	return __response.toJson();

}

QString render::ERROR_badRequestFormat()
{
	QJsonObject __extra;

	__extra.insert("descr", QJsonValue("Bad request format"));

	QJsonObject __error;

	__error.insert("extra", __extra);
	__error.insert("code", QJsonValue(BAD_REQUEST_FORMAT));

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"error", __error }
		});

	return __response.toJson();
}

QString render::ERROR_JSONParseFailed(QString error)
{
	QJsonObject __extra;

	__extra.insert("descr", QJsonValue("JSON PARSER: " + error));

	QJsonObject __error;

	__error.insert("extra", __extra);
	__error.insert("code", QJsonValue(PARSER_FAILED));

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"error", __error }
	});

	return __response.toJson();
}


