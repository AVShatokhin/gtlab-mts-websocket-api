#include "PingRequest.h"

PingRequest::PingRequest(command cmd, connection * con)
	: request(cmd, con)
{
}

PingRequest::~PingRequest()
{
}

void PingRequest::start()
{
	sendResponse(render::Ping(_cmd.id));
	stop();
}
