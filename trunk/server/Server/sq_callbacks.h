#include "sq/squirrel.h"

// SQ Callbacks

/// \brief Called when the game mode starts.
void sc_OnGameModeInit(HSQUIRRELVM v);

/// \brief Called when the game mode ends.
void sc_OnGameModeExit(HSQUIRRELVM v);

/// \brief Called when the filterscript is loaded.
void sc_OnFilterScriptInit(HSQUIRRELVM v);

/// \brief Called when the filterscript is unloaded.
void sc_OnFilterScriptExit(HSQUIRRELVM v);

/// \brief Called when a player connects the server.
/// \param[in] index Index of the player.
/// \param[in] name Name of the player.
/// \return True if we let the player to connect, false otherwise.
bool sc_OnPlayerConnect(HSQUIRRELVM v, const short index, const wchar_t *name);

/// \brief Called when a player is disconnecting from the server.
/// \param[in] index Index of the player.
/// \return No return.
void sc_OnPlayerDisconnect(HSQUIRRELVM v, const short index);

/// \brief Called when a player is spawning.
/// \param[in] playerindex Index of the player.
/// \return No return.
void sc_OnPlayerSpawn(HSQUIRRELVM v, const short playerindex);

/// \brief Called when send message in chat
/// \param[in] playersindex
/// \param[in] text
/// \return No return
bool sc_OnPlayerText(HSQUIRRELVM v, const short playerindex, const wchar_t *text);

/// \brief Fires a dynamic command callback.
/// \param[in] callback Name of the callback to call.
/// \param[in] numargs Number of arguments to pass.
/// \return No return.
void sc_CommandCallback(HSQUIRRELVM v, const wchar_t *callback, const unsigned char numargs);