#include "config.h"

config::config(QString fileName, QObject *parent)
	: QObject(parent), rate(DEFAULT_RATE), id(DEFAULT_ID), device(DEFAULT_DEVICE), websocket_port(DEFAULT_WEBSOCKET_PORT)
{
	#ifdef DEBUG
		QString __path = "d:/projects/gtlab-mts-websocket-api/";
	#else
		QString __path = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).front();
	#endif

	QFile __file(__path + fileName);

	if (!__file.open(QIODevice::ReadOnly)) {
		qDebug() << __file.errorString();
		qDebug() << __file.fileName();
		return;
	}

	QString __buffer = __file.readAll();
	__file.close();

	QJsonParseError __parse_errors;

	QJsonDocument __config = QJsonDocument::fromJson(__buffer.toUtf8(), &__parse_errors);
	
	if (__config.isObject()) {
		QJsonObject __object = __config.object();
		rate = __object.value("rate").toInt(DEFAULT_RATE);
		id = __object.value("id").toString(DEFAULT_ID);
		device = __object.value("device").toString(DEFAULT_DEVICE);
		hw_plugins_dir = __object.value("hw_plugins_dir").toString(HW_PLUGINS_DIR);
	}
	else {
	#ifdef DEBUG
		qDebug() << "Loading config error: " + __parse_errors.errorString();
	#endif // DEBUG
	}
	
	
}

config::~config()
{
}
