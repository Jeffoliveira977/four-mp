#pragma once

#include "../../Shared/Console/ConVar.h"
#include "../../Shared/Network/Limits.h"

/// \brief Indicates the global state of the game.
enum GameState
{
	GameStateLoading, ///< Indicates that client is loading.
	GameStateOffline, ///< Indicates that client is offline.
	GameStateUnloading, ///< Indicates that client is unloading.
	GameStateConnecting, ///< Indicates that client is connecting to the server.
	GameStateInGame, ///< Indicates that client is connected to the server and is playing.
	GameStateSkinSelect, ///< Indicates that client is connected to the server and is choosing their skin.
	GameStateComponentSelect ///< Indicates that client is connected to the server and is choosing their components (clothes).
};

#if defined (FMP_CLIENT)
/// \brief Indicates the target that all keyboard and mouse input is redirected to.
enum InputState
{
	InputStateGame, ///< Indicates that all input is going into the game itself.
	InputStateChat, ///< Indicates that all input is going into the chat.
	InputStateGui ///< Indicates that all input is going into the GUI.
};
#endif

class ClientCore
{
public:
	ClientCore(void);
	~ClientCore(void);
	bool Load(void);
	bool IsRunning(void);
	void Tick(void);
	void Unload(void);
	void Shutdown(void);
	GameState GetGameState(void);
	char *GetName(void);
	short GetIndex(void);
	bool SetGameState(const GameState state);
	bool SetIndex(const short i);
	unsigned int GetSessionKey(void);
#if defined (FMP_CLIENT)
	InputState GetInputState(void);
	bool SetInputState(const InputState state);
#endif
private:
	GameState gamestate; ///< Holds the current game state.
	char name[MAX_PLAYER_NAME_LENGTH];
	short index;
	unsigned int sessionkey;
	friend void ConVarHookName(ConVar *convar, const ConVarType oldtype, void *oldvalue, const ConVarType newtype, void *newvalue);
#if defined (FMP_CLIENT)
	InputState inputstate; ///< Holds the current input target.
#endif
};
