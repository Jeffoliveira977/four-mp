strtokstart <- 0

function OnGameModeInit()
{
	SetScriptName("Building mode");
	SetScriptVersion("0.0.0.1");
	SetScriptAuthor("009");

	CreateCar(0x506434F6, -57.856403, 857.939880, 14.718335, 0.0, 0, 0);
	CreateCar(0x18F25AC7, -58.236626, 841.208984, 14.706969, 0.0, 0, 0);
	CreateCar(0x1D06D681, -58.281269, 820.258911, 14.704250, 0.0, 0, 0);
	CreateCar(0x79FBB0C5, -58.267639, 799.859314, 14.659676, 0.0, 0, 0);
	CreateCar(0xEE6024BC, -58.164066, 772.126892, 14.707888, 0.0, 0, 0);
	
	SetGameTime([12, 0]);

	addPlayerClass(0x98E29920, 0, 0, 0, 0, 16, 100, 0, 0, 0, 0); // Not using
}

function OnGameModeExit()
{
	print("Builing mode by 009 unloaded. "+time()+"\n");
}

function OnPlayerConnect(playerid, name)
{
	SendClientMessageToAll(name + " connected");
}

function OnPlayerDisconnect(playerid)
{
	SendClientMessageToAll(GetPlayerName(playerid) + " [" + playerid + "]" + " disconnect from server");
}

function OnPlayerSpawn(playerid)
{
	print("hello spawn");
	//SetPlayerHP(playerid,200);
	if(playerid == 0) SetPlayerModel(playerid, 0x6AF081E8); // Playboy X
	else if(playerid == 1) SetPlayerModel(playerid, 0x58A1E271); // Liljacob
	else if(playerid == 2) SetPlayerModel(playerid, 0x89395FC9); // Roman
	else SetPlayerModel(playerid, 0x98E29920);
	GivePlayerWeapon(playerid, 15, 150);
}

function OnPlayerDeath(playerid, killerid, reason)
{
	SetPlayerSpawnPosition(playerid, -106.500755, 764.368164, 15.337303, 180.0);
	SendClientMessageToAll(GetPlayerName(killerid) + " [" + killerid + "]" + " kill " + GetPlayerName(playerid) + " [" + playerid + "]" + " reason: " + reason);
}	

function OnPlayerText(playerid, text)
{
	return 1;
}

function OnPlayerCommandText(playerid, cmd)
{
	::strtokstart = 0;
	local comand = strtok(cmd,' ');
	print("COMMAND\n");
	if(comand == "/createcar")
		{
		local carid = strtok(cmd,' ');
		local position = GetPlayerPos(playerid);
		CreateVehicle(carid.tointeger(),position[0],position[1],position[2],0.0,1,1);
		SendClientMessageToAll(GetPlayerName(playerid) + " [" + playerid + "]" + " create vehicle: " + carid.tointeger());
		return 1;
		}
	if(comand == "/getcarpos")
		{
		local comment = strtok(cmd,' ');
		local carid = GetPlayerVehicleID(playerid);
		local model = GetVehicleModel(carid);
		local position = GetVehiclePos(carid);
		local angle = GetVehicleRot(carid);
		if(fexist("vehicles.list")==true)
			{
			local f = fopen("vehicles.list","a");
			fwrite(f,"CreateVehicle(" + model + "," + position[0] + "," + position[1] + "," + position[2] + "," + angle + ",1,1);  //" + comment + "\r\n");
			fclose(f);
			}
		else
			{
			local f = fopen("vehicles.list","w");
			fwrite(f,"CreateVehicle(" + model + "," + position[0] + "," + position[1] + "," + position[2] + "," + angle + ",1,1);  //" + comment + "\r\n");
			fclose(f);
			}
		SendClientMessageToAll("Vehicle info write in file");
		return 1;
		}
	if(comand == "/getmypos")
		{		
		local comment = strtok(cmd,' ');
		local pos = GetPlayerPos(playerid);
		local angle = GetPlayerRot(playerid);
		if(fexist("playerpos.list")==true)
			{
			local f = fopen("playerpos.list","a");
			fwrite(f,"SetPlayerPos(playerid," + pos[0] + "," + pos[1] + "," + pos[2] + ");  //" + comment + "\r\n");
			fwrite(f,"SetPlayerRot(playerid," + angle + ");  //" + comment + "\r\n");
			fclose(f);
			}
		else
			{
			local f = fopen("playerpos.list","w");
			fwrite(f,"SetPlayerPos(playerid," + pos[0] + "," + pos[1] + "," + pos[2] + ");  //" + comment + "\r\n");
			fwrite(f,"SetPlayerRot(playerid," + angle + ");  //" + comment + "\r\n");
			fclose(f);
			}
		SendClientMessageToAll("Player pos info write in file");
		return 1;
		}
	if(comand == "/gun")
		{		
		local gunid = strtok(cmd,' ');
		GivePlayerGun(playerid, gunid.tointeger(),9999);
		return 1;
		}
	if(comand == "/setmyspawnhere")
		{		
		//local pos = GetPlayerPos(playerid);
		//SetPlayerSpawn(playerid,pos[0],pos[1],pos[2]);
		SetPlayerPos(playerid,-106.500755, 764.368164, 15.337303);
		return 1;
		}
	if(comand == "/setmyskin")
		{		
		local skinid = strtok(cmd,' ');
		SetPlayerSkin(playerid,skinid.tointeger());
		return 1;
		}
	return 0;
}

function FixChars(inputstring)
{
local stringfix = "";
for(local l=0;l < inputstring.len(); l++)
	{
	if(inputstring[l].tointeger() > 191)
		{
		local newchar;
		newchar=inputstring[l].tointeger()-50;
		stringfix+=newchar.tochar();
		}
	else
		{
		stringfix+=inputstring[l].tochar();
		}
	}
return stringfix;
}

function DeFixChars(inputstring)
{
local stringfix = "";
for(local l=0;l < inputstring.len(); l++)
	{
	if(inputstring[l].tointeger() < 192)
		{
		if((inputstring[l].tointeger()+50) > 191)
			{
			local newchar;
			newchar=inputstring[l].tointeger()+50;
			stringfix+=newchar.tochar();
			}
		else
			{
			stringfix+=inputstring[l].tochar();
			}
		}
	else
		{
		stringfix+=inputstring[l].tochar();
		}
	}
return stringfix;
}

function strtok(input,char)
{
local result = "";
for(local i= ::strtokstart;i < input.len(); i++)
	{
	if(input[i] == char) 
		{
		::strtokstart=i+1;
		break;
		}
	result+=input[i].tochar();
	}
return result;
}