#pragma once

#include <QWebSocket>

class connection : public QObject
{
	Q_OBJECT

public:
	connection(QWebSocket*);
	~connection();

	quint64 sendTextMessage(QString);

private:
	QWebSocket* _connection;

signals:
	void textMessageReceived(QString);
	void disconnected();

};
