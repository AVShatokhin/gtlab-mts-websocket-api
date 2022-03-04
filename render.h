#pragma once

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "types.h"

class render : public QObject
{
	Q_OBJECT

public:
	render(QObject* parent);
	~render();

	static QString Ping(quint8 id);
	static QString describeChannels(quint8 id, QList<channel>*);

	// Ошибки
	static QString ERROR_badRequest_ID(quint8 id);
	static QString ERROR_unknownMethod(quint8 id, QString methodId);
	static QString ERROR_notEnoughParameters();
	static QString ERROR_badRequestFormat();
	static QString ERROR_JSONParseFailed(QString);
};