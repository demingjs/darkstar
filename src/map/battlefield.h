﻿/*
===========================================================================

  Copyright (c) 2010-2015 Darkstar Dev Teams

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/

  This file is part of DarkStar-server source code.

===========================================================================
*/

#ifndef _CBATTLEFIELD_H
#define _CBATTLEFIELD_H

#include <vector>
#include <functional>
#include <memory>

#include "../common/cbasetypes.h"
#include "../common/mmo.h"

enum BCRULES
{
    RULES_ALLOW_SUBJOBS         = 0x01,
    RULES_LOSE_EXP              = 0x02,
    RULES_REMOVE_3MIN           = 0x04,
    RULES_SPAWN_TREASURE_ON_WIN = 0x08,
    RULES_MAAT                  = 0x10
};

enum BATTLEFIELDMOBCONDITION
{
    CONDITION_NONE             = 0x00,
    CONDITION_SPAWNED_AT_START = 0x01,
    CONDITION_WIN_REQUIREMENT  = 0x02
};

enum LEAVE_CODE
{
    LEAVE_EXIT   = 1,
    LEAVE_WIN    = 2,
    LEAVE_WARPDC = 3,
    LEAVE_LOSE   = 4
};

enum BATTLEFIELD_STATUS
{
    BATTLEFIELD_STATUS_OPEN   = 0,
    BATTLEFIELD_STATUS_LOCKED = 1,
    BATTLEFIELD_STATUS_WON    = 2,
    BATTLEFIELD_STATUS_LOST   = 3
};

enum BATTLEFIELD_RETURN_CODE
{
    BATTLEFIELD_RETURN_CODE_WAIT              = 1,
    BATTLEFIELD_RETURN_CODE_CUTSCENE          = 2,
    BATTLEFIELD_RETURN_CODE_INCREMENT_REQUEST = 3,
    BATTLEFIELD_RETURN_CODE_LOCKED            = 4,
    BATTLEFIELD_RETURN_CODE_REQS_NOT_MET      = 5,
    BATTLEFIELD_RETURN_CODE_BATTLEFIELD_FULL  = 6
};

class CNpcEntity;
class CMobEntity;
class CCharEntity;
class CBaseEntity;
class CBattleEntity;
class CBattlefieldHandler;
class CZone;

typedef struct
{
    CMobEntity* PMob;
    BATTLEFIELDMOBCONDITION condition; // whether it has died or not
} BattlefieldMob_t;

typedef struct
{
    string_t name;
    duration time;
} BattlefieldRecord_t;


typedef struct
{
    string_t name;
    uint32 id;
} BattlefieldInitiator_t;

class CBattlefield : public std::enable_shared_from_this<CBattlefield>
{
public:

    CBattlefield(uint16 id, CZone* PZone, uint8 area, CCharEntity* PInitiator);
    ~CBattlefield();

    //bcnm related functions
    uint16                 GetID() const;
    CZone*                 GetZone() const;
    uint16                 GetZoneID() const;
    string_t               GetName() const;
    BattlefieldInitiator_t GetInitiator() const;
    uint8                  GetArea() const;
    BattlefieldRecord_t    GetRecord() const;
    uint8                  GetStatus() const;
    uint16                 GetRuleMask() const;
    time_point             GetStartTime() const;
    duration               GetTimeInside() const;
    time_point             GetFightTime() const;
    duration               GetTimeLimit() const;
    time_point             GetWipeTime() const;
    uint8                  GetMaxParticipants() const;
    uint8                  GetPlayerCount() const;
    uint8                  GetLevelCap() const;
    uint16                 GetLootID() const;
    duration               GetFinishTime() const;
    duration               GetRemainingTime() const;

    bool                   AllPlayersDead();
    bool                   InProgress();
    bool                   IsOccupied() const;

    void                   ForEachPlayer(std::function<void(CCharEntity*)> func);
    void                   ForEachEnemy(std::function<void(CMobEntity*)> func);
    void                   ForEachRequiredEnemy(std::function<void(CMobEntity*)> func);
    void                   ForEachAdditionalEnemy(std::function<void(CMobEntity*)> func);
    void                   ForEachNpc(std::function<void(CNpcEntity*)> func);
    void                   ForEachAlly(std::function<void(CMobEntity*)> func);

    void                   SetID(uint16 id);
    void                   SetName(int8* name);
    void                   SetInitiator(int8* name);
    void                   SetArea(uint8 area);
    void                   SetRecord(int8* name, duration time);
    void                   SetStatus(uint8 status);
    void                   SetRuleMask(uint16 rulemask);
    void                   SetStartTime(time_point time);
    void                   SetFightTime(time_point time);
    void                   SetTimeLimit(duration time);
    void                   SetWipeTime(time_point time);
    void                   SetMaxParticipants(uint8 max);
    void                   SetLevelCap(uint8 cap);
    void                   SetLootID(uint16 id);

    void                   ApplyLevelCap(CCharEntity* PChar) const;
    void                   ClearEnmityForEntity(CBattleEntity* PEntity);
    bool                   InsertEntity(CBaseEntity* PEntity, bool inBattlefield = false, BATTLEFIELDMOBCONDITION conditions = CONDITION_NONE, bool ally = false);
    CBaseEntity*           GetEntity(CBaseEntity* PEntity);
    bool                   RemoveEntity(CBaseEntity* PEntity, uint8 leavecode = 0);
    void                   DoTick(time_point time);
    bool                   CanCleanup(bool cleanup = false);
    void                   Cleanup();
    bool                   LoadMobs();
    //player related functions

    void                   PushMessageToAllInBcnm(uint16 msg, uint16 param);

    bool                   CanSpawnTreasure() const;
    bool                   SpawnTreasureChest();
    void                   OpenChest();

    bool                   AllEnemiesDefeated();

    //handler functions (time/multiple rounds/etc)

    bool                   LoseBcnm();

    std::vector<uint32>           m_PlayerList;
    std::vector<CNpcEntity*>      m_NpcList;
    std::vector<BattlefieldMob_t> m_RequiredEnemyList;
    std::vector<BattlefieldMob_t> m_AdditionalEnemyList;
    std::vector<CMobEntity*>      m_AllyList;

private:
    uint16                 m_ID;
    uint16                 m_ZoneID;
    string_t               m_Name;
    BattlefieldInitiator_t m_Initiator;
    uint8                  m_Area;
    BattlefieldRecord_t    m_Record;
    uint8                  m_Status{ BATTLEFIELD_STATUS_OPEN };
    uint16                 m_Rules;
    time_point             m_StartTime;
    time_point             m_Tick;
    time_point             m_FightTick;
    duration               m_TimeLimit;
    time_point             m_WipeTime;
    duration               m_FinishTime;
    uint8                  m_MaxParticipants;
    uint8                  m_LevelCap;
    uint32                 m_LootID;

    bool                   m_Cleanup{ false };
    bool                   m_SeenBooty{ false };
    bool                   m_GotBooty{ false };
};

#endif