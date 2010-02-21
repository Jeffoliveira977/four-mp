function OnGameModeInit()
{
	print("Console test");
	RegServerCmd("sqtest", "Command_sqtest", "Test squirrel command", 0);
	addPlayerClass(0x98E29920, 0, 0, 0, 0, 16, 100, 0, 0, 0, 0);
}

function Command_sqtest(args)
{
	print("Squirrel test command succeded");
}