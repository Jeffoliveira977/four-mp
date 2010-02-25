function OnGameModeInit()
{
	SetScriptName("Console test");
	SetScriptVersion("1.0.0.0");
	SetScriptAuthor("FaTony");
	print("Console test");
	RegServerCmd("sqtest", "Command_sqtest", "Test squirrel command", 0);
	addPlayerClass(0x98E29920, 0, 0, 0, 0, 16, 100, 0, 0, 0, 0);
}

function Command_sqtest(args)
{
	print("Squirrel test command");
	print("Args = "+args+"");
	for (local i = 1; i <= args; i++)
	{
		switch (GetCmdArgType(i))
		{
			case 0:
			{
			print("Arg "+i+" - type = float, value = "+GetCmdArgFloat(i)+"");
			break;
			}
			case 1:
			{
			print("Arg "+i+" - type = int, value = "+GetCmdArgInt(i)+"");
			break;
			}
			case 2:
			{
			print("Arg "+i+" - type = string, value = "+GetCmdArgString(i)+"");
			break;
			}
		}
	}
}