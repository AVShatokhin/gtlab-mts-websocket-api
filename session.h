#pragma once

#include <QObject>

#include "parser.h"

class session : public QObject
{
	Q_OBJECT

public:
	session(parser *);
	~session();

};
