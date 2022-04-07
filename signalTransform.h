#pragma once

#include "request.h"

class signalTransform : public request
{
	Q_OBJECT

public:
	signalTransform(command cmd, connection* con);
	~signalTransform();
	void virtual start();
};
