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

			QJsonObject __properties = __object.value("properties").toObject();
			if (!__properties.isEmpty()) {
				parse_signalRecordingStart(&__newCommand, &__properties);
				parse_signalRecordingStop(&__newCommand, &__properties);
				parse_plotterStart(&__newCommand, &__properties);
				parse_signalTransform(&__newCommand, &__properties);
			}

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

void parser::parse_signalRecordingStart(command* newCommand, QJsonObject* properties)
{
	newCommand->recordingId = properties->value("recordingId").toInt(0);
	newCommand->measurementsFrame = properties->value("measurementsFrame").toInt(0);
	newCommand->frameIntervalMillis = properties->value("frameIntervalMillis").toInt(0);
	if (properties->value("channels").isArray()) {
		QJsonArray __channels = properties->value("channels").toArray();
		for (auto i = __channels.begin(); i != __channels.end(); i++) {
			if (i->isObject()) {
				channelRecord __channel;
				QJsonObject __channelItem = i->toObject();
				__channel.channelId = __channelItem.value("channelId").toInt();
				__channel.gainMultiplier = __channelItem.value("gainMultiplier").toDouble();
				if (__channelItem.value("recordings").isArray()) {
					QJsonArray __recordings = __channelItem.value("recordings").toArray();
					for (auto j = __recordings.begin(); j != __recordings.end(); j++) {
						recording __recording;
						QJsonObject __recordingItem = j->toObject();
						__recording.recordingPath = __recordingItem.value("recordingPath").toString();
						__recording.transformId = __recordingItem.value("transformId").toInt();
						__channel.recordings.append(__recording);
					}
				}
				newCommand->channels.append(__channel);
			}
		}
	}
}

void parser::parse_signalRecordingStop(command* newCommand, QJsonObject* properties)
{
	newCommand->recordingId = properties->value("recordingId").toInt(0);
}

void parser::parse_plotterStart(command* newCommand, QJsonObject* properties)
{
	newCommand->plotterId = properties->value("plotterId").toInt(0);
	newCommand->rangeStart = properties->value("rangeStart").toInt(0);
	newCommand->rangeStop = properties->value("rangeStop").toInt(0);
}

void parser::parse_signalTransform(command* newCommand, QJsonObject* properties)
{
	newCommand->transformId = properties->value("transformId").toString("");
	newCommand->sourcePath = properties->value("sourcePath").toString("");
	newCommand->destinationPath = properties->value("destinationPath").toString("");
}


