#include "sq_uinatives.h"

#include "../ServerCore.h"
#include "../PlayerManager.h"
#include "../VehicleManager.h"
#include "../NetworkManager.h"
#include "../logging.h"

extern ServerCore server;
extern PlayerManager playm;
extern VehicleManager vm;
extern NetworkManager nm;

void sq_SendMessageToAll(HSQUIRRELVM v)
{
	wchar_t msg[255];
	sq_getstring(v, 2, (const SQChar**)&msg);
	nm.SendChatMessageToAll(msg);
}

void sq_SendMessageToPlayer(HSQUIRRELVM v)
{
	wchar_t msg[255];
	short index;
	sq_getinteger(v, 2, (SQInteger*)&index);
	sq_getstring(v, 3, (const SQChar**)&msg);
	nm.SendChatMessageToOne(index, msg);
}
