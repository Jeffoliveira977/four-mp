/*
*	Created:			28.10.09
*	Created by:			009 & VoLT & WNeZRoS
*	Last Modifed:		29.10.09
*/
#pragma once

#include <windows.h>
#include "Hook\types.h"
#include "natives.h"

extern DWORD dwLoadOffset;

namespace Natives
{
	 // Player
    static void AddScore(unsigned int playerIndex, int score) { NativeInvoke::Invoke<ScriptVoid>("ADD_SCORE", playerIndex, score); }
    static void AllowPlayerToCarryNonMissionObjects(unsigned int playerIndex, bool allow) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_PLAYER_TO_CARRY_NON_MISSION_OBJECTS", playerIndex, allow); }
    static void AlterWantedLevel(unsigned int playerIndex,  unsigned int level) { NativeInvoke::Invoke<ScriptVoid>("ALTER_WANTED_LEVEL", playerIndex, level); }
    static void AlterWantedLevelNoDrop(unsigned int playerIndex, unsigned int level) { NativeInvoke::Invoke<ScriptVoid>("ALTER_WANTED_LEVEL_NO_DROP", playerIndex, level); }
    static void ApplyWantedLevelChangeNow(unsigned int playerIndex) { NativeInvoke::Invoke<ScriptVoid>("APPLY_WANTED_LEVEL_CHANGE_NOW", playerIndex); }
    static void ChangePlayerModel(unsigned int playerIndex, eModel model) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PLAYER_MODEL", playerIndex, model); }
    static void ClearPlayerHasDamagedAtLeastOnePed(unsigned int playerIndex) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_PLAYER_HAS_DAMAGED_AT_LEAST_ONE_PED", playerIndex); }
    static unsigned int ConvertIntToPlayerIndex(unsigned int playerId) { return NativeInvoke::Invoke<unsigned int>("CONVERT_INT_TO_PLAYERINDEX", playerId); }
    static void ClearWantedLevel(unsigned int playerIndex) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_WANTED_LEVEL", playerIndex); }
    static void CreatePlayer(unsigned int playerId, float x, float y, float z, int *pPlayerIndex) { NativeInvoke::Invoke<ScriptVoid>("CREATE_PLAYER", playerId, x, y, z, pPlayerIndex); }
    static void DisablePlayerLockon(unsigned int playerIndex, bool disabled) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_PLAYER_LOCKON", playerIndex, disabled); }
    static void DisablePlayerSprint(unsigned int playerIndex, bool disabled) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_PLAYER_SPRINT", playerIndex, disabled); }
    static void GetPlayerChar(unsigned int playerIndex, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_PLAYER_CHAR", playerIndex, pPed); }
    static void GetPlayerGroup(unsigned int playerIndex, Group *pGroup) { NativeInvoke::Invoke<ScriptVoid>("GET_PLAYER_GROUP", playerIndex, pGroup); }
    static unsigned int GetPlayerId() { return NativeInvoke::Invoke<unsigned int>("GET_PLAYER_ID"); }
    static void GetPlayersLastCarNoSave(Vehicle* pVehicle) { NativeInvoke::Invoke<ScriptVoid>("GET_PLAYERS_LAST_CAR_NO_SAVE", pVehicle); }
    static void GetPlayerMaxArmour(unsigned int playerIndex, int *pMaxArmour) { NativeInvoke::Invoke<ScriptVoid>("GET_PLAYER_MAX_ARMOUR", playerIndex, pMaxArmour); }
    static const char *GetPlayerName(unsigned int playerIndex) { return NativeInvoke::Invoke<const char *>("GET_PLAYER_NAME", playerIndex); }
    static eModel GetPlayerSettingsModelChoice() { return NativeInvoke::Invoke<eModel>("GET_PLAYERSETTINGS_MODEL_CHOICE"); }
    static ScriptAny GetPlayerToPlaceBombInCar(Vehicle vehicle) { return NativeInvoke::Invoke<ScriptAny>("GET_PLAYER_TO_PLACE_BOMB_IN_CAR", vehicle); }
    static unsigned int GetTimeSincePlayerDroveAgainstTraffic(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_DROVE_AGAINST_TRAFFIC", playerIndex); }
    static unsigned int GetTimeSincePlayerDroveOnPavement(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_DROVE_ON_PAVEMENT", playerIndex); }
    static unsigned int GetTimeSincePlayerHitBuilding(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_HIT_BUILDING", playerIndex); }
    static unsigned int GetTimeSincePlayerHitCar(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_HIT_CAR", playerIndex); }
    static unsigned int GetTimeSincePlayerHitObject(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_HIT_OBJECT", playerIndex); }
    static unsigned int GetTimeSincePlayerHitPed(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_HIT_PED", playerIndex); }
    static unsigned int GetTimeSincePlayerRanLight(unsigned int playerIndex) { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_PLAYER_RAN_LIGHT", playerIndex); }
    static bool HasPlayerCollectedPickup(unsigned int playerIndex, Pickup pikcup) { return NativeInvoke::Invoke<bool>("HAS_PLAYER_COLLECTED_PICKUP", playerIndex, pikcup); }
    static bool HasPlayerDamagedAtLeastOnePed(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("HAS_PLAYER_DAMAGED_AT_LEAST_ONE_PED", playerIndex); }
    static bool IsPlayerClimbing(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_CLIMBING", playerIndex); }
    static bool IsPlayerControlOn(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_CONTROL_ON", playerIndex); }
    static bool IsPlayerDead(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_DEAD", playerIndex); }
    static bool IsPlayerFreeAimingAtChar(unsigned int playerIndex, Ped ped) { return NativeInvoke::Invoke<bool>("IS_PLAYER_FREE_AIMING_AT_CHAR", playerIndex, ped); }
    static bool IsPlayerFreeForAmbientTask(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_FREE_FOR_AMBIENT_TASK", playerIndex); }
    static bool IsPlayerPlaying(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_PLAYING", playerIndex); }
    static bool IsPlayerPressingHorn(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_PRESSING_HORN", playerIndex); }
    static bool IsPlayerTargettingAnything(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("IS_PLAYER_TARGETTING_ANYTHING", playerIndex); }
    static bool IsPlayerTargettingChar(unsigned int playerIndex, Ped ped) { return NativeInvoke::Invoke<bool>("IS_PLAYER_TARGETTING_CHAR", playerIndex, ped); }
    static bool IsPlayerTargettingObject(unsigned int playerIndex, Object obj) { return NativeInvoke::Invoke<bool>("IS_PLAYER_TARGETTING_OBJECT", playerIndex, obj); }
    static bool IsScoreGreater(unsigned int playerIndex, unsigned int score) { return NativeInvoke::Invoke<bool>("IS_SCORE_GREATER", playerIndex, score); }
    static bool IsWantedLevelGreater(unsigned int playerIndex, unsigned int level) { return NativeInvoke::Invoke<bool>("IS_WANTED_LEVEL_GREATER", playerIndex, level); }
    static bool PlayerHasChar(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("PLAYER_HAS_CHAR", playerIndex); }
    static bool PlayerHasFlashingStarsAboutToDrop(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("PLAYER_HAS_FLASHING_STARS_ABOUT_TO_DROP", playerIndex); }
    static bool PlayerHasGreyedOutStars(unsigned int playerIndex) { return NativeInvoke::Invoke<bool>("PLAYER_HAS_GREYED_OUT_STARS", playerIndex); }
    static void RegisterPlayerRespawnCoords(unsigned int playerIndex, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_PLAYER_RESPAWN_COORDS", playerIndex, x, y, z); }
    static void SetEveryoneIgnorePlayer(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_EVERYONE_IGNORE_PLAYER", playerIndex, value); }
    static void SetPlayerCanBeHassledByGangs(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CAN_BE_HASSLED_BY_GANGS", playerIndex, value); }
    static void SetPlayerCanDoDriveBy(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CAN_DO_DRIVE_BY", playerIndex, value); }
    static void SetPlayerCanUseCover(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CAN_USE_COVER", playerIndex, value); }
    static void SetPlayerControl(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CONTROL", playerIndex, value); }
    static void SetPlayerControlAdvanced(unsigned int playerIndex, bool unknown1, bool unknown2, bool unknown3) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CONTROL_ADVANCED", playerIndex, unknown1, unknown2, unknown3); }
    static void SetPlayerFastReload(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_FAST_RELOAD", playerIndex, value); }
    static void SetPlayerGroupToFollowAlways(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_GROUP_TO_FOLLOW_ALWAYS", playerIndex, value); }
    static void SetPlayerInvincible(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_INVINCIBLE", playerIndex, value); }
    static void SetPlayerMoodNormal(unsigned int playerIndex) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_MOOD_NORMAL", playerIndex); }
    static void SetPlayerMoodPissedOff(unsigned int playerIndex, unsigned int unknown150) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_MOOD_PISSED_OFF", playerIndex, unknown150); }
    static void SetPlayerNeverGetsTired(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_NEVER_GETS_TIRED", playerIndex, value); }
    static void SetPlayerSettingsModelVariationsChoice(unsigned int playerIndex) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYERSETTINGS_MODEL_VARIATIONS_CHOICE", playerIndex); }
    static void SetPoliceIgnorePlayer(unsigned int playerIndex, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_POLICE_IGNORE_PLAYER", playerIndex, value); }
    static void StoreScore(unsigned int playerIndex, unsigned int* value) { NativeInvoke::Invoke<ScriptVoid>("STORE_SCORE", playerIndex, value); }
    static void StoreWantedLevel(unsigned int playerIndex, unsigned int* value) { NativeInvoke::Invoke<ScriptVoid>("STORE_WANTED_LEVEL", playerIndex, value); }
    static void RemovePlayerHelmet(unsigned int playerIndex, bool remove) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_PLAYER_HELMET", playerIndex, remove); }

    // Ped
    static void AddAmmoToChar(Ped ped, eWeapon weapon, unsigned int amount) { NativeInvoke::Invoke<ScriptVoid>("ADD_AMMO_TO_CHAR", ped, weapon, amount); }
    static void AddArmourToChar(Ped ped, unsigned int amount) { NativeInvoke::Invoke<ScriptVoid>("ADD_ARMOUR_TO_CHAR", ped, amount); }
    static void ApplyForceToPed(Ped ped, unsigned int unknown0_3, float x, float y, float z, float spinX, float spinY, float spinZ, unsigned int unknown4_0, unsigned int unknown5_1, unsigned int unknown6_1, unsigned int unknown7_1) { NativeInvoke::Invoke<ScriptVoid>("APPLY_FORCE_TO_PED", ped, unknown0_3, x, y, z, spinX, spinY, spinZ, unknown4_0, unknown5_1, unknown6_1, unknown7_1); }
    static void AttachPedToCar(Ped ped, Vehicle vehicle, unsigned int unknown0_0, float offsetX, float offsetY, float offsetZ, float unknown1_276, float unknown2_0, unsigned int unknown3_0, unsigned int unknown4_0) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_PED_TO_CAR", ped, vehicle, unknown0_0, offsetX, offsetY, offsetZ, unknown1_276, unknown2_0, unknown3_0, unknown4_0); }
	static void BlockCharAmbientAnims(Ped ped, bool block) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_CHAR_AMBIENT_ANIMS", ped, block); }
	static void BlockCharGestureAnims(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_CHAR_GESTURE_ANIMS", ped, value); }
    static void BlockPedWeaponSwitching(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_PED_WEAPON_SWITCHING", ped, value); }
    static void CancelCurrentlyPlayingAmbientSpeech(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CANCEL_CURRENTLY_PLAYING_AMBIENT_SPEECH", ped); }
    static void ClearAllCharProps(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ALL_CHAR_PROPS", ped); }
    static void ClearCharLastDamageBone(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_LAST_DAMAGE_BONE", ped); }
    static void ClearCharLastDamageEntity(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_LAST_DAMAGE_ENTITY", ped); }
    static void ClearCharLastWeaponDamage(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_LAST_WEAPON_DAMAGE", ped); }	
    static void ClearCharProp(Ped ped, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_PROP", ped, unknown); }
    static void ClearCharSecondaryTask(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_SECONDARY_TASK", ped); }
    static void ClearCharTasks(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_TASKS", ped); }
    static void ClearCharTasksImmediately(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_TASKS_IMMEDIATELY", ped); }
    static void ClearRoomForChar(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ROOM_FOR_CHAR", ped); }
    static void CreateChar(unsigned int type, eModel model, float x, float y, float z, Ped *pPed, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CHAR", type, model, x, y, z, pPed, unknownTrue); }
    static void CreateRandomChar(float x, float y, float z, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_RANDOM_CHAR", x, y, z, pPed); }
    static void CreateRandomCharAsDriver(Vehicle vehicle, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_RANDOM_CHAR_AS_DRIVER", vehicle, pPed); }
    static void CreateRandomFemaleChar(float x, float y, float z, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_RANDOM_FEMALE_CHAR", x, y, z, pPed); }
    static void CreateRandomMaleChar(float x, float y, float z, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_RANDOM_MALE_CHAR", x, y, z, pPed); }
    static void DamageChar(Ped ped, unsigned int hitPoints, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("DAMAGE_CHAR", ped, hitPoints, unknown); }
    static void DamageCharBodyPart(Ped ped, ePedBodyPart part, unsigned int hitPoints) { NativeInvoke::Invoke<ScriptVoid>("DAMAGE_PED_BODY_PART", ped, part, hitPoints); }
    static void DeleteChar(Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("DELETE_CHAR", pPed); }
    static void DetachPed(Ped ped, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("DETACH_PED", ped, unknown); }
    static void DetachPedFromWithinCar(Ped ped, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("DETACH_PED_FROM_WITHIN_CAR", ped, unknown); }
    static bool DoesCharExist(Ped ped) { return NativeInvoke::Invoke<bool>("DOES_CHAR_EXIST", ped); }
    static void DropObject(Ped ped, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("DROP_OBJECT", ped, unknownTrue); }
    static void ExplodeCharHead(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("EXPLODE_CHAR_HEAD", ped); }
    static void ExtinguishCharFire(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("EXTINGUISH_CHAR_FIRE", ped); }
    static void FirePedWeapon(Ped ped, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("FIRE_PED_WEAPON", ped, x, y, z); }
    static void ForceCharToDropWeapon(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("FORCE_CHAR_TO_DROP_WEAPON", ped); }
    static ScriptAny ForcePedPinnedDown(Ped ped, bool force, unsigned int timerMaybe) { return NativeInvoke::Invoke<ScriptAny>("FORCE_PED_PINNED_DOWN", ped, force, timerMaybe); }
    static void ForcePedToFleeWhilstDrivingVehicle(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("FORCE_PED_TO_FLEE_WHILST_DRIVING_VEHICLE", ped, vehicle); }
    static void FreezeCharPosition(Ped ped, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_CHAR_POSITION", ped, frozen); }
    static void FreezeCharPositionAndDontLoadCollision(Ped ped, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_CHAR_POSITION_AND_DONT_LOAD_COLLISION", ped, frozen); }
    static void GetAmmoInCharWeapon(Ped ped, eWeapon weapon, int *pAmmo) { NativeInvoke::Invoke<ScriptVoid>("GET_AMMO_IN_CHAR_WEAPON", ped, weapon, pAmmo); }
    static bool GetAmmoInClip(Ped ped, eWeapon weapon, int *pAmmo) { return NativeInvoke::Invoke<bool>("GET_AMMO_IN_CLIP", ped, weapon, pAmmo); }
    static const char *GetAnimGroupFromChar(Ped ped) { return NativeInvoke::Invoke<const char *>("GET_ANIM_GROUP_FROM_CHAR", ped); }
    static void GetCharCoordinates(Ped ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_COORDINATES", ped, pX, pY, pZ); }
    static void GetCharHealth(Ped ped, int *pHealth) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_HEALTH", ped, pHealth); }
    static void GetCharAnimCurrentTime(Ped ped, const char *animGroup, const char *animName, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_ANIM_CURRENT_TIME", ped, animGroup, animName, pValue); }
    static void GetCharAnimTotalTime(Ped ped, const char *animGroup, const char *animName, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_ANIM_TOTAL_TIME", ped, animGroup, animName, pValue); }
    static void GetCharArmour(Ped ped, int *pArmour) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_ARMOUR", ped, pArmour); }
    static unsigned int GetCharDrawableVariation(Ped ped, ePedComponent component) { return NativeInvoke::Invoke<ScriptAny>("GET_CHAR_DRAWABLE_VARIATION", ped, component); }
    static void GetCharExtractedDisplacement(Ped ped, bool unknown, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_EXTRACTED_DISPLACEMENT", ped, unknown, pX, pY, pZ); }
    static void GetCharHeading(Ped ped, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_HEADING", ped, pValue); }
    static void GetCharHeightAboveGround(Ped ped, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_HEIGHT_ABOVE_GROUND", ped, pValue); }
    static ScriptAny GetCharLastDamageBone(Ped ped, ePedBone *pBone) { return NativeInvoke::Invoke<ScriptAny>("GET_CHAR_LAST_DAMAGE_BONE", ped, pBone); }
    static bool GetCharMeleeActionFlag0(Ped ped) { return NativeInvoke::Invoke<bool>("GET_CHAR_MELEE_ACTION_FLAG0", ped); }
    static bool GetCharMeleeActionFlag1(Ped ped) { return NativeInvoke::Invoke<bool>("GET_CHAR_MELEE_ACTION_FLAG1", ped); }
    static void GetCharModel(Ped ped, eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_MODEL", ped, pModel); }
    static unsigned int GetCharMoney(Ped ped) { return NativeInvoke::Invoke<unsigned int>("GET_CHAR_MONEY", ped); }
    static void GetCharPropIndex(Ped ped, bool unknown, int *pIndex) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_PROP_INDEX", ped, unknown, pIndex); }
    static bool GetCharReadyToBeExecuted(Ped ped) { return NativeInvoke::Invoke<bool>("GET_CHAR_READY_TO_BE_EXECUTED", ped); }
    static bool GetCharReadyToBeStunned(Ped ped) { return NativeInvoke::Invoke<bool>("GET_CHAR_READY_TO_BE_STUNNED", ped); }
    static void GetCharSpeed(Ped ped, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_SPEED", ped, pValue); }
    static unsigned int GetCharTextureVariation(Ped ped, ePedComponent component) { return NativeInvoke::Invoke<unsigned int>("GET_CHAR_TEXTURE_VARIATION", ped, component); }
    static void GetCharVelocity(Ped ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_VELOCITY", ped, pX, pY, pZ); }
    static void GetCharWeaponInSlot(Ped ped, eWeaponSlot slot, eWeapon *pWeapon, ScriptAny *pUnknown1, ScriptAny *pUnknown2) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_WEAPON_IN_SLOT", ped, slot, pWeapon, pUnknown1, pUnknown2); }
    static bool GetCharWillCowerInsteadOfFleeing(Ped ped) { return NativeInvoke::Invoke<bool>("GET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING", ped); }
    static ScriptAny GetCurrentCharWeapon(Ped ped, eWeapon *pWeapon) { return NativeInvoke::Invoke<ScriptAny>("GET_CURRENT_CHAR_WEAPON", ped, pWeapon); }
    static unsigned int GetDamageToPedBodyPart(Ped ped, ePedBodyPart part) { return NativeInvoke::Invoke<unsigned int>("GET_DAMAGE_TO_PED_BODY_PART", ped, part); }
    static void GetDeadCharCoordinates(Ped ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_DEAD_CHAR_COORDINATES", ped, pX, pY, pZ); }
    static void GetDeadCharPickupCoords(Ped ped, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_DEAD_CHAR_PICKUP_COORDS", ped, pX, pY, pZ); }
    static void GetKeyForCharInRoom(Ped ped, eInteriorRoomKey *pKey) { NativeInvoke::Invoke<ScriptVoid>("GET_KEY_FOR_CHAR_IN_ROOM", ped, pKey); }
    static ScriptAny GetMaxAmmo(Ped ped, eWeapon weapon, int *pMaxAmmo) { return NativeInvoke::Invoke<ScriptAny>("GET_MAX_AMMO", ped, weapon, pMaxAmmo); }
    static void GetMaxAmmoInClip(Ped ped, eWeapon weapon, int *pMaxAmmo) { NativeInvoke::Invoke<ScriptVoid>("GET_MAX_AMMO_IN_CLIP", ped, weapon, pMaxAmmo); }
    static unsigned int GetNumberOfCharDrawableVariations(Ped ped, ePedComponent component) { return NativeInvoke::Invoke<unsigned int>("GET_NUMBER_OF_CHAR_DRAWABLE_VARIATIONS", ped, component); }
    static unsigned int GetNumberOfCharTextureVariations(Ped ped, ePedComponent component, unsigned int unknown1) { return NativeInvoke::Invoke<unsigned int>("GET_NUMBER_OF_CHAR_TEXTURE_VARIATIONS", ped, component, unknown1); }
    static Object GetObjectPedIsHolding(Ped ped) { return NativeInvoke::Invoke<Object>("GET_OBJECT_PED_IS_HOLDING", ped); }
    static void GetOffsetFromCharInWorldCoords(Ped ped, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OFFSET_FROM_CHAR_IN_WORLD_COORDS", ped, x, y, z, pOffX, pOffY, pOffZ); }
    static ePedClimbState GetPedClimbState(Ped ped) { return NativeInvoke::Invoke<ePedClimbState>("GET_PED_CLIMB_STATE", ped); }
    static void GetPedBonePosition(Ped ped, ePedBone bone, float x, float y, float z, Vector3 *pPosition) { NativeInvoke::Invoke<ScriptVoid>("GET_PED_BONE_POSITION", ped, bone, x, y, z, pPosition); }
    static void GetPedGroupIndex(Ped ped, unsigned int* pIndex) { NativeInvoke::Invoke<ScriptVoid>("GET_PED_GROUP_INDEX", ped, pIndex); }
    static void GetPedType(Ped ped, ePedType *pType) { NativeInvoke::Invoke<ScriptVoid>("GET_PED_TYPE", ped, pType); }
	static void GivePedFakeNetworkName(Ped ped, char * name, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PED_FAKE_NETWORK_NAME", ped, name, red, green, blue, alpha); }
	static void GivePedHelmet(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PED_HELMET", ped); }
    static void GiveWeaponToChar(Ped ped, eWeapon weapon, unsigned int ammo, bool unknown0) { NativeInvoke::Invoke<ScriptVoid>("GIVE_WEAPON_TO_CHAR", ped, weapon, ammo, unknown0); }
    static bool HasCharBeenDamagedByCar(Ped ped, Vehicle vehicle) { return NativeInvoke::Invoke<bool>("HAS_CHAR_BEEN_DAMAGED_BY_CAR", ped, vehicle); }
    static bool HasCharBeenDamagedByChar(Ped ped, Ped otherPed, bool unknownFalse) { return NativeInvoke::Invoke<bool>("HAS_CHAR_BEEN_DAMAGED_BY_CHAR", ped, otherPed, unknownFalse); }
    static bool HasCharBeenDamagedByWeapon(Ped ped, eWeapon weapon) { return NativeInvoke::Invoke<bool>("HAS_CHAR_BEEN_DAMAGED_BY_WEAPON", ped, weapon); }
    static bool HasCharGotWeapon(Ped ped, eWeapon weapon) { return NativeInvoke::Invoke<bool>("HAS_CHAR_GOT_WEAPON", ped, weapon); }
    static bool HasCharSpottedChar(Ped ped, Ped otherPed) { return NativeInvoke::Invoke<bool>("HAS_CHAR_SPOTTED_CHAR", ped, otherPed); }
    static bool HasCharSpottedCharInFront(Ped ped, Ped otherPed) { return NativeInvoke::Invoke<bool>("HAS_CHAR_SPOTTED_CHAR_IN_FRONT", ped, otherPed); }
    static bool IsAmbientSpeechPlaying(Ped ped) { return NativeInvoke::Invoke<bool>("IS_AMBIENT_SPEECH_PLAYING", ped); }
    static bool IsCharArmed(Ped ped, eWeaponSlot slot) { return NativeInvoke::Invoke<bool>("IS_CHAR_ARMED", ped, slot); }
    static bool IsCharDead(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_DEAD", ped); }
    static bool IsCharDucking(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_DUCKING", ped); }
    static bool IsCharFacingChar(Ped ped, Ped otherPed, float angle) { return NativeInvoke::Invoke<bool>("IS_CHAR_FACING_CHAR", ped, otherPed, angle); }
    static bool IsCharFatallyInjured(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_FATALLY_INJURED", ped); }
    static bool IsCharGesturing(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_GESTURING", ped); }
    static bool IsCharGettingInToACar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_GETTING_IN_TO_A_CAR", ped); }
    static bool IsCharGettingUp(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_GETTING_UP", ped); }
    static bool IsCharHealthGreater(Ped ped, unsigned int health) { return NativeInvoke::Invoke<bool>("IS_CHAR_HEALTH_GREATER", ped, health); }
    static bool IsCharInAir(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_AIR", ped); }	
    static bool IsCharInAngledArea2D(Ped ped, float x1, float y1, float x2, float y2, float unknown, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANGLED_AREA_2D", ped, x1, y1, x2, y2, unknown, unknownFalse); }
    static bool IsCharInAngledArea3D(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, float unknown, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANGLED_AREA_3D", ped, x1, y1, z1, x2, y2, z2, unknown, unknownFalse); }
    static bool IsCharInAnyBoat(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_BOAT", ped); }
    static bool IsCharInAnyCar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_CAR", ped); }
    static bool IsCharInAnyHeli(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_HELI", ped); }
    static bool IsCharInAnyPlane(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_PLANE", ped); }
    static bool IsCharInAnyPoliceVehicle(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_POLICE_VEHICLE", ped); }
    static bool IsCharInAnyTrain(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_ANY_TRAIN", ped); }
    static bool IsCharInArea2D(Ped ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_AREA_2D", ped, x1, y1, x2, y2, unknownFalse); }
    static bool IsCharInArea3D(Ped ped, float x1, float y1, float z1, float x2, float y2, float z2, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_AREA_3D", ped, x1, y1, z1, x2, y2, z2, unknownFalse); }
    static bool IsCharInAreaOnFoot2D(Ped ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_AREA_ON_FOOT_2D", ped, x1, y1, x2, y2, unknownFalse); }
    static bool IsCharInCar(Ped ped, Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_CAR", ped, vehicle); }
    static bool IsCharInFlyingVehicle(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_FLYING_VEHICLE", ped); }
    static bool IsCharInMeleeCombat(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_MELEE_COMBAT", ped); }
    static bool IsCharInModel(Ped ped, eModel model) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_MODEL", ped, model); }
    static bool IsCharInTaxi(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_TAXI", ped); }
    static bool IsCharInWater(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_IN_WATER", ped); }
    static bool IsCharInjured(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_INJURED", ped); }
    static bool IsCharMale(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_MALE", ped); }
    static bool IsCharModel(Ped ped, eModel model) { return NativeInvoke::Invoke<bool>("IS_CHAR_MODEL", ped, model); }
    static bool IsCharOnAnyBike(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_ON_ANY_BIKE", ped); }
    static bool IsCharOnFire(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_ON_FIRE", ped); }
    static bool IsCharOnFoot(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_ON_FOOT", ped); }
    static bool IsCharOnScreen(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_ON_SCREEN", ped); }
    static bool IsCharPlayingAnim(Ped ped, char *animSet, char *animName) { return NativeInvoke::Invoke<bool>("IS_CHAR_PLAYING_ANIM", ped, animSet, animName); }
    static bool IsCharShooting(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_SHOOTING", ped); }
    static bool IsCharShootingInArea(Ped ped, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CHAR_SHOOTING_IN_AREA", ped, x1, y1, x2, y2, unknownFalse); }
    static bool IsCharSittingIdle(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_SITTING_IDLE", ped); }
    static bool IsCharSittingInAnyCar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_SITTING_IN_ANY_CAR", ped); }
    static bool IsCharSittingInCar(Ped ped, Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CHAR_SITTING_IN_CAR", ped, vehicle); }
    static bool IsCharStopped(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_STOPPED", ped); }
    static bool IsCharStuckUnderCar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_STUCK_UNDER_CAR", ped); }
    static bool IsCharSwimming(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_SWIMMING", ped); }
    static bool IsPedTouchingChar(Ped ped, Ped otherPed) { return NativeInvoke::Invoke<bool>("IS_CHAR_TOUCHING_CHAR", ped, otherPed); }
    static bool IsCharTouchingObject(Ped ped, Object obj) { return NativeInvoke::Invoke<bool>("IS_CHAR_TOUCHING_OBJECT", ped, obj); }
    static bool IsCharTouchingObjectOnFoot(Ped ped, Object obj) { return NativeInvoke::Invoke<bool>("IS_CHAR_TOUCHING_OBJECT_ON_FOOT", ped, obj); }
    static bool IsCharTouchingVehicle(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<bool>("IS_CHAR_TOUCHING_VEHICLE", p0, p1); }
    static bool IsCharTryingToEnterALockedCar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_TRYING_TO_ENTER_A_LOCKED_CAR", ped); }
    static bool IsCharUsingAnyScenario(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_USING_ANY_SCENARIO", ped); }
    static bool IsCharUsingScenario(Ped ped, const char *scenarioName) { return NativeInvoke::Invoke<bool>("IS_CHAR_USING_SCENARIO", ped, scenarioName); }
    static bool IsCharVisible(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_VISIBLE", ped); }
    static bool IsCharWaitingForWorldCollision(Ped ped) { return NativeInvoke::Invoke<bool>("IS_CHAR_WAITING_FOR_WORLD_COLLISION", ped); }
    static bool IsPedAMissionPed(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_A_MISSION_PED", ped); }
    static bool IsPedAttachedToAnyCar(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_ATTACHED_TO_ANY_CAR", ped); }
    static bool IsPedAttachedToObject(Ped ped, Object obj) { return NativeInvoke::Invoke<bool>("IS_PED_ATTACHED_TO_OBJECT", ped, obj); }
    static bool IsPedBeingJacked(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_BEING_JACKED", ped); }
    static bool IsPedDoingDriveby(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_DOING_DRIVEBY", ped); }
    static bool IsPedFleeing(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_FLEEING", ped); }
    static bool IsPedHoldingAnObject(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_HOLDING_AN_OBJECT", ped); }
    static bool IsPedInCombat(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_IN_COMBAT", ped); }
    static bool IsPedInCover(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_IN_COVER", ped); }
    static bool IsPedInGroup(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_IN_GROUP", ped); }
    static bool IsPedJacking(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_JACKING", ped); }
    static bool IsPedLookingAtPed(Ped ped, Ped otherPed) { return NativeInvoke::Invoke<bool>("IS_PED_LOOKING_AT_PED", ped, otherPed); }
    static bool IsPedRagdoll(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_RAGDOLL", ped); }
    static bool IsPedRetreating(Ped ped) { return NativeInvoke::Invoke<bool>("IS_PED_RETREATING", ped); }
    static bool IsScriptedSpeechPlaying(Ped ped) { return NativeInvoke::Invoke<bool>("IS_SCRIPTED_SPEECH_PLAYING", ped); }
    static void MarkCharAsNoLongerNeeded(Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("MARK_CHAR_AS_NO_LONGER_NEEDED", pPed); }
    static void ModifyCharMoveState(Ped ped, ePedMoveState state) { NativeInvoke::Invoke<ScriptVoid>("MODIFY_CHAR_MOVE_STATE", ped, state); }
    static void RemoveAllCharWeapons(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_ALL_CHAR_WEAPONS", ped); }
    static void RemoveCharDefensiveArea(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CHAR_DEFENSIVE_AREA", ped); }
    static void RemoveCharElegantly(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CHAR_ELEGANTLY", ped); }
    static void RemoveCharFromGroup(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CHAR_FROM_GROUP", ped); }
    static void ReviveInjuredPed(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("REVIVE_INJURED_PED", ped); }
    static void SayAmbientSpeech(Ped ped, char *phraseName, ScriptAny unknown0_1, ScriptAny unknown1_1, ScriptAny unknown2_0) { NativeInvoke::Invoke<ScriptVoid>("SAY_AMBIENT_SPEECH", ped, phraseName, unknown0_1, unknown1_1, unknown2_0); }
    static ScriptAny SetAmmoInClip(Ped ped, eWeapon weapon, unsigned int ammo) { return NativeInvoke::Invoke<ScriptAny>("SET_AMMO_IN_CLIP", ped, weapon, ammo); }
    static void SetBlockingOfNonTemporaryEvents(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_BLOCKING_OF_NON_TEMPORARY_EVENTS", ped, value); }
    static void SetCharAccuracy(Ped ped, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ACCURACY", ped, value); }
    static void SetCharAmmo(Ped ped, eWeapon weapon, unsigned int ammo) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_AMMO", ped, weapon, ammo); }
    static void SetCharAsEnemy(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_AS_ENEMY", ped, value); }
    static void SetPedAsMissionChar(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_AS_MISSION_CHAR", ped); }
    static void SetCharCanBeKnockedOffBike(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CAN_BE_KNOCKED_OFF_BIKE", ped, value); }
    static void SetCharCanBeShotInVehicle(Ped ped, bool enabled) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CAN_BE_SHOT_IN_VEHICLE", ped, enabled); }
    static void SetCharCantBeDraggedOut(Ped ped, bool enabled) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CANT_BE_DRAGGED_OUT", ped, enabled); }
    static void SetCharComponentVariation(Ped ped, ePedComponent component, unsigned int modelVariation, unsigned int textureVariation) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_COMPONENT_VARIATION", ped, component, modelVariation, textureVariation); }
    static void SetCharCoordinates(Ped ped, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_COORDINATES", ped, x, y, z); }
    static void SetCharDefaultComponentVariation(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DEFAULT_COMPONENT_VARIATION", ped); }
	static void SetPedDensityMultiplier(unsigned int den) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_DENSITY_MULTIPLIER", den); }
	static void SetCharDesiredHeading(Ped ped, float heading) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DESIRED_HEADING", ped, heading); }
    static void SetCharDropsWeaponsWhenDead(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DROPS_WEAPONS_WHEN_DEAD", ped, value); }
    static void SetCharDruggedUp(Ped ped, bool drugged) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DRUGGED_UP", ped, drugged); }
    static void SetCharFireDamageMultiplier(Ped ped, float multiplier) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_FIRE_DAMAGE_MULTIPLIER", ped, multiplier); }
    static void SetCharGravity(Ped ped, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_GRAVITY", ped, value); }
    static void SetCharHeading(Ped ped, float heading) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_HEADING", ped, heading); }
    static void SetCharHealth(Ped ped, unsigned int health) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_HEALTH", ped, health); }
    static void SetCharInvincible(Ped ped, bool enable) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_INVINCIBLE", ped, enable); }
    static void SetCharIsTargetPriority(Ped ped, bool enable) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_IS_TARGET_PRIORITY", ped, enable); }
    static void SetCharMaxHealth(Ped ped, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MAX_HEALTH", ped, value); }
    static void SetCharMoney(Ped ped, unsigned int amount) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MONEY", ped, amount); }
    static void SetCharNeverLeavesGroup(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_NEVER_LEAVES_GROUP", ped, value); }
    static void SetCharProofs(Ped ped, bool unknown0, bool fallingDamage, bool unknown1, bool unknown2, bool unknown3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_PROOFS", ped, unknown0, fallingDamage, unknown1, unknown2, unknown3); }
    static void SetCharPropIndex(Ped ped, ePedPropType propType, unsigned int index) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_PROP_INDEX", ped, propType, index); }
    static void SetCharRandomComponentVariation(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_RANDOM_COMPONENT_VARIATION", ped); }
    static void SetCharSphereDefensiveArea(Ped ped, float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_SPHERE_DEFENSIVE_AREA", ped, x, y, z, radius); }
    static void SetCharSuffersCriticalHits(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_SUFFERS_CRITICAL_HITS", ped, value); }
    static void SetCharVelocity(Ped ped, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_VELOCITY", ped, x, y, z); }
    static void SetCharVisible(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_VISIBLE", ped, value); }
    static void SetCharWantedByPolice(Ped ped, bool wanted) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WANTED_BY_POLICE", ped, wanted); }
    static void SetCharWillDoDrivebys(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_DO_DRIVEBYS", ped, value); }
    static void SetCharWillFlyThroughWindscreen(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_FLY_THROUGH_WINDSCREEN", ped, value); }
    static void SetCharWillMoveWhenInjured(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_MOVE_WHEN_INJURED", ped, value); }
    static void SetCharWillUseCarsInCombat(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_USE_CARS_IN_COMBAT", ped, value); }
    static void SetCharWillUseCover(Ped ped, CoverPoint coverPoint) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_USE_COVER", ped, coverPoint); }
    static void SetCurrentCharWeapon(Ped ped, eWeapon w, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("SET_CURRENT_CHAR_WEAPON", ped, w, unknownTrue); }
    static void SetGroupCharDucksWhenAimedAt(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_CHAR_DUCKS_WHEN_AIMED_AT", ped, value); }
    static void SetIgnoreLowPriorityShockingEvents(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_IGNORE_LOW_PRIORITY_SHOCKING_EVENTS", ped, value); }
    static void SetPedDiesWhenInjured(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_DIES_WHEN_INJURED", ped, value); }
    static void SetPedIsBlindRaging(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_IS_BLIND_RAGING", ped, value); }
    static void SetPedIsDrunk(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_IS_DRUNK", ped, value); }
    static void SetPedPathMayDropFromHeight(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_PATH_MAY_DROP_FROM_HEIGHT", ped, value); }
    static void SetPedPathMayUseClimbovers(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_PATH_MAY_USE_CLIMBOVERS", ped, value); }
    static void SetPedPathMayUseLadders(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_PATH_MAY_USE_LADDERS", ped, value); }
    static void SetRoomForCharByKey(Ped ped, eInteriorRoomKey key) { NativeInvoke::Invoke<ScriptVoid>("SET_ROOM_FOR_CHAR_BY_KEY", ped, key); }
    static void SetSenseRange(Ped ped, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_SENSE_RANGE", ped, value); }
    static void SwitchPedToAnimated(Ped ped, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_PED_TO_ANIMATED", ped, unknownTrue); }
    static ScriptAny SwitchPedToRagdoll(Ped ped, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("SWITCH_PED_TO_RAGDOLL", ped, p1, p2, p3, p4, p5, p6); }    
    static void RemovePedHelmet(Ped ped, bool removed) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_PED_HELMET", ped, removed); }
    static void RemoveWeaponFromChar(Ped ped, eWeapon weapon) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_WEAPON_FROM_CHAR", ped, weapon); }
    static void UnlockRagdoll(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("UNLOCK_RAGDOLL", ped, value); }
    static void WarpCharFromCarToCoord(Ped ped, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("WARP_CHAR_FROM_CAR_TO_COORD", ped, x, y, z); }
    static void WarpCharIntoCar(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("WARP_CHAR_INTO_CAR", ped, vehicle); }
    static void WarpCharIntoCarAsPassenger(Ped ped, Vehicle vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<ScriptVoid>("WARP_CHAR_INTO_CAR_AS_PASSENGER", ped, vehicle, seatIndex); }
    static void WarpCharFromCarToCar(Ped ped, Vehicle vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<ScriptVoid>("WARP_CHAR_FROM_CAR_TO_CAR", ped, vehicle, seatIndex); }


    // Ped Tasks
    static void SetCharKeepTask(Ped ped, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_KEEP_TASK", ped, value); }
    static void SetDriveTaskCruiseSpeed(Ped ped, float speed) { NativeInvoke::Invoke<ScriptVoid>("SET_DRIVE_TASK_CRUISE_SPEED", ped, speed); }
    static void TaskAchieveHeading(Ped ped, float heading) { NativeInvoke::Invoke<ScriptVoid>("TASK_ACHIEVE_HEADING", ped, heading); }
    static void TaskAimGunAtChar(Ped ped, Ped targetPed, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_AIM_GUN_AT_CHAR", ped, targetPed, duration); } 
    static void TaskAimGunAtCoord(Ped ped, float tX, float tY, float tZ, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_AIM_GUN_AT_COORD", ped, tX, tY, tZ, duration); }
    static void TaskCarDriveWander(Ped ped, Vehicle vehicle, float speed, eVehicleDrivingStyle drivingStyle) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_DRIVE_WANDER", ped, vehicle, speed, drivingStyle); }
   	static void TaskCarDriveToCoord(Ped ped, Vehicle car, float x, float y, float z, float speed, unsigned int p1f_2b, unsigned int p2, unsigned int p3, unsigned int px2, unsigned int p45000) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_DRIVE_TO_COORD", ped, car, x, y, z, speed, p1f_2b, p2, p3, px2, p45000); }
	static void TaskCarMission(Ped ped, Vehicle vehicle, ScriptHandle targetEntity, unsigned int missionType, float speed, eVehicleDrivingStyle drivingStyle, unsigned int unknown6_10, unsigned int unknown7_5) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_MISSION", ped, vehicle, targetEntity, missionType, speed, drivingStyle, unknown6_10, unknown7_5); } // target is whatever missiontype requires (ie. vehicle or just 0). missiontypes: 5=wait(), 21=drivetoplayer()
    static void TaskCarMissionNotAgainstTraffic(Ped ped, Vehicle vehicle, ScriptHandle targetEntity, unsigned int missionType, float speed, eVehicleDrivingStyle  drivingStyle, unsigned int unknown6_10, unsigned int unknown7_5) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_MISSION_NOT_AGAINST_TRAFFIC", ped, vehicle, targetEntity, missionType, speed, drivingStyle, unknown6_10, unknown7_5); }
    static void TaskCarMissionCoorsTarget(Ped ped, Vehicle vehicle, float x, float y, float z, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_MISSION_COORS_TARGET", ped, vehicle, x, y, z, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
    static void TaskCarMissionCoorsTargetNotAgainstTraffic(Ped ped, Vehicle vehicle, float x, float y, float z, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_MISSION_COORS_TARGET_NOT_AGAINST_TRAFFIC", ped, vehicle, x, y, z, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
    static void TaskCarMissionPedTarget(Ped ped, Vehicle vehicle, Ped target, unsigned int unknown0_4, float speed, unsigned int unknown2_1, unsigned int unknown3_5, unsigned int unknown4_10) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_MISSION_PED_TARGET", ped, vehicle, target, unknown0_4, speed, unknown2_1, unknown3_5, unknown4_10); }
    static void TaskCarTempAction(Ped ped, Vehicle vehicle, unsigned int action, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_TEMP_ACTION", ped, vehicle, action, duration); } // action 6 = wait?
    static void TaskCombat(Ped ped, Ped target) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT", ped, target); }
    static void TaskCombatHatedTargetsAroundChar(Ped ped, float radius) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT_HATED_TARGETS_AROUND_CHAR", ped, radius); }
    static void TaskCombatHatedTargetsAroundCharTimed(Ped ped, float radius, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT_HATED_TARGETS_AROUND_CHAR_TIMED", ped, radius, duration); }
    static void TaskCombatTimed(Ped ped, Ped target, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT_TIMED", ped, target, duration); }
    static void TaskDie(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("TASK_DIE", ped); }
    static void TaskEnterCarAsDriver(Ped ped, Vehicle vehicle, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_ENTER_CAR_AS_DRIVER", ped, vehicle, duration); }
    static void TaskEnterCarAsPassenger(Ped ped, Vehicle vehicle, unsigned int duration, unsigned int seatIndex) { NativeInvoke::Invoke<ScriptVoid>("TASK_ENTER_CAR_AS_PASSENGER", ped, vehicle, duration, seatIndex); }
    static void TaskEveryoneLeaveCar(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("TASK_EVERYONE_LEAVE_CAR", vehicle); }
    static void TaskFollowNavMeshToCoord(Ped ped, float x, float y, float z, unsigned int unknown0_2, unsigned int unknown1_minus1, float unknown2_1) { NativeInvoke::Invoke<ScriptVoid>("TASK_FOLLOW_NAV_MESH_TO_COORD", ped, x, y, z, unknown0_2, unknown1_minus1, unknown2_1); }
    static void TaskFollowNavMeshToCoordNoStop(Ped ped, float x, float y, float z,unsigned int unknown0_2, unsigned int unknown1_minus1, float unknown2_1) { NativeInvoke::Invoke<ScriptVoid>("TASK_FOLLOW_NAV_MESH_TO_COORD_NO_STOP", ped, x, y, z, unknown0_2, unknown1_minus1, unknown2_1); }
    static void TaskGoStraightToCoord(Ped ped, float x, float y, float z, unsigned int unknown2, unsigned int unknown45000) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_STRAIGHT_TO_COORD", ped, x, y, z, unknown2, unknown45000); }
    static void TaskGotoCharOffset(Ped ped, Ped target, unsigned int duration, float offsetRight, float offsetFront) { NativeInvoke::Invoke<ScriptVoid>("TASK_GOTO_CHAR_OFFSET", ped, target, duration, offsetRight, offsetFront); }
    static void TaskGuardCurrentPosition(Ped ped, float unknown0_15, float unknown1_10, unsigned int unknown2_1) { NativeInvoke::Invoke<ScriptVoid>("TASK_GUARD_CURRENT_POSITION", ped, unknown0_15, unknown1_10, unknown2_1); }
    static void TaskHandsUp(Ped ped, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_HANDS_UP", ped, duration); }
    static void TaskHeliMission(Ped ped, Vehicle heli, unsigned int uk0_0, unsigned int uk1_0, float pX, float pY, float pZ, unsigned int uk2_4, float speed, unsigned int uk3_5, float uk4_minus1, unsigned int uk5_round_z_plus_1, unsigned int uk6_40) { NativeInvoke::Invoke<ScriptVoid>("TASK_HELI_MISSION", ped, heli, uk0_0, uk1_0, pX, pY, pZ, uk2_4, speed, uk3_5, uk4_minus1, uk5_round_z_plus_1, uk6_40); }
    static void TaskJump(Ped ped, bool jump) { NativeInvoke::Invoke<ScriptVoid>("TASK_JUMP", ped, jump); }
	static void TaskLeaveAnyCar(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_ANY_CAR", ped); }
    static void TaskLeaveCar(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_CAR", ped, vehicle); }
    static void TaskLeaveCarDontCloseDoor(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_CAR_DONT_CLOSE_DOOR", ped, vehicle); }
    static void TaskLeaveCarImmediately(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_CAR_IMMEDIATELY", ped, vehicle); }
    static void TaskLookAtChar(Ped ped, Ped targetPed, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<ScriptVoid>("TASK_LOOK_AT_CHAR", ped, targetPed, duration, unknown_0); }
    static void TaskLookAtCoord(Ped ped, float x, float y, float z, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<ScriptVoid>("TASK_LOOK_AT_COORD", ped, x, y, z, duration, unknown_0); }
    static void TaskLookAtObject(Ped ped, Object targetObject, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<ScriptVoid>("TASK_LOOK_AT_OBJECT", ped, targetObject, duration, unknown_0); }
    static void TaskLookAtVehicle(Ped ped, Vehicle targetVehicle, unsigned int duration, unsigned int unknown_0) { NativeInvoke::Invoke<ScriptVoid>("TASK_LOOK_AT_VEHICLE", ped, targetVehicle, duration, unknown_0); }
    static void TaskOpenDriverDoor(Ped ped, Vehicle vehicle, unsigned int unknown0) { NativeInvoke::Invoke<ScriptVoid>("TASK_OPEN_DRIVER_DOOR", ped, vehicle, unknown0); }
    static void TaskOpenPassengerDoor(Ped ped, Vehicle vehicle, unsigned int seatIndex, unsigned int unknown0) { NativeInvoke::Invoke<ScriptVoid>("TASK_OPEN_PASSENGER_DOOR", ped, vehicle, seatIndex, unknown0); }
    static void TaskPause(Ped ped, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_PAUSE", ped, duration); }
    static void TaskPlayAnimWithFlags(Ped ped, const char *animName, const char *animSet, float unknown0_8, unsigned int unknown1_0, unsigned int flags) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_WITH_FLAGS", ped, animName, animSet, unknown0_8, unknown1_0, flags); }
    static void TaskSetCharDecisionMaker(Ped ped, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("TASK_SET_CHAR_DECISION_MAKER", ped, dm); }
    static void TaskSetCombatDecisionMaker(Ped ped, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("TASK_SET_COMBAT_DECISION_MAKER", ped, dm); }
	static void TaskShootAtCoord(Ped ped, float x, float y, float z, unsigned int time_ms, unsigned int type_5) { NativeInvoke::Invoke<ScriptVoid>("TASK_SHOOT_AT_COORD", ped, x, y, z, time_ms, type_5); }
	static void TaskSmartFleeChar(Ped ped, Ped fleeFromPed, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_SMART_FLEE_CHAR", ped, fleeFromPed, unknown0_100, duration); }
    static void TaskSmartFleeCharPreferringPavements(Ped ped, Ped fleeFromPed, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_SMART_FLEE_CHAR_PREFERRING_PAVEMENTS", ped, fleeFromPed, unknown0_100, duration); }
    static void TaskSmartFleePoint(Ped ped, float x, float y, float z, float unknown0_100, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_SMART_FLEE_POINT", ped, x, y, x, unknown0_100, duration); }
    static void TaskStandStill(Ped ped, int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_STAND_STILL", ped, duration); }
    static void TaskSwapWeapon(Ped ped, eWeapon weapon) { NativeInvoke::Invoke<ScriptVoid>("TASK_SWAP_WEAPON", ped, weapon); }
	static void TaskToggleDuck(Ped ped, bool duck) { NativeInvoke::Invoke<ScriptVoid>("TASK_TOGGLE_DUCK", ped, duck); }
	static void TaskTurnPedToFaceChar(Ped ped, Ped targetPed) { NativeInvoke::Invoke<ScriptVoid>("TASK_TURN_CHAR_TO_FACE_CHAR", ped, targetPed); }
    static void TaskTurnCharToFaceCoord(Ped ped, float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("TASK_TURN_CHAR_TO_FACE_COORD", ped, x, y, z); }
    static void TaskUseMobilePhone(Ped ped, bool use) { NativeInvoke::Invoke<ScriptVoid>("TASK_USE_MOBILE_PHONE", ped, use); }
    static void TaskUseMobilePhoneTimed(Ped ped, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("TASK_USE_MOBILE_PHONE_TIMED", ped, duration); }
    static void TaskWanderStandard(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("TASK_WANDER_STANDARD", ped); }
    static void TaskWarpCharIntoCarAsDriver(Ped ped, Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("TASK_WARP_CHAR_INTO_CAR_AS_DRIVER", ped, vehicle); }
    static void TaskWarpCharIntoCarAsPassenger(Ped ped, Vehicle vehicle, unsigned int seatIndex) { NativeInvoke::Invoke<ScriptVoid>("TASK_WARP_CHAR_INTO_CAR_AS_PASSENGER", ped, vehicle, seatIndex); }

    // Task Sequence
    static void OpenSequenceTask(TaskSequence *pTaskSequence) { NativeInvoke::Invoke<ScriptVoid>("OPEN_SEQUENCE_TASK", pTaskSequence); }
    static void CloseSequenceTask(TaskSequence taskSequence) { NativeInvoke::Invoke<ScriptVoid>("CLOSE_SEQUENCE_TASK", taskSequence); }
    static void TaskPerformSequence(Ped ped, TaskSequence taskSequence) { NativeInvoke::Invoke<ScriptVoid>("TASK_PERFORM_SEQUENCE", ped, taskSequence); }
    static void ClearSequenceTask(TaskSequence taskSequence) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_SEQUENCE_TASK", taskSequence); }

    // Ped Search
    static void AllowScenarioPedsToBeReturnedByNextCommand(bool value) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_SCENARIO_PEDS_TO_BE_RETURNED_BY_NEXT_COMMAND", value); }
    static void BeginCharSearchCriteria() { NativeInvoke::Invoke<ScriptVoid>("BEGIN_CHAR_SEARCH_CRITERIA"); }
    static void EndCharSearchCriteria() { NativeInvoke::Invoke<ScriptVoid>("END_CHAR_SEARCH_CRITERIA"); }
    static bool GetClosestChar(float x, float y, float z, float radius, unsigned int unknown1, unsigned int unknown2, Ped *pPed) { return NativeInvoke::Invoke<bool>("GET_CLOSEST_CHAR", x, y, z, radius, unknown1, unknown2, pPed); }
    static void GetRandomCharInAreaOffsetNoSave(float x, float y, float z, float sx, float sy, float sz, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CHAR_IN_AREA_OFFSET_NO_SAVE", x, y, z, sx, sy, sz, pPed); }
    static void SearchCriteriaConsiderPedsWithFlagTrue(unsigned int flagId) { NativeInvoke::Invoke<ScriptVoid>("SEARCH_CRITERIA_CONSIDER_PEDS_WITH_FLAG_TRUE", flagId); }
    static void SearchCriteriaRejectPedsWithFlagTrue(unsigned int flagId) { NativeInvoke::Invoke<ScriptVoid>("SEARCH_CRITERIA_REJECT_PEDS_WITH_FLAG_TRUE", flagId); }

    // Ped Groups
    static void CreateGroup(bool unknownFalse, Group *pGroup, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("CREATE_GROUP", unknownFalse, pGroup, unknownTrue); }
    static bool DoesGroupExist(Group group) { return NativeInvoke::Invoke<bool>("DOES_GROUP_EXIST", group); }
    static void GetGroupLeader(Group group, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_GROUP_LEADER", group, pPed); }
    static void GetGroupMember(Group group, unsigned int index, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_GROUP_MEMBER", group, index, pPed); }
    static void GetGroupSize(Group group, int *pStartIndex, int *pCount) { NativeInvoke::Invoke<ScriptVoid>("GET_GROUP_SIZE", group, pStartIndex, pCount); }
    static bool IsGroupLeader(Ped ped, Group group) { return NativeInvoke::Invoke<bool>("IS_GROUP_LEADER", ped, group); }
    static bool IsGroupMember(Ped ped, Group g) { return NativeInvoke::Invoke<bool>("IS_GROUP_MEMBER", ped, g); }
    static void RemoveGroup(Group group) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_GROUP", group); }
    static void SetGroupFollowStatus(Group group, unsigned int status) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_FOLLOW_STATUS", group, status); }
    static void SetGroupFormation(Group group, unsigned int formation) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_FORMATION", group, formation); }
    static void SetGroupFormationSpacing(Group group, float space) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_FORMATION_SPACING", group, space); }
    static void SetGroupLeader(Group group, Ped leader) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_LEADER", group, leader); }
    static void SetGroupMember(Group group, Ped member) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_MEMBER", group, member); }
    static void SetGroupSeparationRange(Group group, float seperation) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_SEPARATION_RANGE", group, seperation); }

    // Ped Relationships
    static void AllowGangRelationshipsToBeChangedByNextCommand(bool value) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_GANG_RELATIONSHIPS_TO_BE_CHANGED_BY_NEXT_COMMAND", value); }
    static void SetCharNotDamagedByRelationshipGroup(Ped ped, unsigned int relationshipGroup, bool enable) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_NOT_DAMAGED_BY_RELATIONSHIP_GROUP", ped, relationshipGroup, enable); }
    static void SetCharRelationship(Ped ped, unsigned int relationshipLevel, unsigned int relationshipGroup) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_RELATIONSHIP", ped, relationshipLevel, relationshipGroup); }
    static void SetCharRelationshipGroup(Ped ped, unsigned int relationshipGroup) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_RELATIONSHIP_GROUP", ped, relationshipGroup); }
    static void SetRelationship(unsigned int relationshipLevel, unsigned int relationshipGroup1, unsigned int relationshipGroup2) { NativeInvoke::Invoke<ScriptVoid>("SET_RELATIONSHIP", relationshipLevel, relationshipGroup1, relationshipGroup2); }

    // Vehicle
    static void AddUpsideDownCarCheck(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("ADD_UPSIDEDOWN_CAR_CHECK", vehicle); }
    static void AnchorBoat(Vehicle boat, bool anchor) { NativeInvoke::Invoke<ScriptVoid>("ANCHOR_BOAT", boat, anchor); }
    static void ApplyForceToCar(Vehicle vehicle, unsigned int unknown0_3, float x, float y, float z, float spinX, float spinY, float spinZ, unsigned int unknown4_0, unsigned int unknown5_1, unsigned int unknown6_1, unsigned int unknown7_1) { NativeInvoke::Invoke<ScriptVoid>("APPLY_FORCE_TO_CAR", vehicle, unknown0_3, x, y, z, spinX, spinY, spinZ, unknown4_0, unknown5_1, unknown6_1, unknown7_1); }
    static bool AreTaxiLightsOn(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("ARE_TAXI_LIGHTS_ON", vehicle); }
    static void BreakCarDoor(Vehicle vehicle, eVehicleDoor door, bool unknownFalse) { NativeInvoke::Invoke<ScriptVoid>("BREAK_CAR_DOOR", vehicle, door, unknownFalse); }
    static void BurstCarTyre(Vehicle vehicle, eVehicleTyre tyre) { NativeInvoke::Invoke<ScriptVoid>("BURST_CAR_TYRE", vehicle, tyre); }
    static void CreateCar(unsigned int nameHash, float x, float y, float z, Vehicle *pVehicle, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CAR", nameHash, x, y, z, pVehicle, unknownTrue); }
    static void CreateCharAsPassenger(Vehicle vehicle, ePedType charType, eModel model, unsigned int passengerIndex, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CHAR_AS_PASSENGER", vehicle, charType, model, passengerIndex, pPed); }
    static void CreateCharInsideCar(Vehicle vehicle, ePedType charType, eModel model, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CHAR_INSIDE_CAR", vehicle, charType, model, pPed); }
    static void ChangeCarColour(Vehicle vehicle, unsigned int colour1, unsigned int colour2) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_CAR_COLOUR", vehicle, colour1, colour2); }
    static void ClearCarLastDamageEntity(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CAR_LAST_DAMAGE_ENTITY", vehicle); }
    static void ClearCarLastWeaponDamage(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CAR_LAST_WEAPON_DAMAGE", vehicle); }
    static void ClearRoomForCar(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ROOM_FOR_CAR", vehicle); }
    static void CloseAllCarDoors(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("CLOSE_ALL_CAR_DOORS", vehicle); }
    static void ControlCarDoor(Vehicle vehicle, eVehicleDoor door, unsigned int unknown_maybe_open, float angle) { NativeInvoke::Invoke<ScriptVoid>("CONTROL_CAR_DOOR", vehicle, door, unknown_maybe_open, angle); }
    static bool CreateEmergencyServicesCar(eModel model, float x, float y, float z) { return NativeInvoke::Invoke<bool>("CREATE_EMERGENCY_SERVICES_CAR", model, x, y, z); }
    static void CreateRandomCharAsPassenger(Vehicle vehicle, unsigned int seat, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("CREATE_RANDOM_CHAR_AS_PASSENGER", vehicle, seat, pPed); }
    static void DeleteCar(Vehicle *pVehicle) { NativeInvoke::Invoke<ScriptVoid>("DELETE_CAR", pVehicle); }
    static void DetachCar(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("DETACH_CAR", vehicle); }
    static bool DoesCarHaveRoof(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("DOES_CAR_HAVE_ROOF", vehicle); }
    static bool DoesCarHaveStuckCarCheck(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("DOES_CAR_HAVE_STUCK_CAR_CHECK", vehicle); }
    static bool DoesVehicleExist(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("DOES_VEHICLE_EXIST", vehicle); }
    static void EnablePedHelmet(Ped ped, bool enable) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_PED_HELMET", ped, enable); }
    static void ExplodeCar(Vehicle vehicle, bool unknownTrue, bool unknownFalse) { NativeInvoke::Invoke<ScriptVoid>("EXPLODE_CAR", vehicle, unknownTrue, unknownFalse); }
    static void ExtinguishCarFire(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("EXTINGUISH_CAR_FIRE", vehicle); }
    static void FixCar(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("FIX_CAR", vehicle); }
    static void FixCarTyre(Vehicle vehicle, eVehicleTyre tyre) { NativeInvoke::Invoke<ScriptVoid>("FIX_CAR_TYRE", vehicle, tyre); }
    static void ForceVehicleLights(Vehicle vehicle, eVehicleLights lights) { NativeInvoke::Invoke<ScriptVoid>("FORCE_CAR_LIGHTS", vehicle, lights); }
    static void FreezeCarPosition(Vehicle vehicle, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_CAR_POSITION", vehicle, frozen); }
    static void FreezeCarPositionAndDontLoadCollision(Vehicle vehicle, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_CAR_POSITION_AND_DONT_LOAD_COLLISION", vehicle, frozen); }
    static void GetCarCharIsUsing(Ped ped, Vehicle *pVehicle) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_CHAR_IS_USING", ped, pVehicle); }
    static void GetCarColours(Vehicle vehicle, int *pColour1, int *pColour2) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_COLOURS", vehicle, pColour1, pColour2); }
    static void GetCarCoordinates(Vehicle vehicle, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_COORDINATES", vehicle, pX, pY, pZ); }
    static void GetCarDeformationAtPos(Vehicle vehicle, float x, float y, float z, Vector3 *pDeformation) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_DEFORMATION_AT_POS", vehicle, x, y, z, pDeformation); }
    static void GetCarDoorLockStatus(Vehicle vehicle, eVehicleDoorLock *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_DOOR_LOCK_STATUS", vehicle, pValue); }
    static void GetCarForwardX(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_FORWARD_X", vehicle, pValue); }
    static void GetCarForwardY(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_FORWARD_Y", vehicle, pValue); }
    static void GetCarHeading(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_HEADING", vehicle, pValue); }
    static void GetCarHealth(Vehicle vehicle, int *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_HEALTH", vehicle, pValue); }
    static void GetCarModel(Vehicle vehicle, eModel *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_MODEL", vehicle, pValue); }
    static void GetCarPitch(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_PITCH", vehicle, pValue); }
    static void GetCarRoll(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_ROLL", vehicle, pValue); }
    static void GetCarSpeed(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_SPEED", vehicle, pValue); }
    static void GetCarSpeedVector(Vehicle vehicle, Vector3 *pVector, bool unknownFalse) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_SPEED_VECTOR", vehicle, pVector, unknownFalse); }
    static void GetCarUprightValue(Vehicle vehicle, float *pValue) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_UPRIGHT_VALUE", vehicle, pValue); }
    static void GetCharInCarPassengerSeat(Vehicle vehicle, unsigned int seatIndex, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_CHAR_IN_CAR_PASSENGER_SEAT", vehicle, seatIndex, pPed); }
    static Vehicle GetClosestCar(float x, float y, float z, float radius, unsigned int unknown0_0, unsigned int unknown1_70) { return NativeInvoke::Invoke<Vehicle>("GET_CLOSEST_CAR", x, y, z, radius, unknown0_0, unknown1_70); }
    static bool GetClosestCarNode(float x, float y, float z, float *pResX, float *pResY, float *pResZ) { return NativeInvoke::Invoke<bool>("GET_CLOSEST_CAR_NODE", x, y, z, pResX, pResY, pResZ); }
    static bool GetClosestCarNodeWithHeading(float x, float y, float z, float *pResX, float *pResY, float *pResZ, float *pHeading) { return NativeInvoke::Invoke<bool>("GET_CLOSEST_CAR_NODE_WITH_HEADING", x, y, z, pResX, pResY, pResZ, pHeading); }
    static bool GetClosestMajorCarNode(float x, float y, float z, float *pResX, float *pResY, float *pResZ) { return NativeInvoke::Invoke<bool>("GET_CLOSEST_MAJOR_CAR_NODE", x, y, z, pResX, pResY, pResZ); }
    static bool GetNthClosestCarNodeWithHeading(float x, float y, float z, unsigned int nodeNum,  float *pResX, float *pResY, float *pResZ, float *pHeading) { return NativeInvoke::Invoke<bool>("GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING", x, y, z, nodeNum, pResX, pResY, pResZ, pHeading); }
    static bool GetNthClosestCarNodeWithHeadingOnIsland(float x, float y, float z, unsigned int nodeNum, unsigned int areaId, float *pResX, float *pResY, float *pResZ, float *pHeading, int *pUnknownMaybeAreaId) { return NativeInvoke::Invoke<bool>("GET_NTH_CLOSEST_CAR_NODE_WITH_HEADING_ON_ISLAND", x, y, z, nodeNum, areaId, pResX, pResY, pResZ, pHeading, pUnknownMaybeAreaId); }
    static void GetDeadCarCoordinates(Vehicle vehicle, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_DEAD_CAR_COORDINATES", vehicle, pX, pY, pZ); }
    static void GetDoorAngleRatio(Vehicle vehicle, eVehicleDoor door, float *pAngleRatio) { NativeInvoke::Invoke<ScriptVoid>("GET_DOOR_ANGLE_RATIO", vehicle, door, pAngleRatio); }
    static void GetDriverOfCar(Vehicle vehicle, Ped *pPed) { NativeInvoke::Invoke<ScriptVoid>("GET_DRIVER_OF_CAR", vehicle, pPed); }
    static float GetEngineHealth(Vehicle vehicle) { return NativeInvoke::Invoke<float>("GET_ENGINE_HEALTH", vehicle); }
    static void GetExtraCarColours(Vehicle vehicle, int *pColour1, int *pColour2) { NativeInvoke::Invoke<ScriptVoid>("GET_EXTRA_CAR_COLOURS", vehicle, pColour1, pColour2); }
    static float GetHeightOfVehicle(Vehicle vehicle, float x, float y, float z, bool unknownTrue1, bool unknownTrue2) { return NativeInvoke::Invoke<float>("GET_HEIGHT_OF_VEHICLE", vehicle, x, y, z, unknownTrue1, unknownTrue2); }
    static void GetKeyForCarInRoom(Vehicle vehicle, int *pKey) { NativeInvoke::Invoke<ScriptVoid>("GET_KEY_FOR_CAR_IN_ROOM", vehicle, pKey); }
    static void GetMaximumNumberOfPassengers(Vehicle vehicle, int *pMax) { NativeInvoke::Invoke<ScriptVoid>("GET_MAXIMUM_NUMBER_OF_PASSENGERS", vehicle, pMax); }
    static void GetNearestCableCar(float x, float y, float z, float radius, Vehicle *pVehicle) { NativeInvoke::Invoke<ScriptVoid>("GET_NEAREST_CABLE_CAR", x, y, z, radius, pVehicle); }
    static void GetNumCarColours(Vehicle vehicle, int *pNumColours) { NativeInvoke::Invoke<ScriptVoid>("GET_NUM_CAR_COLOURS", vehicle, pNumColours); }
    static void GetNumberOfPassengers(Vehicle vehicle, int *pNumPassengers) { NativeInvoke::Invoke<ScriptVoid>("GET_NUMBER_OF_PASSENGERS", vehicle, pNumPassengers); }
    static void GetOffsetFromCarGivenWorldCoords(Vehicle vehicle, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OFFSET_FROM_CAR_GIVEN_WORLD_COORDS", vehicle, x, y, z, pOffX, pOffY, pOffZ); }
    static void GetOffsetFromCarInWorldCoords(Vehicle vehicle, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OFFSET_FROM_CAR_IN_WORLD_COORDS", vehicle, x, y, z, pOffX, pOffY, pOffZ); }
    static float GetPetrolTankHealth(Vehicle vehicle) { return NativeInvoke::Invoke<float>("GET_PETROL_TANK_HEALTH", vehicle); }
    static void GetRandomCarModelInMemory(bool unknownTrue, int *pHash, ScriptAny *pUnknown) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CAR_MODEL_IN_MEMORY", unknownTrue, pHash, pUnknown); }
    static void GetVehicleDirtLevel(Vehicle vehicle, float* pIntensity) { NativeInvoke::Invoke<ScriptVoid>("GET_VEHICLE_DIRT_LEVEL", vehicle, pIntensity); }
    static bool HasCarBeenDamagedByCar(Vehicle vehicle, Vehicle otherCar) { return NativeInvoke::Invoke<bool>("HAS_CAR_BEEN_DAMAGED_BY_CAR", vehicle, otherCar); }
    static bool HasCarBeenDamagedByChar(Vehicle vehicle, Ped ped) { return NativeInvoke::Invoke<bool>("HAS_CAR_BEEN_DAMAGED_BY_CHAR", vehicle, ped); }
    static bool HasCarBeenDamagedByWeapon(Vehicle vehicle, eWeapon weapon) { return NativeInvoke::Invoke<bool>("HAS_CAR_BEEN_DAMAGED_BY_WEAPON", vehicle, weapon); }
    static bool HasCarBeenResprayed(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("HAS_CAR_BEEN_RESPRAYED", vehicle); }
    static bool IsBigVehicle(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_BIG_VEHICLE", vehicle); }
    static bool IsCarAMissionCar(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_A_MISSION_CAR", vehicle); }
    static bool IsCarAttached(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_ATTACHED", vehicle); }
    static bool IsCarDead(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_DEAD", vehicle); }
    static bool IsCarDoorDamaged(Vehicle vehicle, eVehicleDoor door) { return NativeInvoke::Invoke<bool>("IS_CAR_DOOR_DAMAGED", vehicle, door); }
    static bool IsCarDoorFullyOpen(Vehicle vehicle, eVehicleDoor door) { return NativeInvoke::Invoke<bool>("IS_CAR_DOOR_FULLY_OPEN", vehicle, door); }
    static bool IsCarInAirProper(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_IN_AIR_PROPER", vehicle); }
    static bool IsCarInArea2D(Vehicle vehicle, float x1, float y1, float x2, float y2, bool unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CAR_IN_AREA_2D", vehicle, x1, y1, x2, y2, unknownFalse); }
    static bool IsCarInArea3D(Vehicle vehicle, float x1, float y1, float z1, float x2, float y2, float z2, unsigned char unknownFalse) { return NativeInvoke::Invoke<bool>("IS_CAR_IN_AREA_3D", vehicle, x1, y1, z1, x2, y2, z2, unknownFalse); }
    static bool IsCarInWater(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_IN_WATER", vehicle); }
    static bool IsCarModel(Vehicle vehicle, eModel model) { return NativeInvoke::Invoke<bool>("IS_CAR_MODEL", vehicle, model); }
    static bool IsCarOnFire(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_ON_FIRE", vehicle); }
    static bool IsCarOnScreen(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_ON_SCREEN", vehicle); }
    static bool IsCarPassengerSeatFree(Vehicle vehicle, unsigned int seatIndex) { return NativeInvoke::Invoke<bool>("IS_CAR_PASSENGER_SEAT_FREE", vehicle, seatIndex); }
    static bool IsCarSirenOn(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_SIREN_ON", vehicle); }
    static bool IsCarStopped(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_STOPPED", vehicle); }
    static bool IsCarStoppedAtTrafficLights(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_STOPPED_AT_TRAFFIC_LIGHTS", vehicle); }
    static bool IsCarStuckOnRoof(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_STUCK_ON_ROOF", vehicle); }
    static bool IsCarTouchingCar(Vehicle vehicle, Vehicle otherCar) { return NativeInvoke::Invoke<bool>("IS_CAR_TOUCHING_CAR", vehicle, otherCar); }
    static bool IsCarTyreBurst(Vehicle vehicle, eVehicleTyre tyre) { return NativeInvoke::Invoke<bool>("IS_CAR_TYRE_BURST", vehicle, tyre); }
    static bool IsCarUpright(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_UPRIGHT", vehicle); }
    static bool IsCarUpsidedown(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_UPSIDEDOWN", vehicle); }
    static bool IsCarWaitingForWorldCollision(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_WAITING_FOR_WORLD_COLLISION", vehicle); }
    static bool IsVehDriveable(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_VEH_DRIVEABLE", vehicle); }
    static bool IsVehWindowIntact(Vehicle vehicle, eVehicleWindow window) { return NativeInvoke::Invoke<bool>("IS_VEH_WINDOW_INTACT", vehicle, window); }
    static bool IsVehicleExtraTurnedOn(Vehicle vehicle, eVehicleExtra extra) { return NativeInvoke::Invoke<bool>("IS_VEHICLE_EXTRA_TURNED_ON", vehicle, extra); }
    static bool IsVehicleOnAllWheels(Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_VEHICLE_ON_ALL_WHEELS", vehicle); }
    static void KnockPedOffBike(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("KNOCK_PED_OFF_BIKE", vehicle); }
    static void LockCarDoors(Vehicle vehicle, eVehicleDoorLock value) { NativeInvoke::Invoke<ScriptVoid>("LOCK_CAR_DOORS", vehicle, value); }
    static void MarkCarAsConvoyCar(Vehicle vehicle, bool convoyCar) { NativeInvoke::Invoke<ScriptVoid>("MARK_CAR_AS_CONVOY_CAR", vehicle, convoyCar); }
    static void MarkCarAsNoLongerNeeded(Vehicle *pVehicle) { NativeInvoke::Invoke<ScriptVoid>("MARK_CAR_AS_NO_LONGER_NEEDED", pVehicle); }
    static void OpenCarDoor(Vehicle vehicle, eVehicleDoor door) { NativeInvoke::Invoke<ScriptVoid>("OPEN_CAR_DOOR", vehicle, door); }
    static void PopCarBoot(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("POP_CAR_BOOT", vehicle); }
    static void RemoveVehicleWindow(Vehicle vehicle, eVehicleWindow window) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CAR_WINDOW", vehicle, window); }
    static void RemoveStuckCarCheck(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_STUCK_CAR_CHECK", vehicle); }
    static void RemoveUpsidedownCarCheck(Vehicle vehicle) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_UPSIDEDOWN_CAR_CHECK", vehicle); }
    static void SetCarCoordinates(Vehicle vehicle, float pX, float pY, float pZ) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_COORDINATES", vehicle, pX, pY, pZ); }
    static void SetCarHealth(Vehicle vehicle, unsigned int pValue) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_HEALTH", vehicle, pValue); }
    static void SetPetrolTankHealth(Vehicle vehicle, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_PETROL_TANK_HEALTH", vehicle, value); }
    static void SetCarCanBeDamaged(Vehicle vehicle, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_CAN_BE_DAMAGED", vehicle, value); }
    static void SetCarCanBeVisiblyDamaged(Vehicle vehicle, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_CAN_BE_VISIBLY_DAMAGED", vehicle, value); }
	static void SetCarDensityMultiplier(unsigned int den) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_DENSITY_MULTIPLIER", den); }
	static void SetCarForwardSpeed(Vehicle vehicle, float speed) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_FORWARD_SPEED", vehicle, speed); }
    static void SetCarHeading(Vehicle vehicle, float dir) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_HEADING", vehicle, dir); }
    static ScriptAny SetCarOnGroundProperly(Vehicle vehicle) { return NativeInvoke::Invoke<ScriptAny>("SET_CAR_ON_GROUND_PROPERLY", vehicle); }
    static void SetCarProofs(Vehicle vehicle, bool bulletProof, bool fireProof, bool explosionProof, bool collisionProof, bool meleeProof) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_PROOFS", vehicle, bulletProof, fireProof, explosionProof, collisionProof, meleeProof); }
    static void SetCarStrong(Vehicle vehicle, bool strong) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_STRONG", vehicle, strong); }
    static void SetCarVisible(Vehicle vehicle, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_VISIBLE", vehicle, value); }
    static void SetExtraCarColours(Vehicle vehicle, unsigned int colour1, unsigned int colour2) { NativeInvoke::Invoke<ScriptVoid>("SET_EXTRA_CAR_COLOURS", vehicle, colour1, colour2); }
    static void SetEngineHealth(Vehicle vehicle, float health) { NativeInvoke::Invoke<ScriptVoid>("SET_ENGINE_HEALTH", vehicle, health); }
	static void SetParkedCarDensityMultiplier(unsigned int den) { NativeInvoke::Invoke<ScriptVoid>("SET_PARKED_CAR_DENSITY_MULTIPLIER", den); }
	static void SetRandomCarDensityMultiplier(unsigned int den) { NativeInvoke::Invoke<ScriptVoid>("SET_RANDOM_CAR_DENSITY_MULTIPLIER", den); }   
	static void SetVehHazardLights(Vehicle vehicle, bool on) { NativeInvoke::Invoke<ScriptVoid>("SET_VEH_HAZARDLIGHTS", vehicle, on); }
    static void SetVehicleDirtLevel(Vehicle vehicle, float intensity) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_DIRT_LEVEL", vehicle, intensity); }
    static void ShutCarDoor(Vehicle vehicle, eVehicleDoor door) { NativeInvoke::Invoke<ScriptVoid>("SHUT_CAR_DOOR", vehicle, door); }
    static void SoundCarHorn(Vehicle vehicle, unsigned int duration) { NativeInvoke::Invoke<ScriptVoid>("SOUND_CAR_HORN", vehicle, duration); }
    static void WashVehicleTextures(Vehicle vehicle, unsigned int intensity) { NativeInvoke::Invoke<ScriptVoid>("WASH_VEHICLE_TEXTURES", vehicle, intensity); }

    // Train
    static void CreateMissionTrain(unsigned int unknown1, float x, float y, float z, bool unknown2, Train *pTrain) { NativeInvoke::Invoke<ScriptVoid>("CREATE_MISSION_TRAIN", unknown1, x, y, z, unknown2, pTrain); }
	static void DeleteAllTrains() { NativeInvoke::Invoke<ScriptVoid>("DELETE_ALL_TRAINS"); }
	static void DeleteMissionTrain(Train *pTrain) { NativeInvoke::Invoke<ScriptVoid>("DELETE_MISSION_TRAIN", pTrain); }
    static eTrainStation GetCurrentStationForTrain(Train train) { return NativeInvoke::Invoke<eTrainStation>("GET_CURRENT_STATION_FOR_TRAIN", train); }
    static eTrainStation GetNextStationForTrain(Train train) { return NativeInvoke::Invoke<eTrainStation>("GET_NEXT_STATION_FOR_TRAIN", train); }
    static const char *GetStationName(Train train, eTrainStation station) { return NativeInvoke::Invoke<const char *>("GET_STATION_NAME", train, station); }
    static void MarkMissionTrainAsNoLongerNeeded(Train train) { NativeInvoke::Invoke<ScriptVoid>("MARK_MISSION_TRAIN_AS_NO_LONGER_NEEDED", train); }
    static void MarkMissionTrainsAsNoLongerNeeded() { NativeInvoke::Invoke<ScriptVoid>("MARK_MISSION_TRAINS_AS_NO_LONGER_NEEDED"); }

    // Object
    static void AddObjectToInteriorRoomByKey(Object obj, eInteriorRoomKey roomKey) { NativeInvoke::Invoke<ScriptVoid>("ADD_OBJECT_TO_INTERIOR_ROOM_BY_KEY", obj, roomKey); }
    static void ApplyForceToObject(Object obj, unsigned int uk0_3, float pX, float pY, float pZ, float spinX, float spinY, float spinZ, unsigned int uk4_0, unsigned int uk5_1, unsigned int uk6_1, unsigned int uk7_1) { NativeInvoke::Invoke<ScriptVoid>("APPLY_FORCE_TO_OBJECT", obj, uk0_3, pX, pY, pZ, spinX, spinY, spinZ, uk4_0, uk5_1, uk6_1, uk7_1); }
    static void AttachObjectToCar(Object obj, Vehicle v, unsigned int unknown0_0, float pX, float pY, float pZ, float rX, float rY, float rZ) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_OBJECT_TO_CAR", obj, v, unknown0_0, pX, pY, pZ, rX, rY, rZ); }
    static void AttachObjectToPed(Object obj, Ped c, ePedBone bone, float pX, float pY, float pZ, float rX, float rY, float rZ, unsigned int unknown1_0) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_OBJECT_TO_PED", obj, c, bone, pX, pY, pZ, rX, rY, rZ, unknown1_0); }
    static void ClearObjectLastDamageEntity(Object obj) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_OBJECT_LAST_DAMAGE_ENTITY", obj); }
    static void ClearRoomForObject(Object obj) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ROOM_FOR_OBJECT", obj); }
    static void CreateObject(eModel model, float x, float y, float z, Object *pObj, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("CREATE_OBJECT", model, x, y, z, pObj, unknownTrue); }
    static void CreateObjectNoOffset(eModel model, float x, float y, float z, Object *pObj, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("CREATE_OBJECT_NO_OFFSET", model, x, y, z, pObj, unknownTrue); }
    static void DeleteObject(Object *pObj) { NativeInvoke::Invoke<ScriptVoid>("DELETE_OBJECT", pObj); }
    static void DetachObject(Object obj, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("DETACH_OBJECT", obj, unknown); }
    static bool DoesObjectExist(Object obj) { return NativeInvoke::Invoke<bool>("DOES_OBJECT_EXIST", obj); }
    static bool DoesObjectHavePhysics(Object obj) { return NativeInvoke::Invoke<bool>("DOES_OBJECT_HAVE_PHYSICS", obj); }
    static bool DoesObjectOfTypeExistAtCoords(float x, float y, float z, float radius, eModel model) { return NativeInvoke::Invoke<bool>("DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS", x, y, z, radius, model); }
    static void ExtinguishObjectFire(Object obj) { NativeInvoke::Invoke<ScriptVoid>("EXTINGUISH_OBJECT_FIRE", obj); }
    static void FreezeObjectPosition(Object obj, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_OBJECT_POSITION", obj, frozen); }
    static Vehicle GetCarObjectIsAttachedTo(Object obj) { return NativeInvoke::Invoke<Vehicle>("GET_CAR_OBJECT_IS_ATTACHED_TO", obj); }
    static void GetObjectCoordinates(Object obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_COORDINATES", obj, pX, pY, pZ); }
    static float GetObjectFragmentDamageHealth(Object obj, bool unknown) { return NativeInvoke::Invoke<float>("GET_OBJECT_FRAGMENT_DAMAGE_HEALTH", obj, unknown); }
    static void GetObjectHeading(Object obj, float *pHeading) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_HEADING", obj, pHeading); }
    static void GetObjectHealth(Object obj, float *pHealth) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_HEALTH", obj, pHealth); }
    static void GetObjectModel(Object obj, eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_MODEL", obj, pModel); }
    static void GetObjectRotationVelocity(Object obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_ROTATION_VELOCITY", obj, pX, pY, pZ); }
    static void GetObjectSpeed(Object obj, float *pSpeed) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_SPEED", obj, pSpeed); }
    static void GetObjectVelocity(Object obj, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_VELOCITY", obj, pX, pY, pZ); }
    static void GetOffsetFromObjectInWorldCoords(Object obj, float x, float y, float z, float *pOffX, float *pOffY, float *pOffZ) { NativeInvoke::Invoke<ScriptVoid>("GET_OFFSET_FROM_OBJECT_IN_WORLD_COORDS", obj, x, y, z, pOffX, pOffY, pOffZ); }
    static Ped GetPedObjectIsAttachedTo(Object obj) { return NativeInvoke::Invoke<Ped>("GET_PED_OBJECT_IS_ATTACHED_TO", obj); }
    static bool HasObjectBeenDamaged(Object obj) { return NativeInvoke::Invoke<bool>("HAS_OBJECT_BEEN_DAMAGED", obj); }
    static bool HasObjectBeenDamagedByCar(Object obj, Vehicle vehicle) { return NativeInvoke::Invoke<bool>("HAS_OBJECT_BEEN_DAMAGED_BY_CAR", obj, vehicle); }
    static bool HasObjectBeenDamagedByChar(Object obj, Ped ped) { return NativeInvoke::Invoke<bool>("HAS_OBJECT_BEEN_DAMAGED_BY_CHAR", obj, ped); }
    static bool HasObjectBeenUprooted(Object obj) { return NativeInvoke::Invoke<bool>("HAS_OBJECT_BEEN_UPROOTED", obj); }
    static bool HasObjectCollidedWithAnything(Object obj) { return NativeInvoke::Invoke<bool>("HAS_OBJECT_COLLIDED_WITH_ANYTHING", obj); }
    static bool HasPoolObjectCollidedWithCushion(Object obj) { return NativeInvoke::Invoke<bool>("HAS_POOL_OBJECT_COLLIDED_WITH_CUSHION", obj); }
    static bool HasPoolObjectCollidedWithObject(Object obj, Object otherObj) { return NativeInvoke::Invoke<bool>("HAS_POOL_OBJECT_COLLIDED_WITH_OBJECT", obj, otherObj); }
    static bool IsObjectAttached(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_ATTACHED", obj); }
    static bool IsObjectInWater(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_IN_WATER", obj); }
    static bool IsObjectOnFire(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_ON_FIRE", obj); }
    static bool IsObjectOnScreen(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_ON_SCREEN", obj); }
    static bool IsObjectStatic(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_STATIC", obj); }
    static bool IsObjectUpright(Object obj, float angle) { return NativeInvoke::Invoke<bool>("IS_OBJECT_UPRIGHT", obj, angle); }
    static bool IsObjectWithinBrainActivationRange(Object obj) { return NativeInvoke::Invoke<bool>("IS_OBJECT_WITHIN_BRAIN_ACTIVATION_RANGE", obj); }
    static void LoadAllObjectsNow() { NativeInvoke::Invoke<ScriptVoid>("LOAD_ALL_OBJECTS_NOW"); }
    static void MakeObjectTargettable(Object obj, bool targettable) { NativeInvoke::Invoke<ScriptVoid>("MAKE_OBJECT_TARGETTABLE", obj, targettable); }
    static void MarkObjectAsNoLongerNeeded(Object *pObj) { NativeInvoke::Invoke<ScriptVoid>("MARK_OBJECT_AS_NO_LONGER_NEEDED", pObj); }
    static void SetObjectCollision(Object obj, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_COLLISION", obj, value); }
    static void SetObjectCoordinates(Object obj, float pX, float pY, float pZ) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_COORDINATES", obj, pX, pY, pZ); }
    static void SetObjectHeading(Object obj, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_HEADING", obj, value); }
    static void SetObjectVisible(Object obj, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_VISIBLE", obj, value); }
    static ScriptAny StartObjectFire(Object obj) { return NativeInvoke::Invoke<ScriptAny>("START_OBJECT_FIRE", obj); }

    // Models
    static bool HasModelLoaded(eModel model) { return NativeInvoke::Invoke<bool>("HAS_MODEL_LOADED", model); }
    static void RequestModel(eModel model) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_MODEL", model); }
    static void GetCurrentBasicCopModel(eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_BASIC_COP_MODEL", pModel); }
    static void GetCurrentBasicPoliceCarModel(eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_BASIC_POLICE_CAR_MODEL", pModel); }
    static void GetCurrentCopModel(eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_COP_MODEL", pModel); }
    static void GetCurrentPoliceCarModel(eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_POLICE_CAR_MODEL", pModel); }
    static void GetCurrentTaxiCarModel(eModel *pModel) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_TAXI_CAR_MODEL", pModel); }
    static const char *GetDisplayNameFromVehicleModel(eModel model) { return NativeInvoke::Invoke<const char *>("GET_DISPLAY_NAME_FROM_VEHICLE_MODEL", model); }
    static void GetModelDimensions(eModel model, Vector3 *pMinVector, Vector3 *pMaxVector) { NativeInvoke::Invoke<ScriptVoid>("GET_MODEL_DIMENSIONS", model, pMinVector, pMaxVector); }
    static const char *GetModelNameForDebug(eModel model) { return NativeInvoke::Invoke<const char *>("GET_MODEL_NAME_FOR_DEBUG", model); }
    static bool IsThisModelABike(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_BIKE", model); }
    static bool IsThisModelABoat(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_BOAT", model); }
    static bool IsThisModelACar(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_CAR", model); }
    static bool IsThisModelAHeli(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_HELI", model); }
    static bool IsThisModelAPed(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_PED", model); }
    static bool IsThisModelAPlane(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_PLANE", model); }
    static bool IsThisModelATrain(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_TRAIN", model); }
    static bool IsThisModelAVehicle(eModel model) { return NativeInvoke::Invoke<bool>("IS_THIS_MODEL_A_VEHICLE", model); }
    static void MarkModelAsNoLongerNeeded(eModel model) { NativeInvoke::Invoke<ScriptVoid>("MARK_MODEL_AS_NO_LONGER_NEEDED", model); }

    // Interiors
    static void ActivateInterior(Interior interior, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_INTERIOR", interior, unknownTrue); }
    static void GetInteriorAtCoords(float x, float y, float z, Interior *pInterior) { NativeInvoke::Invoke<ScriptVoid>("GET_INTERIOR_AT_COORDS", x, y, z, pInterior); }
    static void GetInteriorFromCar(Vehicle vehicle, Interior *pInterior) { NativeInvoke::Invoke<ScriptVoid>("GET_INTERIOR_FROM_CAR", vehicle, pInterior); }
    static void GetInteriorFromChar(Ped ped, Interior *pInterior) { NativeInvoke::Invoke<ScriptVoid>("GET_INTERIOR_FROM_CHAR", ped, pInterior); }
    static void GetInteriorHeading(Interior interior, float *pHeading) { NativeInvoke::Invoke<ScriptVoid>("GET_INTERIOR_HEADING", interior, pHeading); }
    static void GetOffsetFromInteriorInWorldCoords(Interior interior, float x, float y, float z, float *pOffset) { NativeInvoke::Invoke<ScriptVoid>("GET_OFFSET_FROM_INTERIOR_IN_WORLD_COORDS", interior, x, y, z, pOffset); }
    static void GetRoomKeyFromObject(Object obj, eInteriorRoomKey *pRoomKey) { NativeInvoke::Invoke<ScriptVoid>("GET_ROOM_KEY_FROM_OBJECT", obj, pRoomKey); }
    static void RequestInteriorModels(eModel model, const char *interiorName) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_INTERIOR_MODELS", model, interiorName); }

    // Decision Maker
    static void AddCharDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<ScriptVoid>("ADD_CHAR_DECISION_MAKER_EVENT_RESPONSE", dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
    static void AddCombatDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<ScriptVoid>("ADD_COMBAT_DECISION_MAKER_EVENT_RESPONSE", dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
    static void AddGroupDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid, unsigned int responseid, float param1, float param2, float param3, float param4, unsigned int unknown0_1, unsigned int unknown1_1) { NativeInvoke::Invoke<ScriptVoid>("ADD_GROUP_DECISION_MAKER_EVENT_RESPONSE", dm, eventid, responseid, param1, param2, param3, param4, unknown0_1, unknown1_1); }
    static void ClearCharDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_DECISION_MAKER_EVENT_RESPONSE", dm, eventid); }
    static void ClearCombatDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_COMBAT_DECISION_MAKER_EVENT_RESPONSE", dm, eventid); }
    static void ClearGroupDecisionMakerEventResponse(DecisionMaker dm, unsigned int eventid) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_GROUP_DECISION_MAKER_EVENT_RESPONSE", dm, eventid); }
    static void CopyCharDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_CHAR_DECISION_MAKER", type, pDM); }
    static void CopyCombatDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_COMBAT_DECISION_MAKER", type, pDM); }
    static void CopyGroupCharDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_GROUP_CHAR_DECISION_MAKER", type, pDM); }
    static void CopyGroupCombatDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_GROUP_COMBAT_DECISION_MAKER", type, pDM); }
    static void CopySharedCharDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_SHARED_CHAR_DECISION_MAKER", type, pDM); }
    static void CopySharedCombatDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("COPY_SHARED_COMBAT_DECISION_MAKER", type, pDM); }
    static bool DoesDecisionMakerExist(DecisionMaker dm) { return NativeInvoke::Invoke<bool>("DOES_DECISION_MAKER_EXIST", dm); }
    static void LoadCharDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("LOAD_CHAR_DECISION_MAKER", type, pDM); }
    static void LoadCombatDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("LOAD_COMBAT_DECISION_MAKER", type, pDM); }
    static void LoadGroupDecisionMaker(unsigned int type, DecisionMaker *pDM) { NativeInvoke::Invoke<ScriptVoid>("LOAD_GROUP_DECISION_MAKER", type, pDM); }
    static void RemoveDecisionMaker(DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_DECISION_MAKER", dm); }
    static void SetCharDecisionMaker(Ped ped, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DECISION_MAKER", ped, dm); }
    static void SetCharDecisionMakerToDefault(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DECISION_MAKER_TO_DEFAULT", ped); }
    static void SetCombatDecisionMaker(Ped ped, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("SET_COMBAT_DECISION_MAKER", ped, dm); }
    static void SetDecisionMakerAttributeCanChangeTarget(DecisionMaker dm, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_CAN_CHANGE_TARGET", dm, value); }
    static void SetDecisionMakerAttributeCaution(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_CAUTION", dm, value); }
    static void SetDecisionMakerAttributeFireRate(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_FIRE_RATE", dm, value); }
    static void SetDecisionMakerAttributeLowHealth(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_LOW_HEALTH", dm, value); }
    static void SetDecisionMakerAttributeMovementStyle(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_MOVEMENT_STYLE", dm, value); }
    static void SetDecisionMakerAttributeNavigationStyle(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_NAVIGATION_STYLE", dm, value); }
    static void SetDecisionMakerAttributeRetreatingBehaviour(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_RETREATING_BEHAVIOUR", dm, value); }
    static void SetDecisionMakerAttributeSightRange(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_SIGHT_RANGE", dm, value); }
    static void SetDecisionMakerAttributeStandingStyle(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_STANDING_STYLE", dm, value); }
    static void SetDecisionMakerAttributeTargetInjuredReaction(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_TARGET_INJURED_REACTION", dm, value); }
    static void SetDecisionMakerAttributeTargetLossResponse(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_TARGET_LOSS_RESPONSE", dm, value); }
    static void SetDecisionMakerAttributeTeamwork(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_TEAMWORK", dm, value); }
    static void SetDecisionMakerAttributeWeaponAccuracy(DecisionMaker dm, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_DECISION_MAKER_ATTRIBUTE_WEAPON_ACCURACY", dm, value); }
    static void SetGroupCharDecisionMaker(Group group, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_CHAR_DECISION_MAKER", group, dm); }
    static void SetGroupCombatDecisionMaker(Group group, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_COMBAT_DECISION_MAKER", group, dm); }
    static void SetGroupDecisionMaker(Group group, DecisionMaker dm) { NativeInvoke::Invoke<ScriptVoid>("SET_GROUP_DECISION_MAKER", group, dm); }

    // Blips
    static void AddBlipForCar(Vehicle vehicle, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_CAR", vehicle, pBlip); }
    static void AddBlipForChar(Ped ped, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_CHAR", ped, pBlip); }
    static void AddBlipForContact(float x, float y, float z, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_CONTACT", x, y, z, pBlip); }
    static void AddBlipForCoord(float x, float y, float z, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_COORD", x, y, z, pBlip); }
    static void AddBlipForObject(Object obj, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_OBJECT", obj, pBlip); }
    static void AddBlipForPickup(Pickup pickup, Blip *pBlip) { NativeInvoke::Invoke<ScriptVoid>("ADD_BLIP_FOR_PICKUP", pickup, pBlip); }
    static void AddSimpleBlipForPickup(Pickup pickup) { NativeInvoke::Invoke<ScriptVoid>("ADD_SIMPLE_BLIP_FOR_PICKUP", pickup); }
    static void ChangeBlipAlpha(Blip blip, ScriptAny alpha) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_ALPHA", blip, alpha); }
    static void ChangeBlipColour(Blip blip, unsigned int colour) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_COLOUR", blip, colour); }
    static void ChangeBlipDisplay(Blip blip, eBlipDisplay display) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_DISPLAY", blip, display); }
    static void ChangeBlipNameFromAscii(Blip blip, const char *blipName) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_NAME_FROM_ASCII", blip, blipName); }
    static void ChangeBlipNameFromTextFile(Blip blip, const char *gxtName) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_NAME_FROM_TEXT_FILE", blip, gxtName); }
    static void ChangeBlipPriority(Blip blip, eBlipPriority priority) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_PRIORITY", blip, priority); }
    static void ChangeBlipScale(Blip blip, float scale) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_SCALE", blip, scale); }
    static void ChangeBlipSprite(Blip blip, eBlipSprite sprite) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_BLIP_SPRITE", blip, sprite); }
    static void DimBlip(Blip blip, bool unknownTrue) { NativeInvoke::Invoke<ScriptVoid>("DIM_BLIP", blip, unknownTrue); }
    static bool DoesBlipExist(Blip blip) { return NativeInvoke::Invoke<bool>("DOES_BLIP_EXIST", blip); }
    static void FlashBlip(Blip blip, bool on) { NativeInvoke::Invoke<ScriptVoid>("FLASH_BLIP", blip, on); }
    static void FlashBlipAlt(Blip blip, bool on) { NativeInvoke::Invoke<ScriptVoid>("FLASH_BLIP_ALT", blip, on); }
    static void GetBlipColour(Blip blip, int *pColour) { NativeInvoke::Invoke<ScriptVoid>("GET_BLIP_COLOUR", blip, pColour); }
    static void GetBlipCoords(Blip blip, Vector3 *pVector) { NativeInvoke::Invoke<ScriptVoid>("GET_BLIP_COORDS", blip, pVector); }
    static eBlipType GetBlipInfoIdType(Blip blip) { return NativeInvoke::Invoke<eBlipType>("GET_BLIP_INFO_ID_TYPE", blip); }
    static Vehicle GetBlipInfoIdCarIndex(Blip blip) { return NativeInvoke::Invoke<Vehicle>("GET_BLIP_INFO_ID_CAR_INDEX", blip); }
    static unsigned int GetBlipInfoIdDisplay(Blip blip) { return NativeInvoke::Invoke<bool>("GET_BLIP_INFO_ID_DISPLAY", blip); }
    static Object GetBlipInfoIdObjectIndex(Blip blip) { return NativeInvoke::Invoke<Object>("GET_BLIP_INFO_ID_OBJECT_INDEX", blip); }
    static Ped GetBlipInfoIdPedIndex(Blip blip) { return NativeInvoke::Invoke<Ped>("GET_BLIP_INFO_ID_PED_INDEX", blip); }
    static Pickup GetBlipInfoIdPickupIndex(Blip blip) { return NativeInvoke::Invoke<Pickup>("GET_BLIP_INFO_ID_PICKUP_INDEX", blip); }
    static const char *GetBlipName(Blip blip) { return NativeInvoke::Invoke<const char *>("GET_BLIP_NAME", blip); }
    static eBlipSprite GetBlipSprite(Blip blip) { return NativeInvoke::Invoke<eBlipSprite>("GET_BLIP_SPRITE", blip); }
    static Blip GetFirstBlipInfoId(eBlipSprite type) { return NativeInvoke::Invoke<Blip>("GET_FIRST_BLIP_INFO_ID", type); }
    static Blip GetNextBlipInfoId(eBlipSprite type) { return NativeInvoke::Invoke<Blip>("GET_NEXT_BLIP_INFO_ID", type); }
    static bool IsBlipShortRange(Blip blip) { return NativeInvoke::Invoke<bool>("IS_BLIP_SHORT_RANGE", blip); }
    static void RemoveBlip(Blip blip) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_BLIP", blip); }
    static void SetBlipAsFriendly(Blip blip, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_BLIP_AS_FRIENDLY", blip, value); }
    static void SetBlipAsShortRange(Blip blip, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_BLIP_AS_SHORT_RANGE", blip, value); }
    static void SetRoute(Blip blip, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_ROUTE", blip, value); }

    // Pickups
    static void AddPickupToInteriorRoomByName(Pickup pickup, const char *roomName) { NativeInvoke::Invoke<ScriptVoid>("ADD_PICKUP_TO_INTERIOR_ROOM_BY_NAME", pickup, roomName); }
    static void CreateMoneyPickup(float x, float y, float z, unsigned int amount, bool unknownTrue, Pickup *pPickup) { NativeInvoke::Invoke<ScriptVoid>("CREATE_MONEY_PICKUP", x, y, z, amount, unknownTrue, pPickup); }
    static void CreatePickup(eModel model, ePickupType pickupType, float x, float y, float z, Pickup *pPickup, bool unknownFalse) { NativeInvoke::Invoke<ScriptVoid>("CREATE_PICKUP", model, pickupType, x, y, z, pPickup, unknownFalse); }
    static void CreatePickupRotate(eModel model, ePickupType pickupType, unsigned int unknown, float x, float y, float z, float rX, float rY, float rZ, Pickup *pPickup) { NativeInvoke::Invoke<ScriptVoid>("CREATE_PICKUP_ROTATE", model, pickupType, unknown, x, y, z, rX, rY, rZ, pPickup); }
    static void CreatePickupWithAmmo(eModel model, ePickupType pickupType, unsigned int unknown, float x, float y, float z, Pickup *pPickup) { NativeInvoke::Invoke<ScriptVoid>("CREATE_PICKUP_WITH_AMMO", model, pickupType, unknown, x, y, z, pPickup); }
    static bool DoesPickupExist(Pickup pickup) { return NativeInvoke::Invoke<bool>("DOES_PICKUP_EXIST", pickup); }
    static void GetPickupCoordinates(Pickup pickup, float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_PICKUP_COORDINATES", pickup, pX, pY, pZ); }
    static bool HasPickupBeenCollected(Pickup pickup) { return NativeInvoke::Invoke<bool>("HAS_PICKUP_BEEN_COLLECTED", pickup); }
    static void PickupsPassTime(unsigned int time) { NativeInvoke::Invoke<ScriptVoid>("PICKUPS_PASS_TIME", time); }
    static void RemovePickup(Pickup pickup) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_PICKUP", pickup); }
    static void RemoveTemporaryRadarBlipsForPickups() { NativeInvoke::Invoke<ScriptVoid>("REMOVE_TEMPORARY_RADAR_BLIPS_FOR_PICKUPS"); }

    // Camera
    static bool CamIsSphereVisible(Camera camera, float pX, float pY, float pZ, float radius) { return NativeInvoke::Invoke<bool>("CAM_IS_SPHERE_VISIBLE", camera, pX, pY, pZ, radius); }
    static void CreateCam(unsigned int camtype_usually14, Camera *camera) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CAM", camtype_usually14, camera); }
    static void DestroyCam(Camera camera) { NativeInvoke::Invoke<ScriptVoid>("DESTROY_CAM", camera); }
    static bool DoesCamExist(Camera camera) { return NativeInvoke::Invoke<bool>("DOES_CAM_EXIST", camera); }
    static void SetCamActive(Camera camera, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_ACTIVE", camera, value); }
    static void GetCamFov(Camera camera, float *fov) { NativeInvoke::Invoke<ScriptVoid>("GET_CAM_FOV", camera, fov); }
    static void GetCamPos(Camera camera,  float *pX, float *pY, float *pZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CAM_POS", camera, pX, pY, pZ); }
    static void GetCamRot(Camera camera, float *angleX, float *angleY, float *angleZ) { NativeInvoke::Invoke<ScriptVoid>("GET_CAM_ROT", camera, angleX, angleY, angleZ); }
    static void GetGameCam(Camera *camera) { NativeInvoke::Invoke<ScriptVoid>("GET_GAME_CAM", camera); }
    static void GetGameCamChild(Camera *camera) { NativeInvoke::Invoke<ScriptVoid>("GET_GAME_CAM_CHILD", camera); }
    static bool IsCamActive(Camera camera) { return NativeInvoke::Invoke<bool>("IS_CAM_ACTIVE", camera); }
    static bool IsCamInterpolating() { return NativeInvoke::Invoke<bool>("IS_CAM_INTERPOLATING"); }
    static bool IsCamPropagating(Camera camera) { return NativeInvoke::Invoke<bool>("IS_CAM_PROPAGATING", camera); }
    static void SetCamBehindPed(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_BEHIND_PED", ped); }
    static void SetCamFov(Camera camera, float fov) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_FOV", camera, fov); }
    static void SetCamInFrontOfPed(Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_IN_FRONT_OF_PED", ped); }
    static void SetCamPos(Camera camera, float pX, float pY, float pZ) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_POS", camera, pX, pY, pZ); }
    static void SetCamPropagate(Camera camera, bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_PROPAGATE", camera, value); }
    static void SetCamRot(Camera camera, float angleX, float angleY, float angleZ) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_ROT", camera, angleX, angleY, angleZ); }
    static void SetCamTargetPed(Camera camera, Ped ped) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_TARGET_PED", camera, ped); }

    // Network
    static bool NetworkIsGameRanked() { return NativeInvoke::Invoke<bool>("NETWORK_IS_GAME_RANKED"); }
    static bool NetworkIsSessionStarted() { return NativeInvoke::Invoke<bool>("NETWORK_IS_SESSION_STARTED"); }

    // World
    static bool AreAllNavmeshRegionsLoaded() { return NativeInvoke::Invoke<bool>("ARE_ALL_NAVMESH_REGIONS_LOADED"); }
    static void ClearArea(float x, float y, float z, float radius, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_AREA", x, y, z, radius, unknown); }
    static void ClearAreaOfCars(float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_AREA_OF_CARS", x, y, z, radius); }
    static void ClearAreaOfChars(float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_AREA_OF_CHARS", x, y, z, radius); }
    static void ClearAreaOfCops(float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_AREA_OF_COPS", x, y, z, radius); }
    static void ClearAreaOfObjects(float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_AREA_OF_OBJECTS", x, y, z, radius); }
    static void ClearPedNonCreationArea() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_PED_NON_CREATION_AREA"); }
    static void ClearPedNonRemovalArea() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_PED_NON_REMOVAL_AREA"); }
    static void ExtinguishFireAtPoint(float x, float y, float z, float radius) { NativeInvoke::Invoke<ScriptVoid>("EXTINGUISH_FIRE_AT_POINT", x, y, z, radius); }
    static void ForceWeather(eWeather weather) { NativeInvoke::Invoke<ScriptVoid>("FORCE_WEATHER", weather); }
    static void ForceWeatherNow(eWeather weather) { NativeInvoke::Invoke<ScriptVoid>("FORCE_WEATHER_NOW", weather); }
    static void ForceWind(float wind) { NativeInvoke::Invoke<ScriptVoid>("FORCE_WIND", wind); }
    static void FreezePositionOfClosestObjectOfType(float x, float y, float z, float radius, eModel model, bool frozen) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_POSITION_OF_CLOSEST_OBJECT_OF_TYPE", x, y, z, radius, model, frozen); }
    static Vehicle GetClosestCar(float x, float y, float z, float radius, bool unknownFalse, unsigned int unknown70) { return NativeInvoke::Invoke<Vehicle>("GET_CLOSEST_CAR", x, y, z, radius, unknownFalse, unknown70); }
    static bool GetClosestChar(float x, float y, float z, float radius, bool unknown1, bool unknown2, Ped *pPed) { return NativeInvoke::Invoke<bool>("GET_CLOSEST_CHAR", x, y, z, radius, unknown1, unknown2, pPed); }
    static void GetCurrentWeather(eWeather *pWeather) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_WEATHER", pWeather); }
    static ScriptAny GetGroundZFor3DCoord(float x, float y, float z, float *pGroundZ) { return NativeInvoke::Invoke<ScriptAny>("GET_GROUND_Z_FOR_3D_COORD", x, y, z, pGroundZ); }
    static unsigned int GetNumberOfFiresInRange(float x, float y, float z, float radius) { return NativeInvoke::Invoke<unsigned int>("GET_NUMBER_OF_FIRES_IN_RANGE", x, y, z, radius); }
    static void GetSafePickupCoords(float x, float y, float z, float *pSafeX, float *pSafeY, float *pSafeZ) { NativeInvoke::Invoke<ScriptVoid>("GET_SAFE_PICKUP_COORDS", x, y, z, pSafeX, pSafeY, pSafeZ); }
    static bool GetSafePositionForChar(float x, float y, float z, bool unknownTrue, float *pSafeX, float *pSafeY, float *pSafeZ) { return NativeInvoke::Invoke<bool>("GET_SAFE_POSITION_FOR_CHAR", x, y, z, unknownTrue, pSafeX, pSafeY, pSafeZ); }
    static bool HasClosestObjectOfTypeBeenDamagedByChar(float x, float y, float z, float radius, eModel objectModel, Ped ped) { return NativeInvoke::Invoke<bool>("HAS_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED_BY_CHAR", x, y, z, radius, objectModel, ped); }
    static bool IsAreaOccupied(float x1, float y1, float z1, float x2, float y2, float z2, bool unknownFalse1, bool unknownTrue, bool unknownFalse2, bool unknownFalse3, bool unknownFalse4) { return NativeInvoke::Invoke<bool>("IS_AREA_OCCUPIED", x1, y1, z1, x2, y2, z2, unknownFalse1, unknownTrue, unknownFalse2, unknownFalse3, unknownFalse4); }
    static bool IsBulletInArea(float x, float y, float z, float radius, bool unknownTrue) { return NativeInvoke::Invoke<bool>("IS_BULLET_IN_AREA", x, y, z, radius, unknownTrue); }
    static bool IsBulletInBox(float x1, float y1, float z1, float x2, float y2, float z2, bool unknown) { return NativeInvoke::Invoke<bool>("IS_BULLET_IN_BOX", x1, y1, z1, x2, y2, z2, unknown); }
    static bool IsPointObscuredByAMissionEntity(float pX, float pY, float pZ, float sizeX, float sizeY, float sizeZ) { return NativeInvoke::Invoke<bool>("IS_POINT_OBSCURED_BY_A_MISSION_ENTITY", pX, pY, pZ, sizeX, sizeY, sizeZ); }
    static void LoadScene(float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("LOAD_SCENE", x, y, z); }
    static void RemoveAllPickupsOfType(ePickupType type) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_ALL_PICKUPS_OF_TYPE", type); }
    static void RequestCollisionAtPosn(float x, float y, float z) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_COLLISION_AT_POSN", x, y, z); }
	static void SetCreateRandomCops(bool en) { NativeInvoke::Invoke<ScriptVoid>("SET_CREATE_RANDOM_COPS", en); }
	static void SetNoResprays(bool no) { NativeInvoke::Invoke<ScriptVoid>("SET_NO_RESPRAYS", no); }
	static void SwitchAmbientPlanes(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_AMBIENT_PLANES", on); }	
    static void SwitchArrowAboveBlippedPickups(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_ARROW_ABOVE_BLIPPED_PICKUPS", on); }
    static void SwitchRandomBoats(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_RANDOM_BOATS", on); }
    static void SwitchRandomTrains(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_RANDOM_TRAINS", on); }
    static void SwitchGarbageTrucks(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_GARBAGE_TRUCKS", on); }
    static void SwitchMadDrivers(bool on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_MAD_DRIVERS", on); }

    // Garages
    static void CloseGarage(const char *garageName) { NativeInvoke::Invoke<ScriptVoid>("CLOSE_GARAGE", garageName); }
    static bool IsCarInGarageArea(const char *garageName, Vehicle vehicle) { return NativeInvoke::Invoke<bool>("IS_CAR_IN_GARAGE_AREA", garageName, vehicle); }
    static ScriptAny IsGarageClosed(const char *garageName) { return NativeInvoke::Invoke<ScriptAny>("IS_GARAGE_CLOSED", garageName); }
    static ScriptAny IsGarageOpen(const char *garageName) { return NativeInvoke::Invoke<ScriptAny>("IS_GARAGE_OPEN", garageName); }
    static void OpenGarage(const char *name) { NativeInvoke::Invoke<ScriptVoid>("OPEN_GARAGE", name); }

    // Text (GXT stuff)
    static void DisplayText(float x, float y, const char *gxtName) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT", x, y, gxtName); }
    static void DisplayTextWith2Numbers(float x, float y, const char *gxtName, int number1, int number2) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_2_NUMBERS", x, y, gxtName, number1, number2); }
    static void DisplayTextWithFloat(float x, float y, const char *gxtName, float value, unsigned int unknown) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_FLOAT", x, y, gxtName, value, unknown); }
    static void DisplayTextWithLiteralString(float x, float y, const char *gxtName, const char *literalStr) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_LITERAL_STRING", x, y, gxtName, literalStr); }
    static void DisplayTextWithNumber(float x, float y, const char *gxtName, int value) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_NUMBER", x, y, gxtName, value); }
    static void DisplayTextWithString(float x, float y, const char *gxtName, const char *gxtStringName) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_STRING", x, y, gxtName, gxtStringName); }
    static void DisplayTextWithTwoLiteralStrings(float x, float y, const char *gxtName, const char *literalStr1, const char *literalStr2) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_TWO_LITERAL_STRINGS", x, y, gxtName, literalStr1, literalStr2); }
    static void DisplayTextWithTwoStrings(float x, float y, const char *gxtName, const char *gxtStringName1, const char *gxtStringName2) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_TEXT_WITH_TWO_STRINGS", x, y, gxtName, gxtStringName1, gxtStringName2); }
    static unsigned int GetLengthOfStringWithThisTextLabel(const char *gxtName) { return NativeInvoke::Invoke<unsigned int>("GET_LENGTH_OF_STRING_WITH_THIS_TEXT_LABEL", gxtName); }
    static const char *GetFirstNCharactersOfString(const char *gxtName, unsigned int chars) { return NativeInvoke::Invoke<const char *>("GET_FIRST_N_CHARACTERS_OF_STRING", gxtName, chars); }
    static void GetMobilePhoneRenderId(int *pRenderId) { NativeInvoke::Invoke<ScriptVoid>("GET_MOBILE_PHONE_RENDER_ID", pRenderId); }
    static const char *GetNthIntegerInString(const char *gxtName, unsigned int index) { return NativeInvoke::Invoke<const char *>("GET_NTH_INTEGER_IN_STRING", gxtName, index); }
    static void GetScriptRenderTargetRenderId(int *pRenderId) { NativeInvoke::Invoke<ScriptVoid>("GET_SCRIPT_RENDERTARGET_RENDER_ID", pRenderId); }
    static const char *GetStringFromHashKey(unsigned int hashKey) { return NativeInvoke::Invoke<const char *>("GET_STRING_FROM_HASH_KEY", hashKey); }
    static ScriptAny GetStringWidth(const char *gxtName) { return NativeInvoke::Invoke<ScriptAny>("GET_STRING_WIDTH", gxtName); }
    static ScriptAny GetStringWidthWithNumber(const char *gxtName, int number) { return NativeInvoke::Invoke<ScriptAny>("GET_STRING_WIDTH_WITH_NUMBER", gxtName, number); }
    static ScriptAny GetStringWidthWithString(const char *gxtName, const char *literalString) { return NativeInvoke::Invoke<ScriptAny>("GET_STRING_WIDTH_WITH_STRING", gxtName, literalString); }
    static bool HasAdditionalTextLoaded(unsigned int textIndex) { return NativeInvoke::Invoke<bool>("HAS_ADDITIONAL_TEXT_LOADED", textIndex); }
    static bool HasThisAdditionalTextLoaded(const char *textName, unsigned int textIndex) { return NativeInvoke::Invoke<bool>("HAS_THIS_ADDITIONAL_TEXT_LOADED", textName, textIndex); }
    static bool IsFontLoaded(eTextFont font) { return NativeInvoke::Invoke<bool>("IS_FONT_LOADED", font); }
    static bool IsStreamingAdditionalText(unsigned int textIndex) { return NativeInvoke::Invoke<bool>("IS_STREAMING_ADDITIONAL_TEXT", textIndex); }
    static void LoadAdditionalText(const char *textName, unsigned int textIndex) { NativeInvoke::Invoke<ScriptVoid>("LOAD_ADDITIONAL_TEXT", textName, textIndex); }
    static void LoadTextFont(eTextFont font) { NativeInvoke::Invoke<ScriptVoid>("LOAD_TEXT_FONT", font); }
    static void PrintStringWithLiteralStringNow(const char* gxt, const char* text, unsigned int ms, unsigned int unknown1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_STRING_WITH_LITERAL_STRING_NOW", gxt, text, ms, unknown1); }
    static void RequestAdditionalText(const char *textName, unsigned int textIndex) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_ADDITIONAL_TEXT", textName, textIndex); }
    static void SetTextScale(float w, float h) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_SCALE", w, h); }
    static void SetTextBackground(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_BACKGROUND", value); }
    static void SetTextCentre(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_CENTRE", value); }
    static void SetTextColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_COLOUR", r, g, b, a); }
    static void SetTextDrawBeforeFade(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_DRAW_BEFORE_FADE", value); }
    static void SetTextDropshadow(bool displayShadow, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_DROPSHADOW", displayShadow, r, g, b, a); }
    static void SetTextEdge(bool displayEdge, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_EDGE", displayEdge, r, g, b, a); }
    static void SetTextFont(eTextFont font) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_FONT", font); }
    static void SetTextJustify(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_JUSTIFY", value); }
    static void SetTextLineDisplay(unsigned int unk1, unsigned int unk2) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_LINE_DISPLAY", unk1, unk2); }
    static void SetTextLineHeightMult(float lineHeight) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_LINE_HEIGHT_MULT", lineHeight); }
    static void SetTextProportional(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_PROPORTIONAL", value); }
    static void SetTextRenderId(unsigned int renderId) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_RENDER_ID", renderId); }
    static void SetTextRightJustify(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_RIGHT_JUSTIFY", value); }
    static void SetTextToUseTextFileColours(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_TO_USE_TEXT_FILE_COLOURS", value); }
    static void SetTextUseUnderscore(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_USE_UNDERSCORE", value); }
    static void SetTextWrap(float unk1, float unk2) { NativeInvoke::Invoke<ScriptVoid>("SET_TEXT_WRAP", unk1, unk2); }

    // Textures
    static Texture GetTexture(TextureDict dictionary, const char *textureName) { return NativeInvoke::Invoke<Texture>("GET_TEXTURE", dictionary, textureName); }
    static Texture GetTextureFromStreamedTxd(const char *txdName, const char *textureName) { return NativeInvoke::Invoke<Texture>("GET_TEXTURE_FROM_STREAMED_TXD", txdName, textureName); }
    static TextureDict GetTxd(const char *txdName) { return NativeInvoke::Invoke<TextureDict>("GET_TXD", txdName); }
    static bool HasStreamedTxdLoaded(const char *txdName) { return NativeInvoke::Invoke<bool>("HAS_STREAMED_TXD_LOADED", txdName); }
    static TextureDict LoadTxd(const char *txdName) { return NativeInvoke::Invoke<TextureDict>("LOAD_TXD", txdName); }
    static void MarkStreamedTxdAsNoLongerNeeded(const char *txdName) { NativeInvoke::Invoke<ScriptVoid>("MARK_STREAMED_TXD_AS_NO_LONGER_NEEDED", txdName); }
    static void ReleaseTexture(Texture texture) { NativeInvoke::Invoke<ScriptVoid>("RELEASE_TEXTURE", texture); }
    static void RequestStreamedTxd(const char *txdName, bool unknown) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_STREAMED_TXD", txdName, unknown); }
    static void RemoveTxd(TextureDict txd) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_TXD", txd); }

    // Stats
    static void DecrementIntStat(eIntStatistic stat, unsigned int amount) { NativeInvoke::Invoke<ScriptVoid>("DECREMENT_INT_STAT", stat, amount); }
    static unsigned int GetIntStat(eIntStatistic stat) { return NativeInvoke::Invoke<unsigned int>("GET_INT_STAT", stat); }
    static void IncrementFloatStatNoMessage(eFloatStatistic stat, float value) { NativeInvoke::Invoke<ScriptVoid>("INCREMENT_FLOAT_STAT_NO_MESSAGE", stat, value); }
    static void IncrementIntStat(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("INCREMENT_INT_STAT", stat, value); }
    static void IncrementIntStatNoMessage(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("INCREMENT_INT_STAT_NO_MESSAGE", stat, value); }
    static void SetIntStat(eIntStatistic stat, unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SET_INT_STAT", stat, value); }

    // Pad (controls)
    static bool GetPadPitchRoll(unsigned int padIndex, float *pPitch, float *pRoll) { return NativeInvoke::Invoke<bool>("GET_PAD_PITCH_ROLL", padIndex, pPitch, pRoll); }
    static void GetPositionOfAnalogueSticks(unsigned int padIndex, int *pLeftX, int *pLeftY, int *pRightX, int *pRightY) { NativeInvoke::Invoke<ScriptVoid>("GET_POSITION_OF_ANALOGUE_STICKS", padIndex, pLeftX, pLeftY, pRightX, pRightY); }
    static bool IsButtonJustPressed(unsigned int padIndex, ePadButton button) { return NativeInvoke::Invoke<bool>("IS_BUTTON_JUST_PRESSED", padIndex, button); }
    static bool IsButtonPressed(unsigned int padIndex, ePadButton button) { return NativeInvoke::Invoke<bool>("IS_BUTTON_PRESSED", padIndex, button); }

    // Sound
    static eSound GetSoundId() { return NativeInvoke::Invoke<eSound>("GET_SOUND_ID"); }
    static bool HasSoundFinished(eSound sound) { return NativeInvoke::Invoke<bool>("HAS_SOUND_FINISHED", sound); }
    static void PlayAudioEvent(char *name) { NativeInvoke::Invoke<ScriptVoid>("PLAY_AUDIO_EVENT", name); }
    static void PlaySoundFrontend(eSound sound, char *soundName) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND_FRONTEND", sound, soundName); }
    static void SetVariableOnSound(eSound sound, char *varname, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_VARIABLE_ON_SOUND", sound, varname, value); }
    static void StopSound(eSound sound) { NativeInvoke::Invoke<ScriptVoid>("STOP_SOUND", sound); }
    static void ReleaseSoundId(eSound sound) { NativeInvoke::Invoke<ScriptVoid>("RELEASE_SOUND_ID", sound); }

    // Time
    static void ForceTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<ScriptVoid>("FORCE_TIME_OF_DAY", hour, minute); }
    static void ForwardToTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<ScriptVoid>("FORWARD_TO_TIME_OF_DAY", hour, minute); }
    static void GetCurrentDate(int *day, int *month) { NativeInvoke::Invoke<ScriptVoid>("GET_CURRENT_DATE", day, month); }
    static unsigned int GetCurrentDayOfWeek() { return NativeInvoke::Invoke<unsigned int>("GET_CURRENT_DAY_OF_WEEK"); }
    static unsigned int GetHoursOfDay() { return NativeInvoke::Invoke<unsigned int>("GET_HOURS_OF_DAY"); }
    static unsigned int GetMinutesOfDay() { return NativeInvoke::Invoke<unsigned int>("GET_MINUTES_OF_DAY"); }
    static unsigned int GetMinutesToTimeOfDay(unsigned int hour, unsigned int minute) { return NativeInvoke::Invoke<unsigned int>("GET_MINUTES_TO_TIME_OF_DAY", hour, minute); }
    static void GetTimeOfDay(int *hour, int *minute) { NativeInvoke::Invoke<ScriptVoid>("GET_TIME_OF_DAY", hour, minute); }
    static void ReleaseTimeOfDay() { NativeInvoke::Invoke<ScriptVoid>("RELEASE_TIME_OF_DAY"); }
    static void SetTimeOfDay(unsigned int hour, unsigned int minute) { NativeInvoke::Invoke<ScriptVoid>("SET_TIME_OF_DAY", hour, minute); }
    static void SetTimeOneDayBack() { NativeInvoke::Invoke<ScriptVoid>("SET_TIME_ONE_DAY_BACK"); }
    static void SetTimeOneDayForward() { NativeInvoke::Invoke<ScriptVoid>("SET_TIME_ONE_DAY_FORWARD"); }
    static void SetTimeScale(float scale) { NativeInvoke::Invoke<ScriptVoid>("SET_TIME_SCALE", scale); }

    // Fires
    static bool IsScriptFireExtinguished(unsigned int fire) { return NativeInvoke::Invoke<bool>("IS_SCRIPT_FIRE_EXTINGUISHED", fire); }
    static void RemoveScriptFire(unsigned int fire) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_SCRIPT_FIRE", fire); }
    static unsigned int StartCarFire(Vehicle vehicle) { return NativeInvoke::Invoke<ScriptAny>("START_CAR_FIRE", vehicle); }
    static unsigned int StartCharFire(Ped ped) { return NativeInvoke::Invoke<ScriptAny>("START_CHAR_FIRE", ped); }
    static unsigned int StartScriptFire(float x, float y, float z, unsigned char numGenerationsAllowed = 0, unsigned int strength = 1) { return NativeInvoke::Invoke<unsigned int>("START_SCRIPT_FIRE", x, y, z, numGenerationsAllowed, strength); }

    // Radio
    static void DisableFrontEndRadio() { NativeInvoke::Invoke<ScriptVoid>("DISABLE_FRONTEND_RADIO"); }
    static void EnableFrontEndRadio() { NativeInvoke::Invoke<ScriptVoid>("ENABLE_FRONTEND_RADIO"); }
    static void FreezeRadioStation(const char *stationName) { NativeInvoke::Invoke<ScriptVoid>("FREEZE_RADIO_STATION", stationName); }
    static eRadioStation GetPlayerRadioStationIndex() { return NativeInvoke::Invoke<eRadioStation>("GET_PLAYER_RADIO_STATION_INDEX"); }
    static const char *GetPlayerRadioStationName() { return NativeInvoke::Invoke<const char *>("GET_PLAYER_RADIO_STATION_NAME"); }
    static bool IsRadioRetuning() { return NativeInvoke::Invoke<bool>("IS_RADIO_RETUNING"); }
    static void RetuneRadioToStationIndex(eRadioStation radioStation) { NativeInvoke::Invoke<ScriptVoid>("RETUNE_RADIO_TO_STATION_INDEX", radioStation); }

    // Game/Misc
    static void AbortAllGarageActivity() { NativeInvoke::Invoke<ScriptVoid>("ABORT_ALL_GARAGE_ACTIVITY"); }
    static void ActivateCheat(eCheat cheat) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_CHEAT", cheat); }
    static void ActivateFrontEnd() { NativeInvoke::Invoke<unsigned int>("ACTIVATE_FRONTEND"); }
    static void ActivateSaveMenu() { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_SAVE_MENU"); }
    static void AddExplosion(float x, float y, float z, unsigned int p3, float p4, unsigned int p5, unsigned int p6, float p7) { NativeInvoke::Invoke<ScriptVoid>("ADD_EXPLOSION", x, y, z, p3, p4, p5, p6, p7); }
    static void AllowEmergencyServices(bool allow) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_EMERGENCY_SERVICES", allow); }
    static void AllowGameToPauseForStreaming(bool allow) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_GAME_TO_PAUSE_FOR_STREAMING", allow); }
    static void AllowStuntJumpsToTrigger(bool allow) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_STUNT_JUMPS_TO_TRIGGER", allow); }
    static bool AreWidescreenBordersActive() { return NativeInvoke::Invoke<bool>("ARE_WIDESCREEN_BORDERS_ACTIVE"); }
    static ScriptAny AwardAchievement(eAchievement achievement) { return NativeInvoke::Invoke<ScriptAny>("AWARD_ACHIEVEMENT", achievement); }
    static bool CanPhoneBeSeenOnScreen() { return NativeInvoke::Invoke<bool>("CAN_PHONE_BE_SEEN_ON_SCREEN"); }
    static void CancelOverrideRestart() { NativeInvoke::Invoke<ScriptVoid>("CANCEL_OVERRIDE_RESTART"); }
    static void ClearShakePlayerPadWhenControllerDisabled() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_SHAKE_PLAYERPAD_WHEN_CONTROLLER_DISABLED"); }
    static void ClearTimeCycleModifier() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_TIMECYCLE_MODIFIER"); }
    static void DeactivateFrontEnd() { NativeInvoke::Invoke<ScriptVoid>("DEACTIVATE_FRONTEND"); }
    static bool DidSaveCompleteSuccessfully() { return NativeInvoke::Invoke<bool>("DID_SAVE_COMPLETE_SUCCESSFULLY"); }
    static void DisablePauseMenu(bool disabled) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_PAUSE_MENU", disabled); }
    static void DisablePoliceScanner() { NativeInvoke::Invoke<ScriptVoid>("DISABLE_POLICE_SCANNER"); }
    static void DisplayAmmo(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_AMMO", display); }
    static void DisplayAreaName(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_AREA_NAME", display); }
    static void DisplayCash(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_CASH", display); }
    static void DisplayFrontEndMapBlips(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_FRONTEND_MAP_BLIPS", display); }
    static void DisplayHUD(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_HUD", display); }
    static void DisplayRadar(bool display) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_RADAR", display); }
    static void DisplaySniperScopeThisFrame() { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_SNIPER_SCOPE_THIS_FRAME"); }
    static void DoAutoSave() { NativeInvoke::Invoke<ScriptVoid>("DO_AUTO_SAVE"); }
    static void DoScreenFadeIn(unsigned int timeMS) { NativeInvoke::Invoke<ScriptVoid>("DO_SCREEN_FADE_IN", timeMS); }
    static void DoScreenFadeInUnhacked(unsigned int timeMS) { NativeInvoke::Invoke<ScriptVoid>("DO_SCREEN_FADE_IN_UNHACKED", timeMS); }
    static void DoScreenFadeOut(unsigned int timeMS) { NativeInvoke::Invoke<ScriptVoid>("DO_SCREEN_FADE_OUT", timeMS); }
    static void DoScreenFadeOutUnhacked(unsigned int timeMS) { NativeInvoke::Invoke<ScriptVoid>("DO_SCREEN_FADE_OUT_UNHACKED", timeMS); }
	static void DrawRect(float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b, unsigned char a) { NativeInvoke::Invoke<ScriptVoid>("DRAW_RECT", x1, y1, x2, y2, r, g, b, a); }
    static void EnableMaxAmmoCap(bool enable) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_MAX_AMMO_CAP", enable); }
    static void EnablePoliceScanner() { NativeInvoke::Invoke<ScriptVoid>("ENABLE_POLICE_SCANNER"); }
    static void EnableSceneStreaming(bool enable) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_SCENE_STREAMING", enable); }
    static void FakeDeathArrest() { NativeInvoke::Invoke<ScriptVoid>("FAKE_DEATHARREST"); }
    static void FindStreetNameAtPosition(float pX, float pY, float pZ, int *strHash0, int *strHash1) { NativeInvoke::Invoke<ScriptVoid>("FIND_STREET_NAME_AT_POSITION", pX, pY, pZ, strHash0, strHash1); }
    static void FireSingleBullet(float x, float y, float z, float targetX, float targetY, float targetZ, unsigned int unknown) { NativeInvoke::Invoke<ScriptVoid>("FIRE_SINGLE_BULLET", x, y, z, targetX, targetY, targetZ, unknown); }
    static void FlashWeaponIcon(bool on) { NativeInvoke::Invoke<ScriptVoid>("FLASH_WEAPON_ICON", on); }
    static void ForceInitialPlayerStation(const char *stationName) { NativeInvoke::Invoke<ScriptVoid>("FORCE_INITIAL_PLAYER_STATION", stationName); }
    static void ForceLoadingScreen(bool force) { NativeInvoke::Invoke<ScriptVoid>("FORCE_LOADING_SCREEN", force); }
    static void GetCorrectedColour(unsigned int r, unsigned int g, unsigned int b, int *pR, int *pG, int *pB) { NativeInvoke::Invoke<ScriptVoid>("GET_CORRECTED_COLOUR", r, g, b, pR, pG, pB); }
    static bool GetCreateRandomCops() { return NativeInvoke::Invoke<bool>("GET_CREATE_RANDOM_COPS"); }
    static unsigned int GetCurrentEpisode() { return NativeInvoke::Invoke<unsigned int>("GET_CURRENT_EPISODE"); }
    static eLanguage GetCurrentLanguage() { return NativeInvoke::Invoke<eLanguage>("GET_CURRENT_LANGUAGE"); }
    static unsigned int GetEpisodeIndexFromSummons() { return NativeInvoke::Invoke<ScriptAny>("GET_EPISODE_INDEX_FROM_SUMMONS"); }
    static float GetFloatStat(eFloatStatistic stat) { return NativeInvoke::Invoke<float>("GET_FLOAT_STAT", stat); }
    static void GetFrameTime(float *time) { NativeInvoke::Invoke<ScriptVoid>("GET_FRAME_TIME", time); }
    static void GetGameTimer(int *pTimer) { NativeInvoke::Invoke<ScriptVoid>("GET_GAME_TIMER", pTimer); }
    static unsigned int GetHashKey(const char *value) { return NativeInvoke::Invoke<ScriptAny>("GET_HASH_KEY", value); }
    static void GetHUDColour(eHUDType type, int *pR, int *pG, int *pB, ScriptAny *pUnknown) { NativeInvoke::Invoke<ScriptVoid>("GET_HUD_COLOUR", type, pR, pG, pB, pUnknown); }
    static unsigned int GetIdOfThisThread() { return NativeInvoke::Invoke<unsigned int>("GET_ID_OF_THIS_THREAD"); }
    static bool GetIsDepositAnimRunning() { return NativeInvoke::Invoke<bool>("GET_IS_DEPOSIT_ANIM_RUNNING"); }
    static bool GetIsHiDef() { return NativeInvoke::Invoke<bool>("GET_IS_HIDEF"); }
    static bool GetIsWidescreen() { return NativeInvoke::Invoke<bool>("GET_IS_WIDESCREEN"); }
    static eMapArea GetMapAreaFromCoords(float x, float y, float z) { return NativeInvoke::Invoke<eMapArea>("GET_MAP_AREA_FROM_COORDS", x, y, z); }
    static bool GetMissionFlag() { return NativeInvoke::Invoke<bool>("GET_MISSION_FLAG"); }
    static void GetMaxWantedLevel(int *pMaxWantedLevel) { NativeInvoke::Invoke<ScriptVoid>("GET_MAX_WANTED_LEVEL", pMaxWantedLevel); }
    static const char *GetNameOfInfoZone(float x, float y, float z) { return NativeInvoke::Invoke<const char *>("GET_NAME_OF_INFO_ZONE", x, y, z); }
    static const char *GetNameOfZone(float x, float y, float z) { return NativeInvoke::Invoke<const char *>("GET_NAME_OF_ZONE", x, y, z); }
    static unsigned int GetNumStreamingRequests() { return NativeInvoke::Invoke<unsigned int>("GET_NUM_STREAMING_REQUESTS"); }
    static unsigned int GetNumberOfInstancesOfStreamedScript(const char *scriptName) { return NativeInvoke::Invoke<unsigned int>("GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT", scriptName); }
    static unsigned int GetNumberOfPlayers() { return NativeInvoke::Invoke<unsigned int>("GET_NUMBER_OF_PLAYERS"); }
    static unsigned int GetTimeSinceLastArrest() { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_LAST_ARREST"); }
    static unsigned int GetTimeSinceLastDeath() { return NativeInvoke::Invoke<unsigned int>("GET_TIME_SINCE_LAST_DEATH"); }
    static bool HasAchievementBeenPassed(eAchievement achievement) { return NativeInvoke::Invoke<bool>("HAS_ACHIEVEMENT_BEEN_PASSED", achievement); }
    static bool HasDeathArrestExecuted() { return NativeInvoke::Invoke<bool>("HAS_DEATHARREST_EXECUTED"); }
    static bool HasResprayHappened() { return NativeInvoke::Invoke<bool>("HAS_RESPRAY_HAPPENED"); }
    static bool HasScriptLoaded(const char *scriptName) { return NativeInvoke::Invoke<bool>("HAS_SCRIPT_LOADED", scriptName); }
    static bool HaveAnimsLoaded(const char *animName) { return NativeInvoke::Invoke<bool>("HAVE_ANIMS_LOADED", animName); }
    static bool HaveRequestedPathNodesBeenLoaded(unsigned int requestId) { return NativeInvoke::Invoke<bool>("HAVE_REQUESTED_PATH_NODES_BEEN_LOADED", requestId); }
    static void HideHelpTextThisFrame() { NativeInvoke::Invoke<ScriptVoid>("HIDE_HELP_TEXT_THIS_FRAME"); }
    static void HideHUDAndRadarThisFrame() { NativeInvoke::Invoke<ScriptVoid>("HIDE_HUD_AND_RADAR_THIS_FRAME"); }
	static bool IsControlJustPressed(unsigned int unk_0, unsigned int key) { return NativeInvoke::Invoke<bool>("IS_CONTROL_JUST_PRESSED", unk_0, key); }
	static bool IsControlPressed(unsigned int unk_0, unsigned int key) { return NativeInvoke::Invoke<bool>("IS_CONTROL_PRESSED", unk_0, key); }
    static bool IsAussieVersion() { return NativeInvoke::Invoke<bool>("IS_AUSSIE_VERSION"); }
    static bool IsAutoAimingOn() { return NativeInvoke::Invoke<bool>("IS_AUTO_AIMING_ON"); }
    static bool IsAutoSaveInProgress() { return NativeInvoke::Invoke<bool>("IS_AUTO_SAVE_IN_PROGRESS"); }
    static bool IsInteriorScene() { return NativeInvoke::Invoke<bool>("IS_INTERIOR_SCENE"); }
	static bool IsFollowVehicleCamOffsetActive() { return NativeInvoke::Invoke<bool>("IS_FOLLOW_VEHICLE_CAM_OFFSET_ACTIVE"); }
    static bool IsFrontEndFading() { return NativeInvoke::Invoke<bool>("IS_FRONTEND_FADING"); }
    static bool IsGameInControlOfMusic() { return NativeInvoke::Invoke<bool>("IS_GAME_IN_CONTROL_OF_MUSIC"); }
	static bool IsGameKeyboardKeyJustPressed(unsigned int key) { return NativeInvoke::Invoke<bool>("IS_GAME_KEYBOARD_KEY_JUST_PRESSED", key); }
	static bool IsGameKeyboardKeyPressed(unsigned int key) { return NativeInvoke::Invoke<bool>("IS_GAME_KEYBOARD_KEY_PRESSED", key); }
    static bool IsGermanVersion() { return NativeInvoke::Invoke<bool>("IS_GERMAN_VERSION"); }
    static bool IsHelpMessageBeingDisplayed() { return NativeInvoke::Invoke<bool>("IS_HELP_MESSAGE_BEING_DISPLAYED"); }
    static bool IsHintRunning() { return NativeInvoke::Invoke<bool>("IS_HINT_RUNNING"); }
    static bool IsHUDPreferenceSwitchedOn() { return NativeInvoke::Invoke<bool>("IS_HUD_PREFERENCE_SWITCHED_ON"); }
    static bool IsHUDReticuleComplex() { return NativeInvoke::Invoke<bool>("IS_HUD_RETICULE_COMPLEX"); }
    static bool IsInSpectatorMode() { return NativeInvoke::Invoke<bool>("IS_IN_SPECTATOR_MODE"); }
	static bool IsKeyboardKeyJustPressed(unsigned int key) { return NativeInvoke::Invoke<bool>("IS_KEYBOARD_KEY_JUST_PRESSED", key); }
	static bool IsKeyboardKeyPressed(unsigned int key) { return NativeInvoke::Invoke<bool>("IS_KEYBOARD_KEY_PRESSED", key); }
    static bool IsLookInverted() { return NativeInvoke::Invoke<bool>("IS_LOOK_INVERTED"); }
    static bool IsMemoryCardInUse() { return NativeInvoke::Invoke<bool>("IS_MEMORY_CARD_IN_USE"); }
    static bool IsMessageBeingDisplayed() { return NativeInvoke::Invoke<bool>("IS_MESSAGE_BEING_DISPLAYED"); }
    static bool IsMinigameInProgress() { return NativeInvoke::Invoke<bool>("IS_MINIGAME_IN_PROGRESS"); }
    static bool IsMissionCompletePlaying() { return NativeInvoke::Invoke<bool>("IS_MISSION_COMPLETE_PLAYING"); }
    static bool IsMobilePhoneCallOngoing() { return NativeInvoke::Invoke<bool>("IS_MOBILE_PHONE_CALL_ONGOING"); }
    static bool IsMobilePhoneRadioActive() { return NativeInvoke::Invoke<bool>("IS_MOBILE_PHONE_RADIO_ACTIVE"); }
    static bool IsPauseMenuActive() { return NativeInvoke::Invoke<bool>("IS_PAUSE_MENU_ACTIVE"); }
    static bool IsPayNSprayActive() { return NativeInvoke::Invoke<bool>("IS_PAY_N_SPRAY_ACTIVE"); }
    static bool IsPlayerBeingArrested() { return NativeInvoke::Invoke<bool>("IS_PLAYER_BEING_ARRESTED"); }
    static bool IsPlayerOnline() { return NativeInvoke::Invoke<bool>("IS_PLAYER_ONLINE"); }
    static bool IsScreenFadedIn() { return NativeInvoke::Invoke<bool>("IS_SCREEN_FADED_IN"); }
    static bool IsScreenFadedOut() { return NativeInvoke::Invoke<bool>("IS_SCREEN_FADED_OUT"); }
    static bool IsScreenFading() { return NativeInvoke::Invoke<bool>("IS_SCREEN_FADING"); }
    static bool IsScreenFadingIn() { return NativeInvoke::Invoke<bool>("IS_SCREEN_FADING_IN"); }
    static bool IsScreenFadingOut() { return NativeInvoke::Invoke<bool>("IS_SCREEN_FADING_OUT"); }
    static bool IsSniperInverted() { return NativeInvoke::Invoke<bool>("IS_SNIPER_INVERTED"); }
    static bool IsSystemUIShowing() { return NativeInvoke::Invoke<bool>("IS_SYSTEM_UI_SHOWING"); }
    static bool IsThisAMinigameScript() { return NativeInvoke::Invoke<bool>("IS_THIS_A_MINIGAME_SCRIPT"); }
    static bool IsThreadActive(unsigned int threadId) { return NativeInvoke::Invoke<bool>("IS_THREAD_ACTIVE", threadId); }
    static unsigned int LoadAllPathNodes(bool value) { return NativeInvoke::Invoke<ScriptAny>("LOAD_ALL_PATH_NODES", value); }
    static void MarkScriptAsNoLongerNeeded(const char *scriptName) { NativeInvoke::Invoke<ScriptVoid>("MARK_SCRIPT_AS_NO_LONGER_NEEDED", scriptName); }
    static void PauseGame() { NativeInvoke::Invoke<ScriptVoid>("PAUSE_GAME"); }
    static void PlayMovie() { NativeInvoke::Invoke<ScriptVoid>("PLAY_MOVIE"); }
    static void PopulateNow() { NativeInvoke::Invoke<ScriptVoid>("POPULATE_NOW"); }
    static void RequestAnims(const char *animName) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_ANIMS", animName); }
    static void ReleaseMovie() { NativeInvoke::Invoke<ScriptVoid>("RELEASE_MOVIE"); }
    static void ReleaseWeather() { NativeInvoke::Invoke<ScriptVoid>("RELEASE_WEATHER"); }
    static void RemoveAnims(const char *animName) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_ANIMS", animName); }
    static void RemoveCoverPoint(CoverPoint coverPoint) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_COVER_POINT", coverPoint); }
    static void RemoveIpl(const char *iplName) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_IPL", iplName); }
    static void RequestIpl(const char *iplName) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_IPL", iplName); }
    static void SetCameraControlsDisabledWithPlayerControls(bool value) { NativeInvoke::Invoke<ScriptVoid>("SET_CAMERA_CONTROLS_DISABLED_WITH_PLAYER_CONTROLS", value); }
    static void SetMissionFlag(bool isMission) { NativeInvoke::Invoke<ScriptVoid>("SET_MISSION_FLAG", isMission); }
    static void SetFloatStat(eFloatStatistic stat, float value) { NativeInvoke::Invoke<ScriptVoid>("SET_FLOAT_STAT", stat, value); }
	static void SetMaxWantedLevel(unsigned int max) { NativeInvoke::Invoke<ScriptVoid>("SET_MAX_WANTED_LEVEL", max); }
    static void SetRadarZoom(float zoom) { NativeInvoke::Invoke<ScriptVoid>("SET_RADAR_ZOOM", zoom); }
    static void SetTimerA(unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SETTIMERA", value); }
    static void SetTimerB(unsigned int value) { NativeInvoke::Invoke<ScriptVoid>("SETTIMERB", value); }
    static void SetWantedMultiplier(float multiplier) { NativeInvoke::Invoke<ScriptVoid>("SET_WANTED_MULTIPLIER", multiplier); }
	static void TerminateAllScriptsWithThisName(char * name) { NativeInvoke::Invoke<ScriptVoid>("TERMINATE_ALL_SCRIPTS_WITH_THIS_NAME", name); }
    static unsigned int TimerA() { return NativeInvoke::Invoke<unsigned int>("TIMERA"); }
    static unsigned int TimerB() { return NativeInvoke::Invoke<unsigned int>("TIMERB"); }
    static unsigned int TimeStep() { return NativeInvoke::Invoke<unsigned int>("TIMESTEP"); }
    static void UnlockLazlowStation() { NativeInvoke::Invoke<ScriptVoid>("UNLOCK_LAZLOW_STATION"); }
    static void UnregisterScriptWithAudio() { NativeInvoke::Invoke<ScriptVoid>("UNREGISTER_SCRIPT_WITH_AUDIO"); }
    static void UnPauseGame() { NativeInvoke::Invoke<ScriptVoid>("UNPAUSE_GAME"); }

	////////////////// NOT ////////////////////////
	// Wanted to cleanup, but couldn't find params

	static ScriptAny GetEpisodeName(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_EPISODE_NAME", p0); }
	static ScriptAny GetLeftPlayerCashToReachLevel(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_LEFT_PLAYER_CASH_TO_REACH_LEVEL", p0); }
	static ScriptAny GetNumberLinesWithSubstrings(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("GET_NUMBER_LINES_WITH_SUBSTRINGS", p0, p1, p2, p3, p4); }
	static bool HasPlayerCollectedPickup(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<bool>("HAS_PLAYER_COLLECTED_PICKUP", p0, p1); }
	static void RenderWeaponPickupsBigger(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RENDER_WEAPON_PICKUPS_BIGGER", p0); }


// Interesting ones to cleanup:

	static void GetRandomCarBackBumperInSphere(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CAR_BACK_BUMPER_IN_SPHERE", p0, p1, p2, p3, p4, p5, p6); }
	static void GetRandomCarFrontBumperInSphereNoSave(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CAR_FRONT_BUMPER_IN_SPHERE_NO_SAVE", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void GetRandomCarInSphere(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CAR_IN_SPHERE", p0, p1, p2, p3, p4, p5, p6); }
	static void GetRandomCarInSphereNoSave(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("GET_RANDOM_CAR_IN_SPHERE_NO_SAVE", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny GetStatFrontEndVisibility(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_STAT_FRONTEND_VISIBILITY", p0); }
	static ScriptAny GetStringFromString(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("GET_STRING_FROM_STRING", p0, p1, p2); }
	static ScriptAny GetTimeTilNextStation(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_TIME_TIL_NEXT_STATION", p0); }
	static void GetTrainCaboose(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_TRAIN_CABOOSE", p0, p1); }
	static void GetTrainCarriage(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GET_TRAIN_CARRIAGE", p0, p1, p2); }
	static void GetTrainPlayerWouldEnter(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_TRAIN_PLAYER_WOULD_ENTER", p0, p1); }
	static void GetVehicleQuaternion(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("GET_VEHICLE_QUATERNION", p0, p1, p2, p3, p4); }
	static ScriptAny GetWaterHeight(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("GET_WATER_HEIGHT", p0, p1, p2, p3); }
	static ScriptAny GetWaterHeightNoWaves(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("GET_WATER_HEIGHT_NO_WAVES", p0, p1, p2, p3); }
	static void GetWeaponTypeModel(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_WEAPONTYPE_MODEL", p0, p1); }
	static void GetWeaponTypeSlot(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_WEAPONTYPE_SLOT", p0, p1); }
	static void GiveDelayedWeaponToChar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("GIVE_DELAYED_WEAPON_TO_CHAR", p0, p1, p2, p3); }
	static void GivePedAmbientObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PED_AMBIENT_OBJECT", p0, p1); }
	static void GivePedHelmetWithOpts(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PED_HELMET_WITH_OPTS", p0, p1); }
	static void GivePedPickupObject(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PED_PICKUP_OBJECT", p0, p1, p2); }
	static void GivePlayerRagdollControl(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GIVE_PLAYER_RAGDOLL_CONTROL", p0, p1); }
	static void GrabNearbyObjectWithSpecialAttribute(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GRAB_NEARBY_OBJECT_WITH_SPECIAL_ATTRIBUTE", p0, p1); }

	static ScriptAny IsClosestObjectOfTypeSmashedOrDamaged(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("IS_CLOSEST_OBJECT_OF_TYPE_SMASHED_OR_DAMAGED", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny IsCopVehicleInArea3DNoSave(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("IS_COP_VEHICLE_IN_AREA_3D_NO_SAVE", p0, p1, p2, p3, p4, p5); }
	static ScriptAny IsEpisodeAvailable(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_EPISODE_AVAILABLE", p0); }
	static ScriptAny IsExplosionInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("IS_EXPLOSION_IN_AREA", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny IsExplosionInSphere(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("IS_EXPLOSION_IN_SPHERE", p0, p1, p2, p3, p4); }
	static ScriptAny IsHeliPartBroken(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("IS_HELI_PART_BROKEN", p0, p1, p2, p3); }

	static ScriptAny IsObjectInAngledArea3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("IS_OBJECT_IN_ANGLED_AREA_3D", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny IsObjectInArea3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("IS_OBJECT_IN_AREA_3D", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny IsObjectPlayingAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("IS_OBJECT_PLAYING_ANIM", p0, p1, p2); }
	static bool IsObjectReassignmentInProgress() { return NativeInvoke::Invoke<bool>("IS_OBJECT_REASSIGNMENT_IN_PROGRESS"); }

	static ScriptAny IsProjectileInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("IS_PROJECTILE_IN_AREA", p0, p1, p2, p3, p4, p5); }

	static ScriptAny IsSittingObjectNear(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("IS_SITTING_OBJECT_NEAR", p0, p1, p2, p3); }
	static ScriptAny IsSniperBulletInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("IS_SNIPER_BULLET_IN_AREA", p0, p1, p2, p3, p4, p5); }

	static ScriptAny IsVehStuck(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("IS_VEH_STUCK", p0, p1, p2, p3, p4); }

	static void Line(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("LINE", p0, p1, p2, p3, p4, p5); }

	static void LoadSceneForRoomByKey(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("LOAD_SCENE_FOR_ROOM_BY_KEY", p0, p1); }

	static ScriptAny LocateCar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CAR_2D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CAR_3D", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny LocateCharAnyMeans2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_2D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharAnyMeans3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_3D", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny LocateCharAnyMeansCar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_CAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharAnyMeansCar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_CAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharAnyMeansChar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_CHAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharAnyMeansChar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_CHAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharAnyMeansObject2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_OBJECT_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharAnyMeansObject3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ANY_MEANS_OBJECT_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharInCar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_2D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharInCar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_3D", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny LocateCharInCarCar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_CAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharInCarCar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_CAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharInCarChar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_CHAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharInCarChar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_CHAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharInCarObject3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_IN_CAR_OBJECT_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharOnFoot2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_2D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharOnFoot3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_3D", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny LocateCharOnFootCar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_CAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharOnFootCar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_CAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharOnFootChar2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_CHAR_2D", p0, p1, p2, p3, p4); }
	static ScriptAny LocateCharOnFootChar3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_CHAR_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateCharOnFootObject3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_CHAR_ON_FOOT_OBJECT_3D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateObject2D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_OBJECT_2D", p0, p1, p2, p3, p4, p5); }
	static ScriptAny LocateObject3D(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("LOCATE_OBJECT_3D", p0, p1, p2, p3, p4, p5, p6, p7); }

	static ScriptAny PlayObjectAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("PLAY_OBJECT_ANIM", p0, p1, p2, p3, p4, p5); }
	static void PlaySound(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND", p0, p1); }
	static void PlaySoundFromObject(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND_FROM_OBJECT", p0, p1, p2); }
	static void PlaySoundFromPed(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND_FROM_PED", p0, p1, p2); }
	static void PlaySoundFromPosition(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND_FROM_POSITION", p0, p1, p2, p3, p4); }
	static void PlaySoundFromVehicle(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PLAY_SOUND_FROM_VEHICLE", p0, p1, p2); }
	static void PlayStreamFromObject(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAY_STREAM_FROM_OBJECT", p0); }
	static void PlayStreamFromPed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAY_STREAM_FROM_PED", p0); }
	static void PlayStreamFrontEnd() { NativeInvoke::Invoke<ScriptVoid>("PLAY_STREAM_FRONTEND"); }

	static void RegisterIntStat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_INT_STAT", p0, p1); }
	static ScriptAny RegisterSaveHouse(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("REGISTER_SAVE_HOUSE", p0, p1, p2, p3, p4, p5); }

	static void RequestScript(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_SCRIPT", p0); }
	static void ResetCarWheels(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("RESET_CAR_WHEELS", p0, p1); }
	static void ResetStuckTimer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("RESET_STUCK_TIMER", p0, p1); }
	static void ResetVisiblePedDamage(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESET_VISIBLE_PED_DAMAGE", p0); }
	static void RetuneRadioDown() { NativeInvoke::Invoke<ScriptVoid>("RETUNE_RADIO_DOWN"); }
	static void RetuneRadioToStationName(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RETUNE_RADIO_TO_STATION_NAME", p0); }
	static void RetuneRadioUp() { NativeInvoke::Invoke<ScriptVoid>("RETUNE_RADIO_UP"); }
	static ScriptAny RotateObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("ROTATE_OBJECT", p0, p1, p2, p3); }
	static ScriptAny Round(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("ROUND", p0); }
	static void SetActivateObjectPhysicsAsSoonAsItIsUnfrozen(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ACTIVATE_OBJECT_PHYSICS_AS_SOON_AS_IT_IS_UNFROZEN", p0, p1); }
	static void SetAllCarsCanBeDamaged(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ALL_CARS_CAN_BE_DAMAGED", p0); }
	static void SetAllPickupsOfTypeCollectableByCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ALL_PICKUPS_OF_TYPE_COLLECTABLE_BY_CAR", p0, p1); }
	static void SetAllRandomPedsFlee(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ALL_RANDOM_PEDS_FLEE", p0, p1); }
	static void SetAlwaysDisplayWeaponPickupMessage(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ALWAYS_DISPLAY_WEAPON_PICKUP_MESSAGE", p0); }

	static void SetAnimGroupForChar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ANIM_GROUP_FOR_CHAR", p0, p1); }
	static void SetBlipMarkerLongDistance(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_BLIP_MARKER_LONG_DISTANCE", p0, p1); }
	static void SetBlipThrottleRandomly(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_BLIP_THROTTLE_RANDOMLY", p0, p1); }
//	static void SetCarAsMissionCar(Vehicle car) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_AS_MISSION_CAR", car); }
	static void SetCarCollision(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_COLLISION", p0, p1); }
	static void SetCarColourCombination(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_COLOUR_COMBINATION", p0, p1); }
	static void SetCarCoordinatesNoOffset(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_COORDINATES_NO_OFFSET", p0, p1, p2, p3); }
	static void SetCarDoorLatched(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_DOOR_LATCHED", p0, p1, p2, p3); }
	static void SetCarEngineOn(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_ENGINE_ON", p0, p1, p2); }
	static void SetCarHasBeenCreatedInInterior(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_HAS_BEEN_CREATED_IN_INTERIOR", p0); }
	static void SetCarInCutscene(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_IN_CUTSCENE", p0, p1); }
	static void SetCarLightMultiplier(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_LIGHT_MULTIPLIER", p0, p1); }
	static void SetCarLivery(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_LIVERY", p0, p1); }
	static void SetCarOnlyDamagedByPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_ONLY_DAMAGED_BY_PLAYER", p0, p1); }
	static void SetCarRandomRouteSeed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_RANDOM_ROUTE_SEED", p0, p1); }
	static void SetCharAllowedToDuck(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ALLOWED_TO_DUCK", p0, p1); }
	static void SetCharAngledDefensiveArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ANGLED_DEFENSIVE_AREA", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void SetCharAnimCurrentTime(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ANIM_CURRENT_TIME", p0, p1, p2, p3); }
	static void SetCharAnimSpeed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ANIM_SPEED", p0, p1, p2, p3); }
	static void SetCharCanSmashGlass(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CAN_SMASH_GLASS", p0, p1); }
	static void SetCharClimbAnimRate(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CLIMB_ANIM_RATE", p0, p1); }
	static void SetCharCollision(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_COLLISION", p0, p1); }
	static void SetCharCoordinatesDontWarpGang(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_COORDINATES_DONT_WARP_GANG", p0, p1, p2, p3); }
	static void SetCharCoordinatesNoOffset(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_COORDINATES_NO_OFFSET", p0, p1, p2, p3); }
	static void SetCharCurrentWeaponVisible(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_CURRENT_WEAPON_VISIBLE", p0, p1); }
	static void SetCharDiesInstantlyInWater(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DIES_INSTANTLY_IN_WATER", p0, p1); }
	static void SetCharDrownsInSinkingVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DROWNS_IN_SINKING_VEHICLE", p0, p1); }
	static void SetCharDrownsInWater(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_DROWNS_IN_WATER", p0, p1); }
	static ScriptAny SetCharDucking(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("SET_CHAR_DUCKING", p0, p1); }
	static void SetCharForceDieInCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_FORCE_DIE_IN_CAR", p0, p1); }
	static void SetCharGestureGroup(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_GESTURE_GROUP", p0, p1); }
	static void SetCharGetOutUpsideDownCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_GET_OUT_UPSIDE_DOWN_CAR", p0, p1); }
	static void SetCharMaxTimeInWater(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MAX_TIME_IN_WATER", p0, p1); }
	static void SetCharMaxTimeUnderwater(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MAX_TIME_UNDERWATER", p0, p1); }
	static void SetCharMeleeActionFlag0(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MELEE_ACTION_FLAG0", p0, p1); }
	static void SetCharMeleeActionFlag1(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MELEE_ACTION_FLAG1", p0, p1); }
	static void SetCharMeleeMovementConstaintBox(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MELEE_MOVEMENT_CONSTAINT_BOX", p0, p1, p2, p3, p4, p5, p6); }
	static void SetCharMoveAnimSpeedMultiplier(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_MOVE_ANIM_SPEED_MULTIPLIER", p0, p1); }
	static void SetCharNeverTargetted(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_NEVER_TARGETTED", p0, p1); }
	static void SetCharOnlyDamagedByPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ONLY_DAMAGED_BY_PLAYER", p0, p1); }
	static void SetCharReadyToBeExecuted(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_READY_TO_BE_EXECUTED", p0, p1); }
	static void SetCharReadyToBeStunned(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_READY_TO_BE_STUNNED", p0, p1); }
	static void SetCharRotation(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ROTATION", p0, p1, p2, p3); }
	static void SetCharShootRate(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_SHOOT_RATE", p0, p1); }
	static void SetCharStayInCarWhenJacked(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_STAY_IN_CAR_WHEN_JACKED", p0, p1); }
	static void SetCharUsesDeafultAnimGroupWhenFleeing(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_USES_DEAFULT_ANIM_GROUP_WHEN_FLEEING", p0, p1); }
	static void SetCharWeaponSkill(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WEAPON_SKILL", p0, p1); }
	static void SetCharWillCowerInsteadOfFleeing(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_COWER_INSTEAD_OF_FLEEING", p0, p1); }
	static void SetCharWillLeaveCarInCombat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_LEAVE_CAR_IN_COMBAT", p0, p1); }
	static void SetCharWillOnlyFireWithClearLos(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_ONLY_FIRE_WITH_CLEAR_LOS", p0, p1); }
	static void SetCharWillRemainOnBoatAfterMissionEnds(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_WILL_REMAIN_ON_BOAT_AFTER_MISSION_ENDS", p0, p1); }
	static void SetCollideWithPeds(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_COLLIDE_WITH_PEDS", p0); }
	static void SetCurrentMovie(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_CURRENT_MOVIE", p0); }
	static void SetDeadCharCoordinates(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_DEAD_CHAR_COORDINATES", p0, p1, p2, p3); }
	static void SetDeadPedsDropWeapons(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_DEAD_PEDS_DROP_WEAPONS", p0); }
	static void SetDitchPoliceModels(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_DITCH_POLICE_MODELS", p0); }
	static void SetDoNotSpawnParkedCarsOnTop(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_DO_NOT_SPAWN_PARKED_CARS_ON_TOP", p0, p1); }
	static void SetDontActivateRagdollFromPlayerImpact(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_DONT_ACTIVATE_RAGDOLL_FROM_PLAYER_IMPACT", p0, p1); }
	static void SetDrawPlayerComponent(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_DRAW_PLAYER_COMPONENT", p0, p1); }
	static void SetDrunkCam(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_DRUNK_CAM", p0, p1, p2); }
	static void SetEnableNearClipScan(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ENABLE_NEAR_CLIP_SCAN", p0); }
//	static void SetFakeWantedLevel(unsigned int lvl) { NativeInvoke::Invoke<ScriptVoid>("SET_FAKE_WANTED_LEVEL", lvl); }
	static void SetHasBeenOwnedByPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_HAS_BEEN_OWNED_BY_PLAYER", p0, p1); }

	static void SetHeliBladesFullSpeed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_HELI_BLADES_FULL_SPEED", p0); }	
	static void SetInstantWidescreenBorders(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_INSTANT_WIDESCREEN_BORDERS", p0); }
	static void SetInterpFromGameToScript(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_INTERP_FROM_GAME_TO_SCRIPT", p0, p1); }
	static void SetMinigameInProgress(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MINIGAME_IN_PROGRESS", p0); }
	static void SetMissionTrainCoordinates(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_MISSION_TRAIN_COORDINATES", p0, p1, p2, p3); }
	static void SetMobilePhonePosition(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_PHONE_POSITION", p0, p1, p2); }
	static void SetMobilePhoneRadioState(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_PHONE_RADIO_STATE", p0); }
	static void SetMobilePhoneRotation(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_PHONE_ROTATION", p0, p1, p2); }
	static void SetMobilePhoneScale(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_PHONE_SCALE", p0); }
	static void SetMobileRadioEnabledDuringGameplay(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY", p0); }
	static void SetMobileRingType(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOBILE_RING_TYPE", p0); }
	static void SetMoneyCarriedByAllNewPeds(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MONEY_CARRIED_BY_ALL_NEW_PEDS", p0); }

	static void SetMovieTime(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOVIE_TIME", p0); }
	static void SetMovieVolume(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MOVIE_VOLUME", p0); }
	static void SetNeedsToBeHotwired(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NEEDS_TO_BE_HOTWIRED", p0, p1); }
	static void SetNextDesiredMoveState(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_NEXT_DESIRED_MOVE_STATE", p0); }

//	static void SetObjectAlpha(Object obj, unsigned int alp) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_ALPHA", obj, alp); }
	static void SetObjectAnimCurrentTime(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_ANIM_CURRENT_TIME", p0, p1, p2, p3); }
	static void SetObjectAnimSpeed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_ANIM_SPEED", p0, p1, p2, p3); }
	static void SetObjectAsStealable(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_AS_STEALABLE", p0, p1); }
	static void SetObjectDynamic(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_DYNAMIC", p0, p1); }
	static void SetObjectHealth(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_HEALTH", p0, p1); }
	static void SetObjectInitialVelocity(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_INITIAL_VELOCITY", p0, p1, p2, p3); }
	static void SetObjectInvincible(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_INVINCIBLE", p0, p1); }
	static void SetObjectLights(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_LIGHTS", p0, p1); }
	static void SetObjectOnlyDamagedByPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_ONLY_DAMAGED_BY_PLAYER", p0, p1); }
	static void SetObjectPhysicsParams(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_PHYSICS_PARAMS", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static void SetObjectQuaternion(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_QUATERNION", p0, p1, p2, p3, p4); }
	static void SetObjectRotation(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_ROTATION", p0, p1, p2, p3); }
	static void SetObjectScale(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_SCALE", p0, p1); }
	static void SetObjectUsedInPoolGame(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_USED_IN_POOL_GAME", p0, p1); }
//	static void SetPedAlpha(Ped ped, unsigned int alp) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_ALPHA", ped, alp); }
	static void SetPedDontDoEvasiveDives(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_DONT_DO_EVASIVE_DIVES", p0, p1); }
	static void SetPedDontUseVehicleSpecificAnims(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_DONT_USE_VEHICLE_SPECIFIC_ANIMS", p0, p1); }
	static void SetPedEnableLegIK(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_ENABLE_LEG_IK", p0, p1); }
	static void SetPedGeneratesDeadBodyEvents(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_GENERATES_DEAD_BODY_EVENTS", p0, p1); }
	static void SetPedHeedsTheEveryoneIgnorePlayerFlag(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_HEEDS_THE_EVERYONE_IGNORE_PLAYER_FLAG", p0, p1); }
	static void SetPedNonCreationArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_NON_CREATION_AREA", p0, p1, p2, p3, p4, p5); }
	static void SetPedNonRemovalArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_NON_REMOVAL_AREA", p0, p1, p2, p3, p4, p5); }
	static void SetPedWontAttackPlayerWithoutWantedLevel(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_WONT_ATTACK_PLAYER_WITHOUT_WANTED_LEVEL", p0, p1); }
	static void SetPhoneHUDItem(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_PHONE_HUD_ITEM", p0, p1, p2); }
	static void SetPickupCollectableByCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PICKUP_COLLECTABLE_BY_CAR", p0, p1); }
	static void SetPickupsFixCars(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PICKUPS_FIX_CARS", p0); }

//	static void SetPlayerCanDropWeaponsInCar(bool can) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CAN_DROP_WEAPONS_IN_CAR", can); }
//	static void SetPlayerKeepsWeaponsWhenRespawned(bool keep) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_KEEPS_WEAPONS_WHEN_RESPAWNED", keep); }


	static void SetPoliceFocusWillTrackCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_POLICE_FOCUS_WILL_TRACK_CAR", p0, p1); }
	static void SetPoliceRadarBlips(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_POLICE_RADAR_BLIPS", p0); }

	static void SetRandomSeed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_RANDOM_SEED", p0); }
	static void SetReducePedModelBudget(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_REDUCE_PED_MODEL_BUDGET", p0); }
	static void SetReduceVehicleModelBudget(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_REDUCE_VEHICLE_MODEL_BUDGET", p0); }

	static void SetRenderTrainAsDerailed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_RENDER_TRAIN_AS_DERAILED", p0, p1); }
	static void SetRocketLauncherFreebieInHeli(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ROCKET_LAUNCHER_FREEBIE_IN_HELI", p0); }

	static void SetRoomForCarByName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ROOM_FOR_CAR_BY_NAME", p0, p1); }
	static void SetRoomForCharByName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ROOM_FOR_CHAR_BY_NAME", p0, p1); }
	static void SetRotOrder(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ROT_ORDER", p0); }
	static void SetRotationForAttachedPed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_ROTATION_FOR_ATTACHED_PED", p0, p1, p2, p3); }
	static void SetScreenFade(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { NativeInvoke::Invoke<ScriptVoid>("SET_SCREEN_FADE", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static void SetSirenWithNoDriver(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_SIREN_WITH_NO_DRIVER", p0, p1); }
//	static void SetSleepModeActive(bool act) { NativeInvoke::Invoke<ScriptVoid>("SET_SLEEP_MODE_ACTIVE", act); }
	static void SetSniperZoomFactor(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_SNIPER_ZOOM_FACTOR", p0); }
	static void SetStateOfClosestDoorOfType(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_STATE_OF_CLOSEST_DOOR_OF_TYPE", p0, p1, p2, p3, p4, p5); }
	static void SetSyncWeatherAndGameTime(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_SYNC_WEATHER_AND_GAME_TIME", p0); }
	static void SetTaxiGarageRadioState(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_TAXI_GARAGE_RADIO_STATE", p0); }
	static void SetTaxiLights(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TAXI_LIGHTS", p0, p1); }

	static void SetThisScriptCanRemoveBlipsCreatedByAnyScript(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_THIS_SCRIPT_CAN_REMOVE_BLIPS_CREATED_BY_ANY_SCRIPT", p0); }
	static void SetTrainAudioRolloff(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_AUDIO_ROLLOFF", p0, p1); }
	static void SetTrainCruiseSpeed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_CRUISE_SPEED", p0, p1); }
	static void SetTrainForcedToSlowDown(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_FORCED_TO_SLOW_DOWN", p0, p1); }
	static void SetTrainIsStoppedAtStation(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_IS_STOPPED_AT_STATION", p0); }
	static void SetTrainSpeed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_SPEED", p0, p1); }
	static void SetTrainStopsForStations(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_TRAIN_STOPS_FOR_STATIONS", p0, p1); }
	static void SetUseHighDOF(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_USE_HIGHDOF", p0); }
	static void SetUseLegIK(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_USE_LEG_IK", p0, p1); }


//	static void SetVehAlarm(Vehicle car, bool al) { NativeInvoke::Invoke<ScriptVoid>("SET_VEH_ALARM", car, al); }
	static void SetVehHasStrongAxles(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_VEH_HAS_STRONG_AXLES", p0, p1); }
//	static void SetVehInteriorlight(Vehicle car, bool in) { NativeInvoke::Invoke<ScriptVoid>("SET_VEH_INTERIORLIGHT", car, in); }
//	static void SetVehicleAlpha(Vehicle car, unsigned int alp) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_ALPHA", car, alp); }
	static void SetVehicleCanBeTargetted(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_CAN_BE_TARGETTED", p0, p1); }
	static void SetVehicleIsConsideredByPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_IS_CONSIDERED_BY_PLAYER", p0, p1); }
	static void SetVehicleQuaternion(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_QUATERNION", p0, p1, p2, p3, p4); }
	static void SetVehicleSteerBias(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_VEHICLE_STEER_BIAS", p0, p1); }

//	static void SetWidescreenBorders(bool wb) { NativeInvoke::Invoke<ScriptVoid>("SET_WIDESCREEN_BORDERS", wb); }
	static void SetWidescreenFormat(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_WIDESCREEN_FORMAT", p0); }
	static void SetZoneNoCops(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ZONE_NO_COPS", p0, p1); }
	static void SetZonePopulationType(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ZONE_POPULATION_TYPE", p0, p1); }
	static void SetZoneScumminess(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ZONE_SCUMMINESS", p0, p1); }
	static void ShakePad(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SHAKE_PAD", p0, p1, p2); }
	static void ShakePlayerpadWhenControllerDisabled() { NativeInvoke::Invoke<ScriptVoid>("SHAKE_PLAYERPAD_WHEN_CONTROLLER_DISABLED"); }
	static void SkipRadioForward() { NativeInvoke::Invoke<ScriptVoid>("SKIP_RADIO_FORWARD"); }

	static void SkipToNextAllowedStation(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SKIP_TO_NEXT_ALLOWED_STATION", p0); }
	static ScriptAny SlideObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("SLIDE_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void SmashCarWindow(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SMASH_CAR_WINDOW", p0, p1); }
	static ScriptAny SmashGlassOnObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("SMASH_GLASS_ON_OBJECT", p0, p1, p2, p3, p4, p5); }
	static void StartFiringAmnesty() { NativeInvoke::Invoke<ScriptVoid>("START_FIRING_AMNESTY"); }
	static ScriptAny StartNewScript(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("START_NEW_SCRIPT", p0, p1); }
	static ScriptAny StartNewScriptWithArgs(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("START_NEW_SCRIPT_WITH_ARGS", p0, p1, p2, p3); }

	static void StopCarBreaking(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("STOP_CAR_BREAKING", p0, p1); }
//	static void StopMovie() { NativeInvoke::Invoke<ScriptVoid>("STOP_MOVIE"); }
	static void StopPedDoingFallOffTestsWhenShot(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STOP_PED_DOING_FALL_OFF_TESTS_WHEN_SHOT", p0); }
	static void StopPedSpeaking(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("STOP_PED_SPEAKING", p0, p1); }
	static void StopPedWeaponFiringWhenDropped(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STOP_PED_WEAPON_FIRING_WHEN_DROPPED", p0); }
	static void SuppressCarModel(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SUPPRESS_CAR_MODEL", p0); }
	static void SuppressPedModel(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SUPPRESS_PED_MODEL", p0); }
	static void SwapNearestBuildingModel(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWAP_NEAREST_BUILDING_MODEL", p0, p1, p2, p3, p4, p5); }
	
//	static void SwitchCarSiren(Car car, b8 on) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_CAR_SIREN", car, on); }

	static ScriptAny SwitchPedToRagdollWithFall(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13) { return NativeInvoke::Invoke<ScriptAny>("SWITCH_PED_TO_RAGDOLL_WITH_FALL", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); }
	static void SwitchRoadsBackToOriginal(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_ROADS_BACK_TO_ORIGINAL", p0, p1, p2, p3, p4, p5); }
	static void SwitchRoadsOff(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_ROADS_OFF", p0, p1, p2, p3, p4, p5); }
	static void SwitchRoadsOn(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_ROADS_ON", p0, p1, p2, p3, p4, p5); }

//	static ScriptAny UsingStandardControls() { return NativeInvoke::Invoke<ScriptAny>("USING_STANDARD_CONTROLS"); }

//	static ScriptAny UpdateLoadScene() { return NativeInvoke::Invoke<ScriptAny>("UPDATE_LOAD_SCENE"); }

	static ScriptAny AbortScriptedConversation(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("ABORT_SCRIPTED_CONVERSATION", p0); }
	static void ActivateDamageTrackerOnNetworkId(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_DAMAGE_TRACKER_ON_NETWORK_ID", p0, p1); }
	static void ActivateHeliSpeedCheat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_HELI_SPEED_CHEAT", p0, p1); }
//	static void ActivateNetworkSettingsMenu() { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_NETWORK_SETTINGS_MENU"); }
	static void ActivateScriptedCams(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_SCRIPTED_CAMS", p0, p1); }
	static void ActivateViewport(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ACTIVATE_VIEWPORT", p0, p1); }
	static void AddCamSplineNode(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ADD_CAM_SPLINE_NODE", p0, p1); }
	static void AddCarToMissionDeletionList(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_CAR_TO_MISSION_DELETION_LIST", p0); }
	static void AddCoverBlockingArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("ADD_COVER_BLOCKING_AREA", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void AddCoverPoint(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("ADD_COVER_POINT", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void AddFollowNavmeshToPhoneTask(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("ADD_FOLLOW_NAVMESH_TO_PHONE_TASK", p0, p1, p2, p3); }
	static void AddGroupToNetworkRestartNodeGroupList(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_GROUP_TO_NETWORK_RESTART_NODE_GROUP_LIST", p0); }
	static void AddHospitalRestart(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("ADD_HOSPITAL_RESTART", p0, p1, p2, p3, p4); }
	static void AddLineToConversation(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("ADD_LINE_TO_CONVERSATION", p0, p1, p2, p3, p4); }
	static void AddLineToMobilePhoneCall(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("ADD_LINE_TO_MOBILE_PHONE_CALL", p0, p1, p2); }
	static void AddNeededAtPosN(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("ADD_NEEDED_AT_POSN", p0, p1, p2); }
	static void AddNewConversationSpeaker(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("ADD_NEW_CONVERSATION_SPEAKER", p0, p1, p2); }
	static void AddNextMessageToPreviousBriefs(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_NEXT_MESSAGE_TO_PREVIOUS_BRIEFS", p0); }
	static void AddObjectToInteriorRoomByName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ADD_OBJECT_TO_INTERIOR_ROOM_BY_NAME", p0, p1); }
	static void AddPedToMissionDeletionList(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ADD_PED_TO_MISSION_DELETION_LIST", p0, p1); }
	static void AddPointToGpsRaceTrack(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_POINT_TO_GPS_RACE_TRACK", p0); }
	static void AddPoliceRestart(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("ADD_POLICE_RESTART", p0, p1, p2, p3, p4); }
	static void AddScenarioBlockingArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("ADD_SCENARIO_BLOCKING_AREA", p0, p1, p2, p3, p4, p5); }
	static void AddSpawnBlockingArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("ADD_SPAWN_BLOCKING_AREA", p0, p1, p2, p3); }
	static void AddStringToNewsScrollbar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_STRING_TO_NEWS_SCROLLBAR", p0); }
	static void AddStuckCarCheck(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("ADD_STUCK_CAR_CHECK", p0, p1, p2); }
	static void AddStuckCarCheckWithWarp(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("ADD_STUCK_CAR_CHECK_WITH_WARP", p0, p1, p2, p3, p4, p5, p6); }
	static void AddStuntJump(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13, ScriptAny p14, ScriptAny p15) { NativeInvoke::Invoke<ScriptVoid>("ADD_STUNT_JUMP", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15); }
	static void AddToHtmlScriptObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ADD_TO_HTML_SCRIPT_OBJECT", p0, p1); }
	static void AddToPreviousBrief(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_TO_PREVIOUS_BRIEF", p0); }
	static void AddToPreviousBriefWithUnderscore(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_TO_PREVIOUS_BRIEF_WITH_UNDERSCORE", p0); }
	static void AddToWidgetCombo(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ADD_TO_WIDGET_COMBO", p0); }
	static ScriptAny AddWidgetFloatReadOnly(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("ADD_WIDGET_FLOAT_READ_ONLY", p0, p1); }
	static ScriptAny AddWidgetFloatSlider(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("ADD_WIDGET_FLOAT_SLIDER", p0, p1, p2, p3, p4); }
	static ScriptAny AddWidgetReadOnly(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("ADD_WIDGET_READ_ONLY", p0, p1); }
	static ScriptAny AddWidgetSlider(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("ADD_WIDGET_SLIDER", p0, p1, p2, p3, p4); }
	static ScriptAny AddWidgetToggle(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("ADD_WIDGET_TOGGLE", p0, p1); }
	static void AllocateScriptToObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("ALLOCATE_SCRIPT_TO_OBJECT", p0, p1, p2, p3, p4); }
	static void AllocateScriptToRandomPed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("ALLOCATE_SCRIPT_TO_RANDOM_PED", p0, p1, p2, p3); }
	static void AllowAutoConversationLookats(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_AUTO_CONVERSATION_LOOKATS", p0, p1); }
	static void AllowLockonToFriendlyPlayers(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_LOCKON_TO_FRIENDLY_PLAYERS", p0, p1); }
	static void AllowMultipleDrivebyPickups(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_MULTIPLE_DRIVEBY_PICKUPS", p0); }
//	static ScriptAny AllowOneTimeOnlyCommandsToRun() { return NativeInvoke::Invoke<ScriptAny>("ALLOW_ONE_TIME_ONLY_COMMANDS_TO_RUN"); }
	static void AllowReactionAnims(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_REACTION_ANIMS", p0, p1); }
	static void AllowTargetWhenInjured(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_TARGET_WHEN_INJURED", p0, p1); }
	static void AllowThisScriptToBePaused(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ALLOW_THIS_SCRIPT_TO_BE_PAUSED", p0); }
	static void AlwaysUseHeadOnHornAnimWhenDeadInCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ALWAYS_USE_HEAD_ON_HORN_ANIM_WHEN_DEAD_IN_CAR", p0, p1); }
//	static void AmbientAudioBankNoLongerNeeded() { NativeInvoke::Invoke<ScriptVoid>("AMBIENT_AUDIO_BANK_NO_LONGER_NEEDED"); }
	static void AnchorObject(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("ANCHOR_OBJECT", p0, p1, p2); }
	static ScriptAny AreEnemyPedsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("ARE_ENEMY_PEDS_IN_AREA", p0, p1, p2, p3, p4); }
	static void AttachCamToObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_CAM_TO_OBJECT", p0, p1); }
	static void AttachCamToPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_CAM_TO_PED", p0, p1); }
	static void AttachCamToVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_CAM_TO_VEHICLE", p0, p1); }
	static void AttachCarToCar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_CAR_TO_CAR", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void AttachCarToObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_CAR_TO_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void AttachObjectToCarPhysically(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13, ScriptAny p14) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_OBJECT_TO_CAR_PHYSICALLY", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14); }
	static void AttachObjectToObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_OBJECT_TO_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void AttachPedToCarPhysically(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_PED_TO_CAR_PHYSICALLY", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void AttachPedToObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("ATTACH_PED_TO_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void BeginCamCommands(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("BEGIN_CAM_COMMANDS", p0); }
	static void BlendFromNmWithAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("BLEND_FROM_NM_WITH_ANIM", p0, p1, p2, p3, p4, p5, p6); }
	static void BlendOutCharMoveAnims(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("BLEND_OUT_CHAR_MOVE_ANIMS", p0); }
	static void BlockCharHeadIk(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_CHAR_HEAD_IK", p0, p1); }
	static void BlockCharVisemeAnims(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_CHAR_VISEME_ANIMS", p0, p1); }
	static void BlockCoweringInCover(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_COWERING_IN_COVER", p0, p1); }
	static void BlockPeekingInCover(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("BLOCK_PEEKING_IN_COVER", p0, p1); }
	static void CamProcess(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CAM_PROCESS", p0); }
	static void CamSetInterpGraphPos(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CAM_SET_INTERP_GRAPH_POS", p0, p1); }
	static void CamSetInterpGraphRot(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CAM_SET_INTERP_GRAPH_ROT", p0, p1); }
	static ScriptAny CanCreateRandomChar(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("CAN_CREATE_RANDOM_CHAR", p0, p1); }
	static ScriptAny CanPlayerStartMission(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CAN_PLAYER_START_MISSION", p0); }
	static ScriptAny CanRegisterMissionObject() { return NativeInvoke::Invoke<ScriptAny>("CAN_REGISTER_MISSION_OBJECT"); }
	static ScriptAny CanRegisterMissionPed() { return NativeInvoke::Invoke<ScriptAny>("CAN_REGISTER_MISSION_PED"); }
	static ScriptAny CanRegisterMissionVehicle() { return NativeInvoke::Invoke<ScriptAny>("CAN_REGISTER_MISSION_VEHICLE"); }
	static ScriptAny CanStartMissionPassedTune() { return NativeInvoke::Invoke<ScriptAny>("CAN_START_MISSION_PASSED_TUNE"); }
	static ScriptAny CanTheStatHaveString(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CAN_THE_STAT_HAVE_STRING", p0); }
	static void CellCamActivate(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CELL_CAM_ACTIVATE", p0, p1); }
	static ScriptAny CellCamIsCharVisible(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CELL_CAM_IS_CHAR_VISIBLE", p0); }
	static ScriptAny CellCamIsCharVisibleNoFaceCheck(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CELL_CAM_IS_CHAR_VISIBLE_NO_FACE_CHECK", p0); }
	static void CellCamSetCentrePos(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CELL_CAM_SET_CENTRE_POS", p0, p1); }
	static void CellCamSetZoom(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CELL_CAM_SET_ZOOM", p0); }
	static void ChangeCharSitIdleAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_CHAR_SIT_IDLE_ANIM", p0, p1, p2, p3); }
	static void ChangeGarageType(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_GARAGE_TYPE", p0, p1); }
	static void ChangePickupBlipColour(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PICKUP_BLIP_COLOUR", p0); }
	static void ChangePickupBlipDisplay(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PICKUP_BLIP_DISPLAY", p0); }
	static void ChangePickupBlipPriority(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PICKUP_BLIP_PRIORITY", p0); }
	static void ChangePickupBlipScale(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PICKUP_BLIP_SCALE", p0); }
	static void ChangePickupBlipSprite(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PICKUP_BLIP_SPRITE", p0); }
	static void ChangePlaybackToUseAI(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CHANGE_PLAYBACK_TO_USE_AI", p0); }
	static ScriptAny CheckNmFeedback(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("CHECK_NM_FEEDBACK", p0, p1, p2); }
	static ScriptAny CheckStuckTimer(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("CHECK_STUCK_TIMER", p0, p1, p2); }
	static void ClearAdditionalText(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ADDITIONAL_TEXT", p0, p1); }
	static void ClearAngledAreaOfCars(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ANGLED_AREA_OF_CARS", p0, p1, p2, p3, p4, p5, p6); }
	static void ClearBit(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_BIT", p0, p1); }
//	static void ClearBrief() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_BRIEF"); }
	static void ClearCharRelationship(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_CHAR_RELATIONSHIP", p0, p1, p2); }
//	static void ClearHelp() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_HELP"); }
	static void ClearNamedCutscene(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_NAMED_CUTSCENE", p0); }
	static void ClearNetworkRestartNodeGroupList() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_NETWORK_RESTART_NODE_GROUP_LIST"); }
//	static void ClearNewsScrollbar() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_NEWS_SCROLLBAR"); }
	static void ClearOnscreenCounter(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ONSCREEN_COUNTER", p0); }
	static void ClearOnscreenTimer(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ONSCREEN_TIMER", p0); }
//	static void ClearPrints() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_PRINTS"); }
	static void ClearRelationship(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_RELATIONSHIP", p0, p1, p2); }
	static void ClearRoomForViewport(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_ROOM_FOR_VIEWPORT", p0); }
	static void ClearScriptArrayFromScratchpad(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_SCRIPT_ARRAY_FROM_SCRATCHPAD", p0); }
//	static void ClearScriptedConversionCentre() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_SCRIPTED_CONVERSION_CENTRE"); }
//	static void ClearSmallPrints() { NativeInvoke::Invoke<ScriptVoid>("CLEAR_SMALL_PRINTS"); }
	static void ClearTextLabel(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_TEXT_LABEL", p0); }
	static void ClearThisPrint(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_THIS_PRINT", p0); }
	static void ClearThisPrintBigNow(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CLEAR_THIS_PRINT_BIG_NOW", p0); }
//	static void CloseDebugFile() { NativeInvoke::Invoke<ScriptVoid>("CLOSE_DEBUG_FILE"); }
//	static ScriptAny CodeWantsMobilePhoneRemoved() { return NativeInvoke::Invoke<ScriptAny>("CODE_WANTS_MOBILE_PHONE_REMOVED"); }
//	static ScriptAny CodeWantsMobilePhoneRemovedForWeaponSwitching() { return NativeInvoke::Invoke<ScriptAny>("CODE_WANTS_MOBILE_PHONE_REMOVED_FOR_WEAPON_SWITCHING"); }
	static ScriptAny CompareString(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("COMPARE_STRING", p0, p1); }
	static ScriptAny CompareTwoDates(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("COMPARE_TWO_DATES", p0, p1, p2, p3); }
	static void CopyAnimations(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("COPY_ANIMATIONS", p0, p1, p2); }
	static ScriptAny CountPickupsOfType(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("COUNT_PICKUPS_OF_TYPE", p0); }
	static ScriptAny CountScriptCams() { return NativeInvoke::Invoke<ScriptAny>("COUNT_SCRIPT_CAMS"); }
	static ScriptAny CountScriptCamsByTypeAndOrState(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("COUNT_SCRIPT_CAMS_BY_TYPE_AND_OR_STATE", p0, p1, p2); }
	static void CreateCarGenerator(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13, ScriptAny p14) { NativeInvoke::Invoke<ScriptVoid>("CREATE_CAR_GENERATOR", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14); }
	static ScriptAny CreateCheckpoint(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("CREATE_CHECKPOINT", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny CreateEmergencyServicesCarReturnDriver(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("CREATE_EMERGENCY_SERVICES_CAR_RETURN_DRIVER", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny CreateEmergencyServicesCarThenWalk(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("CREATE_EMERGENCY_SERVICES_CAR_THEN_WALK", p0, p1, p2, p3); }
	static ScriptAny CreateHtmlScriptObject(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CREATE_HTML_SCRIPT_OBJECT", p0); }
	static void CreateHtmlViewport(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CREATE_HTML_VIEWPORT", p0); }
	static void CreateMobilePhone(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("CREATE_MOBILE_PHONE", p0); }
	static void CreateNmMessage(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("CREATE_NM_MESSAGE", p0, p1); }
	static void CreateTemporaryRadarBlipsForPickupsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("CREATE_TEMPORARY_RADAR_BLIPS_FOR_PICKUPS_IN_AREA", p0, p1, p2, p3, p4); }
	static ScriptAny CreateWidgetGroup(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("CREATE_WIDGET_GROUP", p0); }
//	static void DeactivateNetworkSettingsMenu() { NativeInvoke::Invoke<ScriptVoid>("DEACTIVATE_NETWORK_SETTINGS_MENU"); }
//	static void DebugOff() { NativeInvoke::Invoke<ScriptVoid>("DEBUG_OFF"); }
//	static void DeleteAllHtmlScriptObjects() { NativeInvoke::Invoke<ScriptVoid>("DELETE_ALL_HTML_SCRIPT_OBJECTS"); }
	static void DeleteCarGenerator(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DELETE_CAR_GENERATOR", p0); }
	static void DeleteCheckpoint(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DELETE_CHECKPOINT", p0); }
	static void DeleteHtmlScriptObject(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DELETE_HTML_SCRIPT_OBJECT", p0); }
	static void DeleteWidgetGroup(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DELETE_WIDGET_GROUP", p0); }
//	static void DestroyAllCams() { NativeInvoke::Invoke<ScriptVoid>("DESTROY_ALL_CAMS"); }
//	static void DestroyMobilePhone() { NativeInvoke::Invoke<ScriptVoid>("DESTROY_MOBILE_PHONE"); }
	static void DestroyThread(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DESTROY_THREAD", p0); }
	static void DestroyViewport(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DESTROY_VIEWPORT", p0); }
	static void DisableCarGenerators(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_CAR_GENERATORS", p0); }
	static void DisableCarGeneratorsWithHeli(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_CAR_GENERATORS_WITH_HELI", p0); }
//	static void DisableEndCreditsFade() { NativeInvoke::Invoke<ScriptVoid>("DISABLE_END_CREDITS_FADE"); }
	static void DisableGPS(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DISABLE_GPS", p0); }
	static void DisplayHelpTextThisFrame(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_HELP_TEXT_THIS_FRAME", p0, p1); }
	static void DisplayNonMinigameHelpMessages(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_NON_MINIGAME_HELP_MESSAGES", p0); }
	static void DisplayPlaybackRecordedCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_PLAYBACK_RECORDED_CAR", p0, p1); }
	static void DisplayPlayerNames(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DISPLAY_PLAYER_NAMES", p0); }
//	static bool DoesGameCodeWantToLeaveNetworkSession() { return NativeInvoke::Invoke<bool>("DOES_GAME_CODE_WANT_TO_LEAVE_NETWORK_SESSION"); }
	static ScriptAny DoesObjectExistWithNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_OBJECT_EXIST_WITH_NETWORK_ID", p0); }
	static ScriptAny DoesPedExistWithNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_PED_EXIST_WITH_NETWORK_ID", p0); }
	static ScriptAny DoesScenarioExistInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("DOES_SCENARIO_EXIST_IN_AREA", p0, p1, p2, p3, p4); }
	static ScriptAny DoesScriptExist(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_SCRIPT_EXIST", p0); }
	static ScriptAny DoesTextLabelExist(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_TEXT_LABEL_EXIST", p0); }
	static ScriptAny DoesThisMinigameScriptAllowNonMinigameHelpMessages() { return NativeInvoke::Invoke<ScriptAny>("DOES_THIS_MINIGAME_SCRIPT_ALLOW_NON_MINIGAME_HELP_MESSAGES"); }
	static ScriptAny DoesVehicleExistWithNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_VEHICLE_EXIST_WITH_NETWORK_ID", p0); }
	static ScriptAny DoesViewportExist(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_VIEWPORT_EXIST", p0); }
	static ScriptAny DoesWebPageExist(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_WEB_PAGE_EXIST", p0); }
	static ScriptAny DoesWidgetGroupExist(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("DOES_WIDGET_GROUP_EXIST", p0); }
	static void DontAbortCarConversations(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("DONT_ABORT_CAR_CONVERSATIONS", p0, p1); }
	static void DontDispatchCopsForPlayer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("DONT_DISPATCH_COPS_FOR_PLAYER", p0, p1); }
	static void DontDisplayLoadingOnFadeThisFrame() { NativeInvoke::Invoke<ScriptVoid>("DONT_DISPLAY_LOADING_ON_FADE_THIS_FRAME"); }
	static void DontSuppressAnyPedModels() { NativeInvoke::Invoke<ScriptVoid>("DONT_SUPPRESS_ANY_PED_MODELS"); }
	static void DontSuppressCarModel(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DONT_SUPPRESS_CAR_MODEL", p0); }
	static void DontSuppressPedModel(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("DONT_SUPPRESS_PED_MODEL", p0); }
	static void DrawCheckpoint(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("DRAW_CHECKPOINT", p0, p1, p2, p3, p4, p5, p6); }
	static void DrawCheckpointWithAlpha(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("DRAW_CHECKPOINT_WITH_ALPHA", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void DrawColouredCylinder(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("DRAW_COLOURED_CYLINDER", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void DrawCurvedWindow(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("DRAW_CURVED_WINDOW", p0, p1, p2, p3, p4); }
	static void DrawFrontendHelperText(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("DRAW_FRONTEND_HELPER_TEXT", p0, p1, p2); }
	static void DrawLightWithRange(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("DRAW_LIGHT_WITH_RANGE", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void DrawMovie(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("DRAW_MOVIE", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void DrawSprite(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("DRAW_SPRITE", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void DrawSpritePhoto(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("DRAW_SPRITE_PHOTO", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void EnableChaseAudio(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_CHASE_AUDIO", p0); }
	static void EnableEndCreditsFade() { NativeInvoke::Invoke<ScriptVoid>("ENABLE_END_CREDITS_FADE"); }
	static void EnableSaveHouse(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_SAVE_HOUSE", p0, p1); }
	static void EnableXboxScreenSaver(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("ENABLE_XBOX_SCREEN_SAVER", p0); }
	static void EndCamCommands(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("END_CAM_COMMANDS", p0); }
	static void EndWidgetGroup() { NativeInvoke::Invoke<ScriptVoid>("END_WIDGET_GROUP"); }
	static void EvolvePtFx(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("EVOLVE_PTFX", p0, p1, p2); }
	static void ExplodeCarInCutscene(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("EXPLODE_CAR_IN_CUTSCENE", p0, p1); }
	static void FindNearestCollectableBinBags(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("FIND_NEAREST_COLLECTABLE_BIN_BAGS", p0, p1, p2); }
	static ScriptAny FindNetworkKillerOfPlayer(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("FIND_NETWORK_KILLER_OF_PLAYER", p0); }
	static ScriptAny FindPositionInRecording(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("FIND_POSITION_IN_RECORDING", p0); }
	static void FindPrimaryPopulationZoneGroup(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("FIND_PRIMARY_POPULATION_ZONE_GROUP", p0, p1); }
	static ScriptAny FindTimePositionInRecording(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("FIND_TIME_POSITION_IN_RECORDING", p0); }
	static ScriptAny FinishWidgetCombo(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("FINISH_WIDGET_COMBO", p0, p1); }
	static void FlashRoute(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("FLASH_ROUTE", p0); }
	static void FlushAllOutOfDateRadarBlipsFromMissionCleanupList() { NativeInvoke::Invoke<ScriptVoid>("FLUSH_ALL_OUT_OF_DATE_RADAR_BLIPS_FROM_MISSION_CLEANUP_LIST"); }
	static void FlushAllSpawnBlockingAreas() { NativeInvoke::Invoke<ScriptVoid>("FLUSH_ALL_SPAWN_BLOCKING_AREAS"); }
	static void FlushCoverBlockingAreas() { NativeInvoke::Invoke<ScriptVoid>("FLUSH_COVER_BLOCKING_AREAS"); }
	static void FlushScenarioBlockingAreas() { NativeInvoke::Invoke<ScriptVoid>("FLUSH_SCENARIO_BLOCKING_AREAS"); }
	static void ForceAirDragMultForPlayersCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("FORCE_AIR_DRAG_MULT_FOR_PLAYERS_CAR", p0, p1); }
	static void ForceFullVoice(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("FORCE_FULL_VOICE", p0); }
	static void ForceHighLod(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("FORCE_HIGH_LOD", p0); }
	static void ForceNoCamPause(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("FORCE_NO_CAM_PAUSE", p0); }
	static void ForcePedToLoadCover(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("FORCE_PED_TO_LOAD_COVER", p0, p1); }
	static void ForceRadioTrack(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("FORCE_RADIO_TRACK", p0, p1, p2, p3); }
	static void ForceSpawnScenarioPedsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("FORCE_SPAWN_SCENARIO_PEDS_IN_AREA", p0, p1, p2, p3, p4); }
	static void GenerateDirections(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("GENERATE_DIRECTIONS", p0, p1, p2, p3, p4); }
	static ScriptAny GetAudibleMusicTrackTextId() { return NativeInvoke::Invoke<ScriptAny>("GET_AUDIBLE_MUSIC_TRACK_TEXT_ID"); }
	static ScriptAny GetBitsInRange(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("GET_BITS_IN_RANGE", p0, p1, p2); }
	static void GetCarModelValue(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_CAR_MODEL_VALUE", p0, p1); }
	static void GetCinematicCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_CINEMATIC_CAM", p0); }
	static ScriptAny GetClosestNetworkRestartNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("GET_CLOSEST_NETWORK_RESTART_NODE", p0, p1, p2, p3, p4); }
	static ScriptAny GetClosestRoad(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { return NativeInvoke::Invoke<ScriptAny>("GET_CLOSEST_ROAD", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void GetCoordinatesForNetworkRestartNode(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GET_COORDINATES_FOR_NETWORK_RESTART_NODE", p0, p1, p2); }
	static ScriptAny GetCurrentPlaybackNumberForCar(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_CURRENT_PLAYBACK_NUMBER_FOR_CAR", p0); }	
	static ScriptAny GetCurrentScriptedConversationLine() { return NativeInvoke::Invoke<ScriptAny>("GET_CURRENT_SCRIPTED_CONVERSATION_LINE"); }
	static ScriptAny GetCutsceneAudioTimeMs() { return NativeInvoke::Invoke<ScriptAny>("GET_CUTSCENE_AUDIO_TIME_MS"); }
	static void GetCutscenePedPosition(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_CUTSCENE_PED_POSITION", p0, p1); }
	static ScriptAny GetCutsceneSectionPlaying() { return NativeInvoke::Invoke<ScriptAny>("GET_CUTSCENE_SECTION_PLAYING"); }
	static ScriptAny GetCutsceneTime() { return NativeInvoke::Invoke<ScriptAny>("GET_CUTSCENE_TIME"); }
	static void GetDebugCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_DEBUG_CAM", p0); }
	static ScriptAny GetDestroyerOfNetworkId(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("GET_DESTROYER_OF_NETWORK_ID", p0, p1); }
	static void GetFollowVehicleCamSubmode(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_FOLLOW_VEHICLE_CAM_SUBMODE", p0); }
	static void GetFrontEndDesignValue(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GET_FRONTEND_DESIGN_VALUE", p0, p1, p2); }
	static void GetGameViewportId(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_GAME_VIEWPORT_ID", p0); }
	static ScriptAny GetGamerNetworkScore(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("GET_GAMER_NETWORK_SCORE", p0, p1, p2); }
	static void GetHelpMessageBoxSize(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_HELP_MESSAGE_BOX_SIZE", p0, p1); }
//	static ScriptAny GetHostId() { return NativeInvoke::Invoke<ScriptAny>("GET_HOST_ID"); }
	static void GetKeyForViewportInRoom(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_KEY_FOR_VIEWPORT_IN_ROOM", p0, p1); }
	static ScriptAny GetLocalGamerlevelFromProfilesettings() { return NativeInvoke::Invoke<ScriptAny>("GET_LOCAL_GAMERLEVEL_FROM_PROFILESETTINGS"); }
	static ScriptAny GetLocalPlayerMPCash() { return NativeInvoke::Invoke<ScriptAny>("GET_LOCAL_PLAYER_MP_CASH"); }
	static ScriptAny GetMobilePhoneTaskSubTask(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("GET_MOBILE_PHONE_TASK_SUB_TASK", p0, p1); }
	static ScriptAny GetMotionControlPreference(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_MOTION_CONTROL_PREFERENCE", p0); }
	static void GetNetworkIdFromObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_NETWORK_ID_FROM_OBJECT", p0, p1); }
	static void GetNetworkIdFromPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_NETWORK_ID_FROM_PED", p0, p1); }
	static void GetNetworkIdFromVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_NETWORK_ID_FROM_VEHICLE", p0, p1); }
	static void GetNetworkTimer(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_NETWORK_TIMER", p0); }
	static ScriptAny GetNextClosestCarNodeWithHeadingOnIsland(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("GET_NEXT_CLOSEST_CAR_NODE_WITH_HEADING_ON_ISLAND", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny GetNoOfPlayersInTeam(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_NO_OF_PLAYERS_IN_TEAM", p0); }
	static ScriptAny GetNthClosestCarNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("GET_NTH_CLOSEST_CAR_NODE", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny GetNthClosestCarNodeFavourDirection(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("GET_NTH_CLOSEST_CAR_NODE_FAVOUR_DIRECTION", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny GetNumberOfWebPageLinks(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_NUMBER_OF_WEB_PAGE_LINKS", p0); }
	static void GetObjectFromNetworkId(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_OBJECT_FROM_NETWORK_ID", p0, p1); }
	static void GetPedFromNetworkId(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_PED_FROM_NETWORK_ID", p0, p1); }
	static ScriptAny GetPlayerRankLevelDuringMP(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_PLAYER_RANK_LEVEL_DURING_MP", p0); }
	static void GetPlayerRGBColour(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("GET_PLAYER_RGB_COLOUR", p0, p1, p2, p3); }
	static ScriptAny GetPlayerTeam(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_PLAYER_TEAM", p0); }
	static void GetPositionOfCarRecordingAtTime(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GET_POSITION_OF_CAR_RECORDING_AT_TIME", p0, p1, p2); }
	static ScriptAny GetProfileSetting(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_PROFILE_SETTING", p0); }
	static ScriptAny GetRandomCarNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("GET_RANDOM_CAR_NODE", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny GetRandomNetworkRestartNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("GET_RANDOM_NETWORK_RESTART_NODE", p0, p1, p2, p3, p4, p5); }
	static ScriptAny GetRandomNetworkRestartNodeUsingGroupList(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("GET_RANDOM_NETWORK_RESTART_NODE_USING_GROUP_LIST", p0, p1, p2, p3, p4, p5); }
	static void GetRootCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_ROOT_CAM", p0); }
	static void GetScreenViewportId(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_SCREEN_VIEWPORT_ID", p0); }
	static void GetScriptCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("GET_SCRIPT_CAM", p0); }
	static void GetScriptTaskStatus(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("GET_SCRIPT_TASK_STATUS", p0, p1, p2); }
	static void GetSequenceProgress(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_SEQUENCE_PROGRESS", p0, p1); }
	static ScriptAny GetSortedNetworkRestartNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("GET_SORTED_NETWORK_RESTART_NODE", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny GetSortedNetworkRestartNodeUsingGroupList(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("GET_SORTED_NETWORK_RESTART_NODE_USING_GROUP_LIST", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void GetSpawnCoordinatesForCarNode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("GET_SPAWN_COORDINATES_FOR_CAR_NODE", p0, p1, p2, p3, p4, p5); }
	static ScriptAny GetSpeechForEmergencyServiceCall() { return NativeInvoke::Invoke<ScriptAny>("GET_SPEECH_FOR_EMERGENCY_SERVICE_CALL"); }
	static void GetStateOfClosestDoorOfType(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("GET_STATE_OF_CLOSEST_DOOR_OF_TYPE", p0, p1, p2, p3, p4, p5); }
	static ScriptAny GetStringFromTextFile(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_STRING_FROM_TEXT_FILE", p0); }
	static ScriptAny GetTaskPlaceCarBombUnsuccessful() { return NativeInvoke::Invoke<ScriptAny>("GET_TASK_PLACE_CAR_BOMB_UNSUCCESSFUL"); }
	static void GetTeamRGBColour(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("GET_TEAM_RGB_COLOUR", p0, p1, p2, p3); }
	static ScriptAny GetTotalDurationOfCarRecording(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_TOTAL_DURATION_OF_CAR_RECORDING", p0); }
	static void GetVehicleFromNetworkId(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("GET_VEHICLE_FROM_NETWORK_ID", p0, p1); }
	static ScriptAny GetWebPageHeight(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("GET_WEB_PAGE_HEIGHT", p0); }
	static ScriptAny GetWebPageLinkAtPosn(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("GET_WEB_PAGE_LINK_AT_POSN", p0, p1, p2); }
	static ScriptAny GetWebPageLinkHref(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("GET_WEB_PAGE_LINK_HREF", p0, p1); }
	static void HandleAudioAnimEvent(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("HANDLE_AUDIO_ANIM_EVENT", p0, p1); }
	static ScriptAny HasCarRecordingBeenLoaded(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("HAS_CAR_RECORDING_BEEN_LOADED", p0); }
	static ScriptAny HasCharAnimFinished(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("HAS_CHAR_ANIM_FINISHED", p0, p1, p2); }
	static ScriptAny HasControlOfNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("HAS_CONTROL_OF_NETWORK_ID", p0); }
	static ScriptAny HasCutsceneFinished() { return NativeInvoke::Invoke<ScriptAny>("HAS_CUTSCENE_FINISHED"); }
	static ScriptAny HasCutsceneLoaded() { return NativeInvoke::Invoke<ScriptAny>("HAS_CUTSCENE_LOADED"); }
	static ScriptAny HasFragmentRootOfClosestObjectOfTypeBeenDamaged(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("HAS_FRAGMENT_ROOT_OF_CLOSEST_OBJECT_OF_TYPE_BEEN_DAMAGED", p0, p1, p2, p3, p4); }
	static ScriptAny HasGamerChangedNetworkModelSettings() { return NativeInvoke::Invoke<ScriptAny>("HAS_GAMER_CHANGED_NETWORK_MODEL_SETTINGS"); }
	static ScriptAny HasNetworkPlayerLeftGame(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("HAS_NETWORK_PLAYER_LEFT_GAME", p0); }
	static ScriptAny HasOverridenSitIdleAnimFinished(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("HAS_OVERRIDEN_SIT_IDLE_ANIM_FINISHED", p0); }
	static ScriptAny HasReloadedWithMotionControl(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("HAS_RELOADED_WITH_MOTION_CONTROL", p0, p1); }
	static void HideCharWeaponForScriptedCutscene(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("HIDE_CHAR_WEAPON_FOR_SCRIPTED_CUTSCENE", p0, p1); }
	static void HintCam(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("HINT_CAM", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny HowLongHasNetworkPlayerBeenDeadFor(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("HOW_LONG_HAS_NETWORK_PLAYER_BEEN_DEAD_FOR", p0); }
	static void InitCutscene(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("INIT_CUTSCENE", p0); }
	static void InitDebugWidgets() { NativeInvoke::Invoke<ScriptVoid>("INIT_DEBUG_WIDGETS"); }
	static void InitFrontendHelperText() { NativeInvoke::Invoke<ScriptVoid>("INIT_FRONTEND_HELPER_TEXT"); }
	static ScriptAny IsAmbientSpeechDisabled(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_AMBIENT_SPEECH_DISABLED", p0); }
	static ScriptAny IsAnyCharShootingInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { return NativeInvoke::Invoke<ScriptAny>("IS_ANY_CHAR_SHOOTING_IN_AREA", p0, p1, p2, p3, p4, p5, p6); }
	static ScriptAny IsAnySpeechPlaying(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_ANY_SPEECH_PLAYING", p0); }
	static ScriptAny IsBitSet(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("IS_BIT_SET", p0, p1); }
	static ScriptAny IsCharRespondingToEvent(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("IS_CHAR_RESPONDING_TO_EVENT", p0, p1); }
	static ScriptAny IsDamageTrackerActiveOnNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_DAMAGE_TRACKER_ACTIVE_ON_NETWORK_ID", p0); }
	static ScriptAny IsDebugCameraOn() { return NativeInvoke::Invoke<ScriptAny>("IS_DEBUG_CAMERA_ON"); }
	static ScriptAny IsNetworkGamePending() { return NativeInvoke::Invoke<ScriptAny>("IS_NETWORK_GAME_PENDING"); }
	static ScriptAny IsNetworkGameRunning() { return NativeInvoke::Invoke<ScriptAny>("IS_NETWORK_GAME_RUNNING"); }
	static ScriptAny IsNetworkPlayerActive(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_NETWORK_PLAYER_ACTIVE", p0); }
	static ScriptAny IsNetworkSession() { return NativeInvoke::Invoke<ScriptAny>("IS_NETWORK_SESSION"); }
	static ScriptAny IsNonFragObjectSmashed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { return NativeInvoke::Invoke<ScriptAny>("IS_NON_FRAG_OBJECT_SMASHED", p0, p1, p2, p3, p4); }
	static ScriptAny IsPlaybackGoingOnForCar(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYBACK_GOING_ON_FOR_CAR", p0); }
	static ScriptAny IsPlayerLoggingInNp() { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYER_LOGGING_IN_NP"); }
	static ScriptAny IsPlayerOnlineGamespy() { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYER_ONLINE_GAMESPY"); }
	static ScriptAny IsPlayerOnlineNp() { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYER_ONLINE_NP"); }
	static ScriptAny IsPlayerReadyForCutscene(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYER_READY_FOR_CUTSCENE", p0); }
	static ScriptAny IsPlayerScriptControlOn(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_PLAYER_SCRIPT_CONTROL_ON", p0); }
	static ScriptAny IsRecordingGoingOnForCar(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_RECORDING_GOING_ON_FOR_CAR", p0); }
	static ScriptAny IsScriptedConversationOngoing() { return NativeInvoke::Invoke<ScriptAny>("IS_SCRIPTED_CONVERSATION_ONGOING"); }
	static ScriptAny IsSpecificCamInterpolating(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_SPECIFIC_CAM_INTERPOLATING", p0); }
	static ScriptAny IsSphereVisibleToAnotherMachine(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("IS_SPHERE_VISIBLE_TO_ANOTHER_MACHINE", p0, p1, p2, p3); }
	static ScriptAny IsStreamingPriorityRequests() { return NativeInvoke::Invoke<ScriptAny>("IS_STREAMING_PRIORITY_REQUESTS"); }
	static ScriptAny IsThisHelpMessageBeingDisplayed(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("IS_THIS_HELP_MESSAGE_BEING_DISPLAYED", p0); }
	static ScriptAny IsThisHelpMessageWithNumberBeingDisplayed(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("IS_THIS_HELP_MESSAGE_WITH_NUMBER_BEING_DISPLAYED", p0, p1); }
	static ScriptAny IsThisHelpMessageWithStringBeingDisplayed(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("IS_THIS_HELP_MESSAGE_WITH_STRING_BEING_DISPLAYED", p0, p1); }
	static ScriptAny IsThisMachineTheServer() { return NativeInvoke::Invoke<ScriptAny>("IS_THIS_MACHINE_THE_SERVER"); }
	static ScriptAny IsThisPrintBeingDisplayed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { return NativeInvoke::Invoke<ScriptAny>("IS_THIS_PRINT_BEING_DISPLAYED", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static ScriptAny IsWorldPointWithinBrainActivationRange() { return NativeInvoke::Invoke<ScriptAny>("IS_WORLD_POINT_WITHIN_BRAIN_ACTIVATION_RANGE"); }
	static ScriptAny IsXbox360Version() { return NativeInvoke::Invoke<ScriptAny>("IS_XBOX360_VERSION"); }
	static void LaunchLocalPlayerInNetworkGame() { NativeInvoke::Invoke<ScriptVoid>("LAUNCH_LOCAL_PLAYER_IN_NETWORK_GAME"); }
	static void LoadPathNodesInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("LOAD_PATH_NODES_IN_AREA", p0, p1, p2, p3); }
	static void LoadWebPage(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("LOAD_WEB_PAGE", p0, p1); }
	static ScriptAny LocalPlayerIsReadyToStartPlaying() { return NativeInvoke::Invoke<ScriptAny>("LOCAL_PLAYER_IS_READY_TO_START_PLAYING"); }
	static ScriptAny LookAtNearbyEntityWithSpecialAttribute(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("LOOK_AT_NEARBY_ENTITY_WITH_SPECIAL_ATTRIBUTE", p0, p1, p2, p3, p4, p5); }
	static void MakePlayerSafeForCutscene(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("MAKE_PLAYER_SAFE_FOR_CUTSCENE", p0); }
	static void MissionAudioBankNoLongerNeeded() { NativeInvoke::Invoke<ScriptVoid>("MISSION_AUDIO_BANK_NO_LONGER_NEEDED"); }
	static void MuteGameworldAndPositionedRadioForTv(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("MUTE_GAMEWORLD_AND_POSITIONED_RADIO_FOR_TV", p0); }
	static void MuteGameworldAudio(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("MUTE_GAMEWORLD_AUDIO", p0); }
	static void MutePositionedRadio(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("MUTE_POSITIONED_RADIO", p0); }
	static ScriptAny NetworkAcceptInvite(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_ACCEPT_INVITE", p0); }
	static ScriptAny NetworkAdvertiseSession(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_ADVERTISE_SESSION", p0); }
	static ScriptAny NetworkAllPartyMembersPresent() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_ALL_PARTY_MEMBERS_PRESENT"); }
	static ScriptAny NetworkAmIBlockedByPlayer(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_AM_I_BLOCKED_BY_PLAYER", p0); }
	static ScriptAny NetworkAmIMutedByPlayer(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_AM_I_MUTED_BY_PLAYER", p0); }
	static ScriptAny NetworkChangeGameMode(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_CHANGE_GAME_MODE", p0, p1, p2, p3); }
	static ScriptAny NetworkChangeGameModePending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_CHANGE_GAME_MODE_PENDING"); }
	static ScriptAny NetworkChangeGameModeSucceeded() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_CHANGE_GAME_MODE_SUCCEEDED"); }
	static ScriptAny NetworkCheckInviteArrival() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_CHECK_INVITE_ARRIVAL"); }
	static void NetworkClearInviteArrival() { NativeInvoke::Invoke<ScriptVoid>("NETWORK_CLEAR_INVITE_ARRIVAL"); }
	static ScriptAny NetworkClearSummons() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_CLEAR_SUMMONS"); }
	static ScriptAny NetworkDidInviteFriend(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_DID_INVITE_FRIEND", p0); }
	static ScriptAny NetworkEndSession() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_END_SESSION"); }
	static ScriptAny NetworkEndSessionPending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_END_SESSION_PENDING"); }
	static void NetworkFindGame(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_FIND_GAME", p0, p1, p2, p3); }
	static ScriptAny NetworkFindGamePending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_FIND_GAME_PENDING"); }
	static ScriptAny NetworkGetFriendCount() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_FRIEND_COUNT"); }
	static ScriptAny NetworkGetFriendName(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_FRIEND_NAME", p0); }
	static ScriptAny NetworkGetGameMode() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_GAME_MODE"); }
	static ScriptAny NetworkGetMaxPrivateSlots() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_MAX_PRIVATE_SLOTS"); }
	static ScriptAny NetworkGetMaxSlots() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_MAX_SLOTS"); }
	static ScriptAny NetworkGetMetPlayerName(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_MET_PLAYER_NAME", p0); }
	static ScriptAny NetworkGetNumPartyMembers() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_NUM_PARTY_MEMBERS"); }
	static ScriptAny NetworkGetNumPlayersMet() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_NUM_PLAYERS_MET"); }
	static ScriptAny NetworkGetNumUnacceptedInvites() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_NUM_UNACCEPTED_INVITES"); }
	static ScriptAny NetworkGetNumUnfilledReservations() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_NUM_UNFILLED_RESERVATIONS"); }
	static ScriptAny NetworkGetNumberOfGames() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_NUMBER_OF_GAMES"); }
	static ScriptAny NetworkGetRendezvousHostPlayerId() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_RENDEZVOUS_HOST_PLAYER_ID"); }
	static ScriptAny NetworkGetUnacceptedInviteEpisode(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_UNACCEPTED_INVITE_EPISODE", p0); }
	static ScriptAny NetworkGetUnacceptedInviteGameMode(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_UNACCEPTED_INVITE_GAME_MODE", p0); }
	static ScriptAny NetworkGetUnacceptedInviterName(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_GET_UNACCEPTED_INVITER_NAME", p0); }
	static ScriptAny NetworkHaveAcceptedInvite() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HAVE_ACCEPTED_INVITE"); }
	static ScriptAny NetworkHaveOnlinePrivileges() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HAVE_ONLINE_PRIVILEGES"); }
	static ScriptAny NetworkHaveSummons() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HAVE_SUMMONS"); }
	static ScriptAny NetworkHostGame(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HOST_GAME", p0, p1, p2, p3, p4, p5); }
	static ScriptAny NetworkHostGamePending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HOST_GAME_PENDING"); }
	static ScriptAny NetworkHostGameSucceeded() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HOST_GAME_SUCCEEDED"); }
	static ScriptAny NetworkHostRendezvous(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_HOST_RENDEZVOUS", p0, p1, p2); }
	static ScriptAny NetworkInviteFriend(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_INVITE_FRIEND", p0, p1); }
	static ScriptAny NetworkIsCommonEpisode(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_COMMON_EPISODE", p0); }
	static ScriptAny NetworkIsFriendInSameTitle(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_FRIEND_IN_SAME_TITLE", p0); }
	static ScriptAny NetworkIsFriendOnline(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_FRIEND_ONLINE", p0); }
	static ScriptAny NetworkIsInviteeOnline() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_INVITEE_ONLINE"); }
	static ScriptAny NetworkIsOperationPending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_OPERATION_PENDING"); }
	static ScriptAny NetworkIsPlayerBlockedByMe(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_PLAYER_BLOCKED_BY_ME", p0); }
	static ScriptAny NetworkIsPlayerMutedByMe(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_PLAYER_MUTED_BY_ME", p0); }
	static ScriptAny NetworkIsPlayerTalking(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_PLAYER_TALKING", p0); }
	static ScriptAny NetworkIsRendezvous() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_RENDEZVOUS"); }
	static ScriptAny NetworkIsRendezvousHost() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_RENDEZVOUS_HOST"); }
	static ScriptAny NetworkIsSessionAdvertised() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_SESSION_ADVERTISED"); }
	static ScriptAny NetworkIsSessionInvitable() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_SESSION_INVITABLE"); }
	static ScriptAny NetworkIsTvt() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_IS_TVT"); }
	static ScriptAny NetworkJoinGame(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_JOIN_GAME", p0); }
	static ScriptAny NetworkJoinGamePending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_JOIN_GAME_PENDING"); }
	static ScriptAny NetworkJoinGameSucceeded() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_JOIN_GAME_SUCCEEDED"); }
	static ScriptAny NetworkJoinSummons() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_JOIN_SUMMONS"); }
	static void NetworkKickPlayer(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_KICK_PLAYER", p0); }
	static ScriptAny NetworkLeaveGame() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_LEAVE_GAME"); }
	static ScriptAny NetworkLeaveGamePending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_LEAVE_GAME_PENDING"); }
	static ScriptAny NetworkPlayerHasDiedRecently(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_PLAYER_HAS_DIED_RECENTLY", p0); }
	static ScriptAny NetworkPlayerHasHeadset(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_PLAYER_HAS_HEADSET", p0); }
	static ScriptAny NetworkRestoreGameConfig(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_RESTORE_GAME_CONFIG", p0); }
	static ScriptAny NetworkReturnToRendezvous() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_RETURN_TO_RENDEZVOUS"); }
	static ScriptAny NetworkReturnToRendezvousPending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_RETURN_TO_RENDEZVOUS_PENDING"); }
	static ScriptAny NetworkReturnToRendezvousSucceeded() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_RETURN_TO_RENDEZVOUS_SUCCEEDED"); }
	static void NetworkSetFriendlyFireOption(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SET_FRIENDLY_FIRE_OPTION", p0); }
	static void NetworkSetHealthReticuleOption(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SET_HEALTH_RETICULE_OPTION", p0); }
	static ScriptAny NetworkSetPlayerMuted(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("NETWORK_SET_PLAYER_MUTED", p0, p1); }
	static void NetworkSetTalkerFocus(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SET_TALKER_FOCUS", p0); }
	static void NetworkSetTalkerProximity(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SET_TALKER_PROXIMITY", p0); }
	static void NetworkSetTeamOnlyChat(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SET_TEAM_ONLY_CHAT", p0); }
	static void NetworkShowFriendProfileUi(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SHOW_FRIEND_PROFILE_UI", p0); }
	static void NetworkShowMetPlayerFeedbackUi(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SHOW_MET_PLAYER_FEEDBACK_UI", p0); }
	static void NetworkShowMetPlayerProfileUi(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SHOW_MET_PLAYER_PROFILE_UI", p0); }
	static void NetworkShowPlayerProfileUi(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_SHOW_PLAYER_PROFILE_UI", p0); }
	static ScriptAny NetworkStartSession() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_START_SESSION"); }
	static ScriptAny NetworkStartSessionPending() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_START_SESSION_PENDING"); }
	static ScriptAny NetworkStartSessionSucceeded() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_START_SESSION_SUCCEEDED"); }
	static void NetworkStoreGameConfig(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("NETWORK_STORE_GAME_CONFIG", p0); }
	static ScriptAny NetworkStoreSinglePlayerGame() { return NativeInvoke::Invoke<ScriptAny>("NETWORK_STORE_SINGLE_PLAYER_GAME"); }
	static void NewMobilePhoneCall() { NativeInvoke::Invoke<ScriptVoid>("NEW_MOBILE_PHONE_CALL"); }
	static void NewScriptedConversation() { NativeInvoke::Invoke<ScriptVoid>("NEW_SCRIPTED_CONVERSATION"); }
	static void OpenDebugFile() { NativeInvoke::Invoke<ScriptVoid>("OPEN_DEBUG_FILE"); }
	static void OverrideFreezeFlags(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("OVERRIDE_FREEZE_FLAGS", p0); }
	static void OverrideNextRestart(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("OVERRIDE_NEXT_RESTART", p0, p1, p2, p3); }
	static void OverrideNumberOfParkedCars(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("OVERRIDE_NUMBER_OF_PARKED_CARS", p0); }
	static void PausePlaybackRecordedCar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PAUSE_PLAYBACK_RECORDED_CAR", p0); }
	static void PauseScriptedConversation(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PAUSE_SCRIPTED_CONVERSATION", p0); }
	static void PlayAudioEventFromObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PLAY_AUDIO_EVENT_FROM_OBJECT", p0, p1); }
	static void PlayAudioEventFromPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PLAY_AUDIO_EVENT_FROM_PED", p0, p1); }
	static void PlayAudioEventFromVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PLAY_AUDIO_EVENT_FROM_VEHICLE", p0, p1); }
	static void PlayFireSoundFromPosition(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PLAY_FIRE_SOUND_FROM_POSITION", p0, p1, p2, p3); }
	static ScriptAny PlayerWantsToJoinNetworkGame(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("PLAYER_WANTS_TO_JOIN_NETWORK_GAME", p0); }
	static void PlayStatsInt(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PLAYSTATS_INT", p0, p1); }
	static void PlayStatsMissionCancelled(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAYSTATS_MISSION_CANCELLED", p0); }
	static void PlayStatsMissionFailed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAYSTATS_MISSION_FAILED", p0); }
	static void PlayStatsMissionPassed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAYSTATS_MISSION_PASSED", p0); }
	static void PlayStatsMissionStarted(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PLAYSTATS_MISSION_STARTED", p0); }
	static void PointCamAtCoord(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("POINT_CAM_AT_COORD", p0, p1, p2, p3); }
	static void PointCamAtObject(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("POINT_CAM_AT_OBJECT", p0, p1); }
	static void PointCamAtPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("POINT_CAM_AT_PED", p0, p1); }
	static void PointCamAtVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("POINT_CAM_AT_VEHICLE", p0, p1); }
	static ScriptAny PreloadStream(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("PRELOAD_STREAM", p0); }
	static ScriptAny PreloadStreamWithStartOffset(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("PRELOAD_STREAM_WITH_START_OFFSET", p0, p1); }
	static void PreviewRingtone(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PREVIEW_RINGTONE", p0); }
	static void Print(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PRINT", p0, p1, p2); }
	static void PrintBig(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PRINT_BIG", p0, p1, p2); }
	static void PrintHelp(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP", p0); }
	static void PrintHelpForever(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_FOREVER", p0); }
	static void PrintHelpForeverWithNumber(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_FOREVER_WITH_NUMBER", p0, p1); }
	static void PrintHelpForeverWithString(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_FOREVER_WITH_STRING", p0, p1); }
	static void PrintHelpForeverWithStringNoSound(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_FOREVER_WITH_STRING_NO_SOUND", p0, p1); }
	static void PrintHelpWithNumber(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_WITH_NUMBER", p0, p1); }
	static void PrintHelpWithString(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_WITH_STRING", p0, p1); }
	static void PrintHelpWithStringNoSound(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("PRINT_HELP_WITH_STRING_NO_SOUND", p0, p1); }
	static void PrintNow(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PRINT_NOW", p0, p1, p2); }
	static void PrintStringInString(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PRINT_STRING_IN_STRING", p0, p1, p2, p3); }
	static void PrintStringInStringNow(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PRINT_STRING_IN_STRING_NOW", p0, p1, p2, p3); }
	static void PrintWith2Numbers(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("PRINT_WITH_2_NUMBERS", p0, p1, p2, p3, p4); }
	static void PrintWith2NumbersNow(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("PRINT_WITH_2_NUMBERS_NOW", p0, p1, p2, p3, p4); }
	static void PrintWithNumber(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PRINT_WITH_NUMBER", p0, p1, p2, p3); }
	static void PrintWithNumberBig(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PRINT_WITH_NUMBER_BIG", p0, p1, p2, p3); }
	static void PrintWithNumberNow(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("PRINT_WITH_NUMBER_NOW", p0, p1, p2, p3); }
	static void PrintFloat(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PRINTFLOAT", p0); }
	static void PrintInt(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PRINTINT", p0); }
	static void PrintNL() { NativeInvoke::Invoke<ScriptVoid>("PRINTNL"); }
	static void PrintString(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PRINTSTRING", p0); }
	static void PrintVector(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("PRINTVECTOR", p0, p1, p2); }
	static void PrioritizeStreamingRequest() { NativeInvoke::Invoke<ScriptVoid>("PRIORITIZE_STREAMING_REQUEST"); }
	static void ProcessMissionDeletionList() { NativeInvoke::Invoke<ScriptVoid>("PROCESS_MISSION_DELETION_LIST"); }
	static void ProstituteCamActivate(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("PROSTITUTE_CAM_ACTIVATE", p0); }
	static void RegisterBestPosition(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_BEST_POSITION", p0, p1); }
	static void RegisterClientBroadcastVariables(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_CLIENT_BROADCAST_VARIABLES", p0, p1, p2); }
	static void RegisterHatedTargetsAroundPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_HATED_TARGETS_AROUND_PED", p0, p1); }
	static void RegisterHatedTargetsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_HATED_TARGETS_IN_AREA", p0, p1, p2, p3, p4); }
	static void RegisterHostBroadcastVariables(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_HOST_BROADCAST_VARIABLES", p0, p1, p2); }
	static void RegisterKillInMultiplayerGame(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_KILL_IN_MULTIPLAYER_GAME", p0, p1, p2); }
	static void RegisterMissionPassed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_MISSION_PASSED", p0); }
	static void RegisterMultiplayerGameWin(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_MULTIPLAYER_GAME_WIN", p0, p1); }
	static void RegisterNetworkBestGameScores(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_NETWORK_BEST_GAME_SCORES", p0, p1, p2); }
	static void RegisterScriptWithAudio(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_SCRIPT_WITH_AUDIO", p0); }
	static void RegisterStringForFrontendStat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_STRING_FOR_FRONTEND_STAT", p0, p1); }
	static void RegisterTarget(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_TARGET", p0, p1); }
	static void RegisterTrackNumber(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_TRACK_NUMBER", p0); }
	static void RegisterWorldPointScriptBrain(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("REGISTER_WORLD_POINT_SCRIPT_BRAIN", p0, p1); }
	static void ReleasePathNodes() { NativeInvoke::Invoke<ScriptVoid>("RELEASE_PATH_NODES"); }
	static void ReloadWebPage(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RELOAD_WEB_PAGE", p0); }
	static void RemoveCarRecording(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CAR_RECORDING", p0); }
	static void RemoveCarsFromGeneratorsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_CARS_FROM_GENERATORS_IN_AREA", p0, p1, p2, p3, p4, p5); }
	static void RemoveFakeNetworkNameFromPed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_FAKE_NETWORK_NAME_FROM_PED", p0); }
	static void RemoveProjtexFromObject(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_PROJTEX_FROM_OBJECT", p0); }
	static void RemovePtfxFromPed(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REMOVE_PTFX_FROM_PED", p0); }
	static void RenderRaceTrack(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RENDER_RACE_TRACK", p0); }
	static void ReportTaggedRadioTrack(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REPORT_TAGGED_RADIO_TRACK", p0); }
	static ScriptAny RequestAmbientAudioBank(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("REQUEST_AMBIENT_AUDIO_BANK", p0); }
	static void RequestCarRecording(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("REQUEST_CAR_RECORDING", p0); }


//////////

	static ScriptAny RequestControlOfNetworkId(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("REQUEST_CONTROL_OF_NETWORK_ID", p0); }
	static ScriptAny RequestMissionAudioBank(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("REQUEST_MISSION_AUDIO_BANK", p0); }
	static void ReserveNetworkMissionObjectsForHost(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESERVE_NETWORK_MISSION_OBJECTS_FOR_HOST", p0); }
	static void ReserveNetworkMissionPeds(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESERVE_NETWORK_MISSION_PEDS", p0); }
	static void ReserveNetworkMissionPedsForHost(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESERVE_NETWORK_MISSION_PEDS_FOR_HOST", p0); }
	static void ReserveNetworkMissionVehicles(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESERVE_NETWORK_MISSION_VEHICLES", p0); }
	static void ReserveNetworkMissionVehiclesForHost(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("RESERVE_NETWORK_MISSION_VEHICLES_FOR_HOST", p0); }
	static void ResetCamInterpCustomSpeedGraph() { NativeInvoke::Invoke<ScriptVoid>("RESET_CAM_INTERP_CUSTOM_SPEED_GRAPH"); }
	static void RestoreScriptArrayFromScratchpad(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("RESTORE_SCRIPT_ARRAY_FROM_SCRATCHPAD", p0, p1, p2, p3); }
	static ScriptAny RestoreScriptValuesForNetworkGame(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("RESTORE_SCRIPT_VALUES_FOR_NETWORK_GAME", p0); }
	static void ResurrectNetworkPlayer(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("RESURRECT_NETWORK_PLAYER", p0, p1, p2, p3, p4); }
	static void SaveFloatToDebugFile(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SAVE_FLOAT_TO_DEBUG_FILE", p0); }
	static void SaveIntToDebugFile(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SAVE_INT_TO_DEBUG_FILE", p0); }
	static void SaveNewlineToDebugFile() { NativeInvoke::Invoke<ScriptVoid>("SAVE_NEWLINE_TO_DEBUG_FILE"); }
	static void SaveScriptArrayInScratchpad(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SAVE_SCRIPT_ARRAY_IN_SCRATCHPAD", p0, p1, p2, p3); }
	static void SaveStringToDebugFile(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SAVE_STRING_TO_DEBUG_FILE", p0); }
	static void SayAmbientSpeechWithVoice(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SAY_AMBIENT_SPEECH_WITH_VOICE", p0, p1, p2, p3, p4, p5); }
	static void ScriptAssert(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SCRIPT_ASSERT", p0); }
	static void ScriptIsMovingMobilePhoneOffscreen(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SCRIPT_IS_MOVING_MOBILE_PHONE_OFFSCREEN", p0); }
	static void ScriptIsUsingMobilePhone(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SCRIPT_IS_USING_MOBILE_PHONE", p0); }
	static void SendNmMessage(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SEND_NM_MESSAGE", p0); }
	static void SetAllCarGeneratorsBackToActive() { NativeInvoke::Invoke<ScriptVoid>("SET_ALL_CAR_GENERATORS_BACK_TO_ACTIVE"); }
	static void SetAllMotionControlPreferencesOnOff(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ALL_MOTION_CONTROL_PREFERENCES_ON_OFF", p0); }
	static void SetAmbientVoiceName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_AMBIENT_VOICE_NAME", p0, p1); }
	static void SetArmourPickupNetworkRegenTime(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ARMOUR_PICKUP_NETWORK_REGEN_TIME", p0); }
	static void SetBit(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_BIT", p0, p1); }
	static void SetBitsInRange(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_BITS_IN_RANGE", p0, p1, p2, p3); }
	static void SetBriansMood(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_BRIANS_MOOD", p0); }
	static void SetCamAttachOffset(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_ATTACH_OFFSET", p0, p1, p2, p3); }
	static void SetCamAttachOffsetIsRelative(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_ATTACH_OFFSET_IS_RELATIVE", p0, p1); }
	static void SetCamComponentShake(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_COMPONENT_SHAKE", p0, p1, p2, p3, p4, p5, p6); }
	static void SetCamDofFocuspoint(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_DOF_FOCUSPOINT", p0, p1, p2, p3, p4); }
	static void SetCamFarClip(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_FAR_CLIP", p0, p1); }
	static void SetCamFarDof(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_FAR_DOF", p0, p1); }
	static void SetCamInheritRollVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_INHERIT_ROLL_VEHICLE", p0, p1); }
	static void SetCamInterpCustomSpeedGraph(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_INTERP_CUSTOM_SPEED_GRAPH", p0); }
	static void SetCamInterpStyleCore(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_INTERP_STYLE_CORE", p0, p1, p2, p3, p4); }
	static void SetCamInterpStyleDetailed(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_INTERP_STYLE_DETAILED", p0, p1, p2, p3, p4); }
	static void SetCamMotionBlur(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_MOTION_BLUR", p0, p1); }
	static void SetCamName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_NAME", p0, p1); }
	static void SetCamNearClip(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_NEAR_CLIP", p0, p1); }
	static void SetCamNearDof(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_NEAR_DOF", p0, p1); }
	static void SetCamPointOffset(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_POINT_OFFSET", p0, p1, p2, p3); }
	static void SetCamPointOffsetIsRelative(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_POINT_OFFSET_IS_RELATIVE", p0, p1); }
	static void SetCamShake(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_SHAKE", p0, p1, p2); }
	static void SetCamSplineDuration(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_SPLINE_DURATION", p0, p1); }
	static void SetCamSplineProgress(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_SPLINE_PROGRESS", p0, p1); }
	static void SetCamSplineSpeedConstant(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_SPLINE_SPEED_CONSTANT", p0, p1); }
	static void SetCamSplineSpeedGraph(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAM_SPLINE_SPEED_GRAPH", p0, p1); }
	static void SetCanBurstCarTyres(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAN_BURST_CAR_TYRES", p0, p1); }
	static void SetCanResprayCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAN_RESPRAY_CAR", p0, p1); }
	static void SetCanTargetCharWithoutLos(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CAN_TARGET_CHAR_WITHOUT_LOS", p0, p1); }
	static void SetCarGeneratorsActiveInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_GENERATORS_ACTIVE_IN_AREA", p0, p1, p2, p3, p4, p5, p6); }
	static void SetCarOnlyDamagedByRelationshipGroup(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_CAR_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP", p0, p1, p2); }
	static void SetCharInCutscene(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_IN_CUTSCENE", p0, p1); }
	static void SetCharNameDebug(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_NAME_DEBUG", p0, p1); }
	static void SetCharOnlyDamagedByRelationshipGroup(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_CHAR_ONLY_DAMAGED_BY_RELATIONSHIP_GROUP", p0, p1, p2); }
	static void SetCinematicButtonEnabled(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_CINEMATIC_BUTTON_ENABLED", p0); }
	static void SetCutsceneExtraRoomPos(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_CUTSCENE_EXTRA_ROOM_POS", p0, p1, p2); }
	static void SetFollowPedPitchLimitDown(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_FOLLOW_PED_PITCH_LIMIT_DOWN", p0); }
	static void SetFollowPedPitchLimitUp(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_FOLLOW_PED_PITCH_LIMIT_UP", p0); }
	static void SetFollowVehicleCamOffset(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_FOLLOW_VEHICLE_CAM_OFFSET", p0, p1, p2, p3); }
	static void SetFollowVehicleCamSubmode(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_FOLLOW_VEHICLE_CAM_SUBMODE", p0); }
	static void SetFreeResprays(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_FREE_RESPRAYS", p0); }
	static void SetFreebiesInVehicle(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_FREEBIES_IN_VEHICLE", p0, p1); }
	static void SetGameCamHeading(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_GAME_CAM_HEADING", p0); }
	static void SetGameCamPitch(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_GAME_CAM_PITCH", p0); }
	static void SetGarageLeaveCameraAlone(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_GARAGE_LEAVE_CAMERA_ALONE", p0, p1); }
	static void SetGlobalInstancePriority(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_GLOBAL_INSTANCE_PRIORITY", p0); }
	static void SetGlobalRenderFlags(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_GLOBAL_RENDER_FLAGS", p0, p1, p2, p3); }
	static void SetHealthPickupNetworkRegenTime(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_HEALTH_PICKUP_NETWORK_REGEN_TIME", p0); }
	static void SetHelpMessageBoxSize(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_HELP_MESSAGE_BOX_SIZE", p0); }
	static void SetHintFov(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_HINT_FOV", p0); }
	static void SetHintTimes(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_HINT_TIMES", p0, p1, p2); }
	static void SetHintTimesDefault() { NativeInvoke::Invoke<ScriptVoid>("SET_HINT_TIMES_DEFAULT"); }
	static void SetInMPTutorial(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_IN_MP_TUTORIAL", p0); }
	static void SetInSpectatorMode(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_IN_SPECTATOR_MODE", p0); }
	static void SetInformRespectedFriends(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_INFORM_RESPECTED_FRIENDS", p0, p1, p2); }
	static void SetInterpFromScriptToGame(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_INTERP_FROM_SCRIPT_TO_GAME", p0, p1); }
	static void SetInterpInOutVehicleEnabledThisFrame(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_INTERP_IN_OUT_VEHICLE_ENABLED_THIS_FRAME", p0); }
	static void SetLoadCollisionForCarFlag(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_LOAD_COLLISION_FOR_CAR_FLAG", p0, p1); }
	static void SetLoadCollisionForCharFlag(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_LOAD_COLLISION_FOR_CHAR_FLAG", p0, p1); }
	static void SetLobbyMuteOverride(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_LOBBY_MUTE_OVERRIDE", p0); }
	static void SetLoudVehicleRadio(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_LOUD_VEHICLE_RADIO", p0, p1); }
	static void SetMask(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_MASK", p0, p1, p2, p3); }
	static void SetMessagesWaiting(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MESSAGES_WAITING", p0); }
	static void SetMsgForLoadingScreen(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MSG_FOR_LOADING_SCREEN", p0); }
	static void SetMultiplayerHUDCash(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MULTIPLAYER_HUD_CASH", p0); }
	static void SetMultiplayerHUDTime(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_MULTIPLAYER_HUD_TIME", p0); }
	static void SetNetworkIdCanMigrate(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NETWORK_ID_CAN_MIGRATE", p0, p1); }
	static void SetNetworkIdExistsOnAllMachines(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES", p0, p1); }
	static void SetNetworkIdStopCloning(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NETWORK_ID_STOP_CLONING", p0, p1); }
	static void SetNetworkPlayerAsVip(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NETWORK_PLAYER_AS_VIP", p0, p1); }
	static void SetNetworkVehicleRespotTimer(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NETWORK_VEHICLE_RESPOT_TIMER", p0, p1); }
	static void SetNmAnimPose(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_ANIM_POSE", p0, p1, p2, p3); }
	static void SetNmMessageBool(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_BOOL", p0, p1); }
	static void SetNmMessageFloat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_FLOAT", p0, p1); }
	static void SetNmMessageInstanceIndex(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_INSTANCE_INDEX", p0, p1, p2, p3); }
	static void SetNmMessageInt(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_INT", p0, p1); }
	static void SetNmMessageString(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_STRING", p0, p1); }
	static void SetNmMessageVec3(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_NM_MESSAGE_VEC3", p0, p1, p2, p3); }
	static void SetObjectProofs(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_PROOFS", p0, p1, p2, p3, p4, p5); }
	static void SetObjectRecordsCollisions(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_OBJECT_RECORDS_COLLISIONS", p0, p1); }
	static void SetOverrideNoSprintingOnPhoneInMultiplayer(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_OVERRIDE_NO_SPRINTING_ON_PHONE_IN_MULTIPLAYER", p0); }
	static void SetPedWithBrainCanBeConvertedToDummyPed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PED_WITH_BRAIN_CAN_BE_CONVERTED_TO_DUMMY_PED", p0, p1); }
	static void SetPlaybackSpeed(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYBACK_SPEED", p0, p1); }
	static void SetPlayerControlForNetwork(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CONTROL_FOR_NETWORK", p0, p1, p2); }
	static void SetPlayerControlOnInMissionCleanup(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_CONTROL_ON_IN_MISSION_CLEANUP", p0); }
	static void SetPlayerIconColour(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_ICON_COLOUR", p0); }
	static void SetPlayerPlayerTargetting(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_PLAYER_TARGETTING", p0); }
	static void SetPlayerTeam(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYER_TEAM", p0, p1); }
	static void SetPlayersDropMoneyInNetworkGame(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PLAYERS_DROP_MONEY_IN_NETWORK_GAME", p0); }
	static void SetPtfxCamInsideVehicle(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_PTFX_CAM_INSIDE_VEHICLE", p0); }
	static void SetRichPresenceTemplateLobby(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATELOBBY", p0); }
	static void SetRichPresenceTemplateMP1(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP1", p0, p1, p2, p3); }
	static void SetRichPresenceTemplateMP2(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP2", p0); }
	static void SetRichPresenceTemplateMP3(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP3", p0, p1); }
	static void SetRichPresenceTemplateMP4(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP4", p0, p1); }
	static void SetRichPresenceTemplateMP5(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP5", p0, p1, p2); }
	static void SetRichPresenceTemplateMP6(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEMP6", p0, p1, p2); }
	static void SetRichPresenceTemplateParty() { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATEPARTY"); }
	static void SetRichPresenceTemplateSP1(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATESP1", p0, p1, p2); }
	static void SetRichPresenceTemplateSP2(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_RICH_PRESENCE_TEMPLATESP2", p0); }
	static void SetRomansMood(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_ROMANS_MOOD", p0); }
	static void SetRoomForViewportByKey(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ROOM_FOR_VIEWPORT_BY_KEY", p0, p1); }
	static void SetRoomForViewportByName(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_ROOM_FOR_VIEWPORT_BY_NAME", p0, p1); }
	static void SetScenarioPedDensityMultiplier(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_SCENARIO_PED_DENSITY_MULTIPLIER", p0, p1); }
	static void SetScriptedAnimSeatOffset(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_SCRIPTED_ANIM_SEAT_OFFSET", p0, p1); }
	static void SetScriptedConversionCentre(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_SCRIPTED_CONVERSION_CENTRE", p0, p1, p2); }
	static void SetSequenceToRepeat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_SEQUENCE_TO_REPEAT", p0, p1); }
	static void SetSpecificPassengerIndexToUseInGroups(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_SPECIFIC_PASSENGER_INDEX_TO_USE_IN_GROUPS", p0, p1); }
	static void SetSpritesDrawBeforeFade(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_SPRITES_DRAW_BEFORE_FADE", p0); }
	static void SetStatFrontEndVisibility(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_STAT_FRONTEND_VISIBILITY", p0, p1); }
	static void SetThisMachineRunningServerScript(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_THIS_MACHINE_RUNNING_SERVER_SCRIPT", p0); }
	static void SetTimecycleModifier(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_TIMECYCLE_MODIFIER", p0); }
	static void SetUsePoolGamePhysicsSettings(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SET_USE_POOL_GAME_PHYSICS_SETTINGS", p0); }
	static void SetUsesCollisionOfClosestObjectOfType(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_USES_COLLISION_OF_CLOSEST_OBJECT_OF_TYPE", p0, p1, p2, p3, p4, p5); }
	static void SetViewportPriority(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_VIEWPORT_PRIORITY", p0, p1); }
	static void SetVisibilityOfClosestObjectOfType(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SET_VISIBILITY_OF_CLOSEST_OBJECT_OF_TYPE", p0, p1, p2, p3, p4, p5); }
	static void SetVoiceIdFromHeadComponent(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_VOICE_ID_FROM_HEAD_COMPONENT", p0, p1, p2); }
	static void SetWeaponPickupNetworkRegenTime(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_WEAPON_PICKUP_NETWORK_REGEN_TIME", p0, p1); }
	static void SetWebPageLinkActive(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("SET_WEB_PAGE_LINK_ACTIVE", p0, p1, p2); }
	static void SetWebPageScroll(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SET_WEB_PAGE_SCROLL", p0, p1); }
	static void ShowSigninUi() { NativeInvoke::Invoke<ScriptVoid>("SHOW_SIGNIN_UI"); }
	static void ShutdownAndLaunchNetworkGame(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SHUTDOWN_AND_LAUNCH_NETWORK_GAME", p0); }
	static void ShutdownAndLaunchSinglePlayerGame() { NativeInvoke::Invoke<ScriptVoid>("SHUTDOWN_AND_LAUNCH_SINGLE_PLAYER_GAME"); }
	static void SkipTimeInPlaybackRecordedCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SKIP_TIME_IN_PLAYBACK_RECORDED_CAR", p0, p1); }
	static void SkipToEndAndStopPlaybackRecordedCar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SKIP_TO_END_AND_STOP_PLAYBACK_RECORDED_CAR", p0); }
	static void SkipToNextScriptedConversationLine() { NativeInvoke::Invoke<ScriptVoid>("SKIP_TO_NEXT_SCRIPTED_CONVERSATION_LINE"); }
	static void StartCredits() { NativeInvoke::Invoke<ScriptVoid>("START_CREDITS"); }
	static void StartCustomMobilePhoneRinging(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("START_CUSTOM_MOBILE_PHONE_RINGING", p0); }
	static void StartCutscene() { NativeInvoke::Invoke<ScriptVoid>("START_CUTSCENE"); }
	static void StartCutsceneNow(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("START_CUTSCENE_NOW", p0); }
	static void StartEndCreditsMusic() { NativeInvoke::Invoke<ScriptVoid>("START_END_CREDITS_MUSIC"); }
	static void StartGpsRaceTrack(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("START_GPS_RACE_TRACK", p0); }
	static void StartLoadScene(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("START_LOAD_SCENE", p0, p1, p2); }
	static void StartMobilePhoneCall(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("START_MOBILE_PHONE_CALL", p0, p1, p2, p3, p4, p5); }
	static void StartMobilePhoneCalling() { NativeInvoke::Invoke<ScriptVoid>("START_MOBILE_PHONE_CALLING"); }
	static void StartNewWidgetCombo() { NativeInvoke::Invoke<ScriptVoid>("START_NEW_WIDGET_COMBO"); }
	static void StartPlaybackRecordedCar(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("START_PLAYBACK_RECORDED_CAR", p0, p1); }
	static void StartPlaybackRecordedCarUsingAi(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("START_PLAYBACK_RECORDED_CAR_USING_AI", p0, p1); }
	static ScriptAny StartPtfx(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("START_PTFX", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny StartPtfxOnObj(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("START_PTFX_ON_OBJ", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny StartPtfxOnPedBone(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { return NativeInvoke::Invoke<ScriptAny>("START_PTFX_ON_PED_BONE", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static ScriptAny StartPtfxOnVeh(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("START_PTFX_ON_VEH", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void StartScriptConversation(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("START_SCRIPT_CONVERSATION", p0, p1); }
	static void StopCredits() { NativeInvoke::Invoke<ScriptVoid>("STOP_CREDITS"); }
	static void StopEndCreditsMusic() { NativeInvoke::Invoke<ScriptVoid>("STOP_END_CREDITS_MUSIC"); }
	static void StopMobilePhoneRinging() { NativeInvoke::Invoke<ScriptVoid>("STOP_MOBILE_PHONE_RINGING"); }
	static void StopPlaybackRecordedCar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STOP_PLAYBACK_RECORDED_CAR", p0); }
	static void StopPreviewRingtone() { NativeInvoke::Invoke<ScriptVoid>("STOP_PREVIEW_RINGTONE"); }
	static void StopPtfx(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STOP_PTFX", p0); }
	static void StopRecordingCar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STOP_RECORDING_CAR", p0); }
	static void StopStream() { NativeInvoke::Invoke<ScriptVoid>("STOP_STREAM"); }
	static void StoreCarCharIsInNoSave(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("STORE_CAR_CHAR_IS_IN_NO_SAVE", p0, p1); }
	static ScriptAny StoreDamageTrackerForNetworkPlayer(ScriptAny p0, ScriptAny p1, ScriptAny p2) { return NativeInvoke::Invoke<ScriptAny>("STORE_DAMAGE_TRACKER_FOR_NETWORK_PLAYER", p0, p1, p2); }
	static void StoreScriptValuesForNetworkGame(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("STORE_SCRIPT_VALUES_FOR_NETWORK_GAME", p0); }
	static void StreamCutscene() { NativeInvoke::Invoke<ScriptVoid>("STREAM_CUTSCENE"); }
	static ScriptAny StringToInt(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("STRING_TO_INT", p0, p1); }
	static void SuppressFadeInAfterDeathArrest(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SUPPRESS_FADE_IN_AFTER_DEATH_ARREST", p0); }
	static void SwitchCarGenerator(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_CAR_GENERATOR", p0, p1); }
	static void SwitchOffWaypoint() { NativeInvoke::Invoke<ScriptVoid>("SWITCH_OFF_WAYPOINT"); }
	static void SwitchPedPathsOff(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_PED_PATHS_OFF", p0, p1, p2, p3, p4, p5); }
	static void SwitchPedPathsOn(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_PED_PATHS_ON", p0, p1, p2, p3, p4, p5); }
	static void SwitchPedRoadsBackToOriginal(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_PED_ROADS_BACK_TO_ORIGINAL", p0, p1, p2, p3, p4, p5); }
	static void SwitchStreaming(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("SWITCH_STREAMING", p0); }
	static void TakeCarOutOfParkedCarsBudget(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TAKE_CAR_OUT_OF_PARKED_CARS_BUDGET", p0, p1); }
	static void TaskCarDriveToCoord(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_DRIVE_TO_COORD", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static void TaskCarDriveToCoordNotAgainstTraffic(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { NativeInvoke::Invoke<ScriptVoid>("TASK_CAR_DRIVE_TO_COORD_NOT_AGAINST_TRAFFIC", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static void TaskCharSlideToCoord(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_CHAR_SLIDE_TO_COORD", p0, p1, p2, p3, p4, p5); }
	static void TaskCharSlideToCoordAndPlayAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13) { NativeInvoke::Invoke<ScriptVoid>("TASK_CHAR_SLIDE_TO_COORD_AND_PLAY_ANIM", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); }
	static void TaskCharSlideToCoordHdgRate(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("TASK_CHAR_SLIDE_TO_COORD_HDG_RATE", p0, p1, p2, p3, p4, p5, p6); }
	static void TaskChatWithChar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_CHAT_WITH_CHAR", p0, p1, p2, p3); }
	static void TaskClearLookAt(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TASK_CLEAR_LOOK_AT", p0); }
	static void TaskClimb(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_CLIMB", p0, p1); }
	static void TaskClimbLadder(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_CLIMB_LADDER", p0, p1); }
	static void TaskCombatHatedTargetsInArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT_HATED_TARGETS_IN_AREA", p0, p1, p2, p3, p4); }
	static void TaskCombatRoll(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_COMBAT_ROLL", p0, p1); }
	static void TaskCower(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TASK_COWER", p0); }
	static void TaskDead(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TASK_DEAD", p0); }
	static void TaskDriveBy(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("TASK_DRIVE_BY", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void TaskDrivePointRoute(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_DRIVE_POINT_ROUTE", p0, p1, p2); }
	static void TaskDrivePointRouteAdvanced(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_DRIVE_POINT_ROUTE_ADVANCED", p0, p1, p2, p3, p4, p5); }
	static void TaskDuck(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_DUCK", p0, p1); }
	static void TaskExtendRoute(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_EXTEND_ROUTE", p0, p1, p2); }
	static void TaskFleeCharAnyMeans(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("TASK_FLEE_CHAR_ANY_MEANS", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void TaskFlushRoute() { NativeInvoke::Invoke<ScriptVoid>("TASK_FLUSH_ROUTE"); }
	static void TaskFollowNavMeshAndSlideToCoord(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("TASK_FOLLOW_NAV_MESH_AND_SLIDE_TO_COORD", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void TaskFollowPointRoute(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_FOLLOW_POINT_ROUTE", p0, p1, p2); }
	static void TaskGetOffBoat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_GET_OFF_BOAT", p0, p1); }
	static void TaskGoStraightToCoordRelativeToCar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_STRAIGHT_TO_COORD_RELATIVE_TO_CAR", p0, p1, p2, p3, p4, p5, p6); }
	static void TaskGoToChar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_TO_CHAR", p0, p1, p2, p3); }
	static void TaskGoToCoordAnyMeans(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_TO_COORD_ANY_MEANS", p0, p1, p2, p3, p4, p5); }
	static void TaskGoToCoordWhileAiming(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_TO_COORD_WHILE_AIMING", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
	static void TaskGoToCoordWhileShooting(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_TO_COORD_WHILE_SHOOTING", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskGoToObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_GO_TO_OBJECT", p0, p1, p2, p3); }
	static void TaskGotoCar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_GOTO_CAR", p0, p1, p2, p3); }
	static void TaskGuardAngledDefensiveArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11, ScriptAny p12, ScriptAny p13) { NativeInvoke::Invoke<ScriptVoid>("TASK_GUARD_ANGLED_DEFENSIVE_AREA", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13); }
	static void TaskGuardAssignedDefensiveArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("TASK_GUARD_ASSIGNED_DEFENSIVE_AREA", p0, p1, p2, p3, p4, p5, p6); }
	static void TaskGuardSphereDefensiveArea(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10) { NativeInvoke::Invoke<ScriptVoid>("TASK_GUARD_SPHERE_DEFENSIVE_AREA", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); }
	static void TaskLeaveCarAndFlee(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_CAR_AND_FLEE", p0, p1, p2, p3, p4); }
	static void TaskLeaveCarInDirection(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_LEAVE_CAR_IN_DIRECTION", p0, p1, p2); }
	static void TaskMobileConversation(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_MOBILE_CONVERSATION", p0, p1); }
	static void TaskPerformSequenceFromProgress(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_PERFORM_SEQUENCE_FROM_PROGRESS", p0, p1, p2, p3); }
	static void TaskPerformSequenceLocally(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_PERFORM_SEQUENCE_LOCALLY", p0, p1); }
	static void TaskPickupAndCarryObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_PICKUP_AND_CARRY_OBJECT", p0, p1, p2, p3, p4, p5); }
	static void TaskPlayAnim(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimFacial(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_FACIAL", p0, p1, p2, p3, p4, p5, p6); }
	static void TaskPlayAnimNonInterruptable(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_NON_INTERRUPTABLE", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimReadyToBeExecuted(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_READY_TO_BE_EXECUTED", p0, p1, p2, p3); }
	static void TaskPlayAnimSecondary(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_SECONDARY", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimSecondaryInCar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_SECONDARY_IN_CAR", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimSecondaryNoInterrupt(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_SECONDARY_NO_INTERRUPT", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimSecondaryUpperBody(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_SECONDARY_UPPER_BODY", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimUpperBody(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_UPPER_BODY", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TaskPlayAnimWithAdvancedFlags(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9, ScriptAny p10, ScriptAny p11) { NativeInvoke::Invoke<ScriptVoid>("TASK_PLAY_ANIM_WITH_ADVANCED_FLAGS", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); }
	static void TaskPutCharDirectlyIntoCover(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_PUT_CHAR_DIRECTLY_INTO_COVER", p0, p1, p2, p3, p4); }
	static void TaskSeekCoverFromPed(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_SEEK_COVER_FROM_PED", p0, p1, p2); }
	static void TaskSeekCoverFromPos(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_SEEK_COVER_FROM_POS", p0, p1, p2, p3, p4); }
	static void TaskSeekCoverToCoords(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("TASK_SEEK_COVER_TO_COORDS", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void TaskSeekCoverToCoverPoint(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_SEEK_COVER_TO_COVER_POINT", p0, p1, p2, p3, p4, p5); }
	static void TaskSetIgnoreWeaponRangeFlag(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_SET_IGNORE_WEAPON_RANGE_FLAG", p0, p1); }
	static void TaskShimmy(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_SHIMMY", p0, p1); }
	static ScriptAny TaskShimmyInDirection(ScriptAny p0, ScriptAny p1) { return NativeInvoke::Invoke<ScriptAny>("TASK_SHIMMY_IN_DIRECTION", p0, p1); }
	static ScriptAny TaskShimmyLetGo(ScriptAny p0) { return NativeInvoke::Invoke<ScriptAny>("TASK_SHIMMY_LET_GO", p0); }
	static void TaskShootAtChar(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_SHOOT_AT_CHAR", p0, p1, p2, p3); }
	static void TaskShuffleToNextCarSeat(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TASK_SHUFFLE_TO_NEXT_CAR_SEAT", p0, p1); }
	static void TaskSitDown(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_SIT_DOWN", p0, p1, p2, p3); }
	static void TaskSitDownInstantly(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("TASK_SIT_DOWN_INSTANTLY", p0, p1, p2, p3); }
	static void TaskSitDownOnNearestObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("TASK_SIT_DOWN_ON_NEAREST_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void TaskSitDownOnObject(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { NativeInvoke::Invoke<ScriptVoid>("TASK_SIT_DOWN_ON_OBJECT", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static void TaskSitDownOnSeat(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("TASK_SIT_DOWN_ON_SEAT", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void TaskStandGuard(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { NativeInvoke::Invoke<ScriptVoid>("TASK_STAND_GUARD", p0, p1, p2, p3, p4, p5, p6, p7); }
	static void TaskStartScenarioAtPosition(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5) { NativeInvoke::Invoke<ScriptVoid>("TASK_START_SCENARIO_AT_POSITION", p0, p1, p2, p3, p4, p5); }
	static void TaskStartScenarioInPlace(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("TASK_START_SCENARIO_IN_PLACE", p0, p1, p2); }
	static void TaskUseNearestScenarioToPos(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_USE_NEAREST_SCENARIO_TO_POS", p0, p1, p2, p3, p4); }
	static void TaskUseNearestScenarioToPosWarp(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4) { NativeInvoke::Invoke<ScriptVoid>("TASK_USE_NEAREST_SCENARIO_TO_POS_WARP", p0, p1, p2, p3, p4); }
	static void TellNetPlayerToStartPlaying(ScriptAny p0, ScriptAny p1) { NativeInvoke::Invoke<ScriptVoid>("TELL_NET_PLAYER_TO_START_PLAYING", p0, p1); }
	static void ThisScriptIsSafeForNetworkGame() { NativeInvoke::Invoke<ScriptVoid>("THIS_SCRIPT_IS_SAFE_FOR_NETWORK_GAME"); }
	static void ThisScriptShouldBeSaved() { NativeInvoke::Invoke<ScriptVoid>("THIS_SCRIPT_SHOULD_BE_SAVED"); }
	static void TrainLeaveStation(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TRAIN_LEAVE_STATION", p0); }
	static void TriggerMissionCompleteAudio(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TRIGGER_MISSION_COMPLETE_AUDIO", p0); }
	static void TriggerPoliceReport(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("TRIGGER_POLICE_REPORT", p0); }
	static ScriptAny TriggerPtfx(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7) { return NativeInvoke::Invoke<ScriptAny>("TRIGGER_PTFX", p0, p1, p2, p3, p4, p5, p6, p7); }
	static ScriptAny TriggerPtfxOnObj(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("TRIGGER_PTFX_ON_OBJ", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static ScriptAny TriggerPtfxOnPedBone(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8, ScriptAny p9) { return NativeInvoke::Invoke<ScriptAny>("TRIGGER_PTFX_ON_PED_BONE", p0, p1, p2, p3, p4, p5, p6, p7, p8, p9); }
	static ScriptAny TriggerPtfxOnVeh(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6, ScriptAny p7, ScriptAny p8) { return NativeInvoke::Invoke<ScriptAny>("TRIGGER_PTFX_ON_VEH", p0, p1, p2, p3, p4, p5, p6, p7, p8); }
	static void TurnOffRadioHUBInLobby() { NativeInvoke::Invoke<ScriptVoid>("TURN_OFF_RADIOHUD_IN_LOBBY"); }
	static void UnattachCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("UNATTACH_CAM", p0); }
	static void UnfreezeRadioStation(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("UNFREEZE_RADIO_STATION", p0); }
	static void UnlockMissionNewsStory(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("UNLOCK_MISSION_NEWS_STORY", p0); }
	static void UnpausePlaybackRecordedCar(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("UNPAUSE_PLAYBACK_RECORDED_CAR", p0); }
	static void UnpointCam(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("UNPOINT_CAM", p0); }
	static void UpdateNetworkRelativeScore(ScriptAny p0, ScriptAny p1, ScriptAny p2) { NativeInvoke::Invoke<ScriptVoid>("UPDATE_NETWORK_RELATIVE_SCORE", p0, p1, p2); }
	static void UpdateNetworkStatistics(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3) { NativeInvoke::Invoke<ScriptVoid>("UPDATE_NETWORK_STATISTICS", p0, p1, p2, p3); }
	static void UpdatePtfxOffsets(ScriptAny p0, ScriptAny p1, ScriptAny p2, ScriptAny p3, ScriptAny p4, ScriptAny p5, ScriptAny p6) { NativeInvoke::Invoke<ScriptVoid>("UPDATE_PTFX_OFFSETS", p0, p1, p2, p3, p4, p5, p6); }
	static void UseMask(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("USE_MASK", p0); }
	static void UsePlayerColourInsteadOfTeamColour(ScriptAny p0) { NativeInvoke::Invoke<ScriptVoid>("USE_PLAYER_COLOUR_INSTEAD_OF_TEAM_COLOUR", p0); }
	static void UsePreviousFontSettings() { NativeInvoke::Invoke<ScriptVoid>("USE_PREVIOUS_FONT_SETTINGS"); }
}

/*
#define GTA40
#if defined GTA40
// 1.0.4.0

// unsigned int
static unsigned int (__cdecl * GetPlayerId)() = (unsigned int (__cdecl *)())(0x9F6110 + dwLoadOffset);
static void (__cdecl * GetPlayerPed)(unsigned int, Ped*) = (void (__cdecl *)(unsigned int, Ped*))(0x9F7ED0 + dwLoadOffset);
static unsigned int (__cdecl * ConvertIntToPlayerIndex)(unsigned int) = (unsigned int (__cdecl *)(unsigned int))(0x9F6140 + dwLoadOffset);
static void (__cdecl * AddScore)(unsigned int, int) = (void(__cdecl *)(unsigned int, int))(0x9F4E80 + dwLoadOffset);
static void (__cdecl * ChangePlayerModel)(unsigned int, eModel) = (void (__cdecl *)(unsigned int, eModel))(0x9F4C00 + dwLoadOffset);
static void (__cdecl * RegisterPlayerRespawnCoords)(int pl, float, float, float) = (void (__cdecl*)(int, float, float, float))(0x9CE6D0+dwLoadOffset);
// char
static bool (__cdecl * DoesCharExist)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FE1A0 + dwLoadOffset);
static void (__cdecl * CreateChar)(unsigned int,eModel,float,float,float,Ped*,bool) = (void(__cdecl *)(unsigned int,eModel,float,float,float,Ped*,bool))(0xA05CD0 + dwLoadOffset);
static void (__cdecl * GetCharCoordinates)(Ped, float*, float*, float*) = (void(__cdecl *)(Ped, float*, float*, float*))(0x9FB910 + dwLoadOffset);
static void (__cdecl * SetCharCoordinates)(Ped, float, float, float) = (void(__cdecl *)(Ped, float, float, float))(0x9FB970 + dwLoadOffset);
static void (__cdecl * AddArmourToChar)(Ped, float) = (void (__cdecl *)(Ped, float))(0x9FD190 + dwLoadOffset);
static unsigned int (__cdecl * GetCharMoney)(Ped) = (unsigned int (__cdecl *)(Ped))(0x9FD580 + dwLoadOffset);
static void (__cdecl * RemoveWeaponFromChar)(Ped, int) = (void(__cdecl *)(Ped, int))(0x9CD2D0 + dwLoadOffset);
static bool (__cdecl * IsCharInAnyCar)(Ped) = (bool(__cdecl *)(Ped))(0x9FBB40 + dwLoadOffset);
static void (__cdecl * GetCharCar)(Ped, Vehicle*) = (void(__cdecl *)(Ped, Vehicle*))(0xA071A0 + dwLoadOffset);
static void (__cdecl * RemoveAllCharWeapons)(Ped) = (void(__cdecl *)(Ped))(0x9CD320 + dwLoadOffset);
static void (__cdecl * SetCharMoney)(Ped, int) = (void(__cdecl *)(Ped, int))(0x9FD550 + dwLoadOffset);
static void (__cdecl * DeleteChar)(Ped*) = (void(__cdecl *)(Ped*))(0x9Fb830 + dwLoadOffset);
static bool (__cdecl * IsCharShooting)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FCF40 + dwLoadOffset);
static bool (__cdecl * IsCharInAir)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FF2D0 + dwLoadOffset);
static bool (__cdecl * IsCharDucking)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FE260 + dwLoadOffset);
static bool (__cdecl * IsCharDead)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FC280 + dwLoadOffset);
static bool (__cdecl * IsCharSittingInAnyCar)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FD910 + dwLoadOffset);
static bool (__cdecl * IsCharOnFoot)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FD970 + dwLoadOffset);
static bool (__cdecl * IsCharGettingInToACar)(Ped ped) = (bool(__cdecl *)(Ped))(0x9FFD20 + dwLoadOffset);
static void (__cdecl * GetCharHeading)(Ped, float*) = (void(__cdecl *)(Ped, float*))(0x9FC3C0 + dwLoadOffset);
static void (__cdecl * SetCharHeading)(Ped, float) = (void(__cdecl *)(Ped, float))(0x9FC490 + dwLoadOffset);
static void (__cdecl * GivePedFakeNetworkName)(Ped,char*, int r, int g, int b, int a) = (void(__cdecl *)(Ped, char*, int, int, int, int))(0xA02390 + dwLoadOffset);
static int (__cdecl * GetCurrentCharWeapon)(Ped, int*) = (int (__cdecl*)(Ped, int*))(0x9CD060+dwLoadOffset);
static void (__cdecl * FreezeCharPosition)(Ped, bool) = (void (__cdecl*)(Ped, bool))(0x9FDDA0+dwLoadOffset);
static void (__cdecl * SetCharDefaultComponentVariation)(Ped) = (void (__cdecl*)(Ped))(0xA00130+dwLoadOffset);
static void (__cdecl * SetCurrentCharWeapon)(Ped ped, eWeapon w, bool unkTrue) = (void (__cdecl*)(Ped, eWeapon, bool))(0x9CD060+dwLoadOffset);
static void (__cdecl * GiveWeaponToChar)(Ped, eWeapon, int, bool unk0) = (void (__cdecl*)(Ped, eWeapon, int, bool))(0x9CCEF0+dwLoadOffset);
// Car
static bool (__cdecl * DoesVehicleExist)(Vehicle car) = (bool(__cdecl *)(Vehicle))(0x9DFDC0 + dwLoadOffset);
static void (__cdecl * CreateCar)(unsigned int,float,float,float,Vehicle*,bool) = (void(__cdecl *)(unsigned int,float,float,float,Vehicle*,bool))(0x9E4C90 + dwLoadOffset);
static bool (__cdecl * IsModelCar)(eModel) = (bool(__cdecl *)(eModel))(0x9DD670 + dwLoadOffset);
static void (__cdecl * GetCarCoordinates)(Vehicle, float*, float*, float*) = (void(__cdecl *)(Vehicle, float*, float*, float*))(0x9DE420 + dwLoadOffset);
static void (__cdecl * SetCarCoordinates)(Vehicle, float, float, float) = (void(__cdecl *)(Vehicle, float, float, float))(0x9DE4D0 + dwLoadOffset);
static void (__cdecl * GetCarHeading)(Vehicle, float*) = (void(__cdecl *)(Vehicle, float*))(0x9DE710 + dwLoadOffset);
static void (__cdecl * SetCarHeading)(Vehicle, float) = (void(__cdecl *)(Vehicle, float))(0x9DE790 + dwLoadOffset);
static bool (__cdecl * IsCarStopped)(Vehicle) = (bool(__cdecl *)(Vehicle))(0x9DEA20 + dwLoadOffset);
static void (__cdecl * GetCarSpeedVector)(Vehicle, Vector3*, bool) = (void(__cdecl *)(Vehicle, Vector3*, bool))(0x9E01E0 + dwLoadOffset);
static void (__cdecl * GetMaximumNumberOfPassengers)(Vehicle, int*) = (void(__cdecl *)(Vehicle, int*))(0x9DEB00 + dwLoadOffset);
static void (__cdecl * GetCarSpeed)(Vehicle, float*) = (void(__cdecl *)(Vehicle, float*))(0x9DF0E0 + dwLoadOffset);
// Game 5B52D1DE
static int (__cdecl * PrintStringWithLiteralStringNow)(const char*, const char*, unsigned int, unsigned int) = (int (__cdecl *)(const char*, const char*, unsigned int, unsigned int))(0xA10FE0 + dwLoadOffset);
static void (__cdecl * RequestModel)(eModel) = (void(__cdecl *)(eModel))(0x9D1930 + dwLoadOffset);
static int (__cdecl * HasModelLoaded)(eModel) = (int(__cdecl *)(eModel))(0x9D1970 + dwLoadOffset);
static bool (__cdecl * MarkModelAsNoLongerNeeded)(eModel) = (bool(__cdecl *)(eModel))(0x9D1AC0 + dwLoadOffset);
static void (__cdecl * Pause)() = (void(__cdecl *)())(0xA0D850 + dwLoadOffset);
static void (__cdecl * Unpause)() = (void(__cdecl *)())(0xA0D870 + dwLoadOffset);
static void (__cdecl * TerminateAllScriptsWithThisName)(char *script) = (void(__cdecl *)(char*))(0xA0CF10 + dwLoadOffset);
static void (__cdecl * SetFloatStat)(eFloatStatistic stat, float value) = (void(__cdecl *)(eFloatStatistic stat, float value))(0x9D2AF0 + dwLoadOffset);
static void (__cdecl * SetIntStat)(eIntStatistic stat, float value) = (void(__cdecl *)(eIntStatistic stat, float value))(0x9D2AC0 + dwLoadOffset);
static void (__cdecl * AllowEmergencyServices)(bool) = (void(__cdecl *)(bool))(0x4B6EF0 + dwLoadOffset);
static void (__cdecl * SetPedDensityMultiplier)(bool) = (void(__cdecl *)(bool))(0x9F8E20 + dwLoadOffset);
static void (__cdecl * SetCarDensityMultiplier)(bool) = (void(__cdecl *)(bool))(0x9DD0F0 + dwLoadOffset);
static void (__cdecl * DisableCarGenerators)(bool) = (void(__cdecl *)(bool))(0x9DD0A0 + dwLoadOffset);
static void (__cdecl * DisableCarGeneratorsWithHeli)(bool) = (void(__cdecl *)(bool))(0x9DD0C0 + dwLoadOffset);
static void (__cdecl * SwitchPoliceHelis)(bool) = (void(__cdecl *)(bool))(0x9DD780 + dwLoadOffset);
static void (__cdecl * SetSleepModeActive)(bool) = (void(__cdecl *)(bool))(0xA0D6E0 + dwLoadOffset);
static void (__cdecl * SetTimeOfDay)(int, int) = (void(__cdecl *)(int, int))(0x9CB170 + dwLoadOffset);
static int (__cdecl * IsControlPressed)(int, int) = (int(__cdecl *)(int, int))(0xA0B050 + dwLoadOffset);
*/

/*
// Camera
static void (__cdecl * GetGameCam)(Camera *cam) = (void (__cdecl *)(Camera*))(0xA19860 + dwLoadOffset);
static void (__cdecl * GetCamPos)(Camera cam, float *x, float *y, float *z) = (void(__cdecl *)(Camera, float*, float*, float*))(0xA19040 + dwLoadOffset);
static void (__cdecl * GetCamRot)(Camera cam, float *x, float *y, float *z) = (void(__cdecl *)(Camera, float*, float*, float*))(0xA19090 + dwLoadOffset);
// Task
static void (__cdecl * ClearCharTask)(Ped ped) = (void(__cdecl *)(Ped))(0x9E9660 + dwLoadOffset);
static void (__cdecl * TaskGoStraightToCoord)(Ped, float x, float y, float z, int ms_2_walk_4_sprint, int unk_45000) = (void(__cdecl *)(Ped, float, float, float, int, int))(0x9Eb820 + dwLoadOffset);
static void (__cdecl * TaskToggleDuck)(Ped ped, bool duck) = (void(__cdecl *)(Ped, bool))(0x9EABA0 + dwLoadOffset);
static void (__cdecl * TaskShootAtCoord)(Ped ped, float x, float y, float z, int Unknown0_5k, int Unknown1_5) = (void(__cdecl *)(Ped, float, float, float, int, int))(0x9ED3E0 + dwLoadOffset);
static void (__cdecl * TaskJump)(Ped ped, bool jump) = (void(__cdecl *)(Ped, bool))(0x9EADC0 + dwLoadOffset);
static void (__cdecl * TaskSwapWeapon)(Ped ped, eWeapon weapon) = (void(__cdecl *)(Ped, eWeapon))(0x9F0D60 + dwLoadOffset);
static void (__cdecl * TaskLeaveAnyCar)(Ped ped) = (void(__cdecl *)(Ped))(0x9ED0C0 + dwLoadOffset);
static void (__cdecl * TaskEnterCarAsDriver)(Ped ped, Vehicle vehicle, int duration) = (void(__cdecl *)(Ped, Vehicle, int))(0x9EB3E0 + dwLoadOffset);
static void (__cdecl * TaskEnterCarAsPassenger)(Ped ped, Vehicle vehicle, int duration, int seatIndex) = (void(__cdecl *)(Ped, Vehicle, int, int))(0x9EB2A0 + dwLoadOffset);
static void (__cdecl * TaskCarDriveToCoord)(Ped, Vehicle, float, float, float, float speed, int p1f_2b, int p2, int p3, int px2, int p45000) = (void(__cdecl *)(Ped, Vehicle, float, float, float, float, int, int, int, int, int))(0x9EB6D0 + dwLoadOffset);
static void (__cdecl * TaskAimGunAtCoord)(Ped, float, float, float, int) = (void (__cdecl*)(Ped, float, float, float, int))(0x9ED330+dwLoadOffset);
// World
static void (__cdecl * ClearAreaOfChars)(float x, float y, float z, float radius) = (void (__cdecl*)(float, float, float, float))(0xA0CC60+dwLoadOffset);
static void (__cdecl * ClearAreaOfCops)(float x, float y, float z, float radius) = (void (__cdecl*)(float, float, float, float))(0xA0CCD0+dwLoadOffset);


#else#
// 1.0.0.4

#endif
*/