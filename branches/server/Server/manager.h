/*
*	Created:			28.10.09
*	Created by:			WNeZRoS
*	Last Modifed:		-
*/

int GetPlayerID(SystemAddress ad);
int GetPlayerID(const char *ip, int port);
int PlayerConnect(char *name, int index);
void PlayerDisconnect(int index);
int GetFreePlayerClass();