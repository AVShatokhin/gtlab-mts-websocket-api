#include "describeChannels.h"

describeChannels::describeChannels(command cmd, connection * con)
	: request(cmd, con)
{
}

describeChannels::~describeChannels()
{
}

void describeChannels::start()
{
	QList<channelInfo> __channels;

	channelInfo __c;

	__c.channelId = 1;
	__c.displayName = "1";
	__c.samplingRate = 1111;

	__channels << __c;

	__c.channelId = 2;
	__c.displayName = "2";
	__c.samplingRate = 2222;

	__channels << __c;

	sendResponse(render::describeChannels(_cmd.id, &__channels));

	stop();
}


