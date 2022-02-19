#include "session.h"

session::session(parser * par)
	: QObject()
{
	QObject::connect(par, &parser::newCommand, [=] {
		qDebug() << "New command";
		par->send();
		});

	QObject::connect(par, &parser::disconnected, [=] {
		qDebug() << "Disconnected";
		deleteLater();
		});
}

session::~session()
{
}
