#include "describeChannels.h"

describeChannels::describeChannels(command cmd, connection * con, adc *adc)
	: request(cmd, con), _adc(adc)
{
}

describeChannels::~describeChannels()
{
}

void describeChannels::start()
{
	sendResponse(render::describeChannels(_cmd.id, _adc->getState()));
	stop();
}
