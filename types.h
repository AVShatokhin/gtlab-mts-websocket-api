#pragma once
#include <QObject>

struct command {
	quint8 id;
	QString methodId;
};

struct channel {
	quint32 channelId;
	QString displayName;
	quint32 samplingRate;
};

//struct response {
//	quint8 id;	
//	QString methodId;
//	bool next;
//	QString code;
//	QString extra;
//};

