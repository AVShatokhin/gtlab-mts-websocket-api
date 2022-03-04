#include "parser.h"

parser::parser(connection * newConnection)
	: QObject(), _currentConnection(newConnection)
{
	QObject::connect(_currentConnection, &connection::textMessageReceived, [=](QString message) {
		parse(message);
		});

	QObject::connect(_currentConnection, &connection::disconnected, [=] {
		qDebug() << "Parser closed";
		emit disconnected();
		deleteLater();
		});
}

parser::~parser()
{
}

void parser::parse(QString message)
{
	command __newCommand;
	QJsonParseError __parse_errors;

	QJsonDocument __income = QJsonDocument::fromJson(message.toUtf8(), &__parse_errors);

	if (QJsonParseError::NoError != __parse_errors.error) {
		_currentConnection->sendTextMessage(render::ERROR_JSONParseFailed(__parse_errors.errorString()));
		return;
	}
	
	if (__income.isObject()) {
		QJsonObject __object = __income.object();
		if (__object.contains("id") && __object.contains("methodId")) {
			__newCommand.id = __object.value("id").toInt();
			__newCommand.methodId = __object.value("methodId").toString();
			emit newCommand(__newCommand);
			return;
		}
		else {
			_currentConnection->sendTextMessage(render::ERROR_notEnoughParameters());
		}
	}
	else {
		_currentConnection->sendTextMessage(render::ERROR_badRequestFormat());
	}	
}
