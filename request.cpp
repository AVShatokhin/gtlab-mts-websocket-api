#include "request.h"


request::request(command cmd, connection * con)
	: QObject(), _cmd(cmd), _connection(con)
{	
}

request::~request()
{
}

void request::stop()
{	
	emit requestCompleted();
	deleteLater();
}

void request::start()
{
	// ����������� start() ����� �������� stop(), 
	// ������� ����� ���� ������������ ��������� ��� ����������������� ������� ����������
	// ����� �������� ������ request ���������� ������ unknownMethod
	_connection->sendTextMessage(render::ERROR_unknownMethod(_cmd.id, _cmd.methodId));
	stop();	
}


void request::sendResponse(QString response)
{
	_connection->sendTextMessage(response);
}
