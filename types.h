#pragma once
#include <QObject>

struct recording {
	quint8 transformId;
	QString transformParams; // TBD
	QString recordingPath;
};

struct channelRecord {
	quint32 channelId;
	double gainMultiplier;
	QList<recording> recordings;
};

struct command {
	quint8 id;
	QString methodId;

	// signalRecording.start
	quint8 recordingId;
	quint16 measurementsFrame;
	quint16 frameIntervalMillis;
	QList<channelRecord> channels;

	// plotter.start
	quint8 plotterId;
	quint32 rangeStart;
	quint32 rangeStop;

	// signal transform
	QString transformId;
	QString sourcePath;
	QString destinationPath;

};

struct ADC_state {
	QString deviceType;
	quint8 deviceState;		
	quint8 channelsCount;
	quint32 samplingRate;
};



//struct response {
//	quint8 id;	
//	QString methodId;
//	bool next;
//	QString code;
//	QString extra;
//};

