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

QString render::describeChannels(quint8 id, QList<channelInfo> * channels)
{

	QJsonArray __channels;

	for (int i = 0; i != channels->size(); i++) {
		QJsonObject __channel;
		__channel.insert("channelId", QJsonValue((int)channels->at(i).channelId));
		__channels << __channel;
	}

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"id", id},
		{"result", __channels }
	});

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



QString render::ERROR_badRequest_ID(quint8 id)
{
	QJsonObject __extra;

	__extra.insert("id", QJsonValue((int)id));
	__extra.insert("descr", QJsonValue("ID already exists"));

	QJsonObject __error;

	__error.insert("code", QJsonValue(1000));
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

	__error.insert("code", QJsonValue(1000));
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
	__error.insert("code", QJsonValue(1000));

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
	__error.insert("code", QJsonValue(1000));

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
	__error.insert("code", QJsonValue(1000));

	QJsonDocument __response = QJsonDocument();

	__response.setObject({
		{"error", __error }
	});

	return __response.toJson();
}

