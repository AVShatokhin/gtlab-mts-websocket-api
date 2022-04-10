#pragma once

#include "const.h"

#include <QObject>
#include <QStandardPaths>
#include <QFile>
#include <QJSonDocument>
#include <QJsonObject>

class config : public QObject
{
	Q_OBJECT

public:
	config(QString fileName, QObject *parent = nullptr);
	~config();
	int rate;
	QString id;
	QString device;
	QString hw_plugins_dir;
	int websocket_port;
};
