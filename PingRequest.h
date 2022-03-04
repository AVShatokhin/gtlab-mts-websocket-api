#pragma once

#include "request.h"
#include "types.h"

#include "render.h"

class PingRequest : public request
{
	Q_OBJECT

public:
	PingRequest(command, connection*);
	~PingRequest();
	virtual void start();
};
