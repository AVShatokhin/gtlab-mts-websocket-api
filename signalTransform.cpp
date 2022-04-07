#include "signalTransform.h"

signalTransform::signalTransform(command cmd, connection* con)
	: request(cmd, con)
{
}

signalTransform::~signalTransform()
{
}

void signalTransform::start()
{
	sendResponse(render::signalTransform(_cmd.id, NO_ERROR));
	stop();
}
