#pragma once

#include "request.h"
#include "types.h"

#include "render.h"

class describeChannels : public request
{
	Q_OBJECT

public:
	describeChannels(command, connection*);
	~describeChannels();
	void virtual start();
};
