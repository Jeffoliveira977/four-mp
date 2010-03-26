#pragma once

#include "ConsoleCore.h"

void ConCmdDynamic(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsList(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsLoad(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsLoadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsPause(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsPauseAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsReload(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsReloadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsUnload(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsUnloadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsUnpause(ConsoleCore *concore, unsigned char numargs);
void ConCmdFsUnpauseAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginList(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginLoad(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginLoadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginPause(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginPauseAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginReload(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginReloadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginUnload(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginUnloadAll(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginUnpause(ConsoleCore *concore, unsigned char numargs);
void ConCmdPluginUnpauseAll(ConsoleCore *concore, unsigned char numargs);