/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2019, OpenJK contributors
Copyright (C) 2019 - 2020, CleanJoKe contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#pragma once

// ======================================================================
// DEFINE
// ======================================================================

#define CTF_CAPTURE_BONUS		100		// what you get for capture
#define CTF_TEAM_BONUS			25		// what your team gets for capture
#define CTF_RECOVERY_BONUS		10		// what you get for recovery
#define CTF_FLAG_BONUS			10		// what you get for picking up enemy flag
#define CTF_FRAG_CARRIER_BONUS	20		// what you get for fragging enemy flag carrier
#define CTF_FLAG_RETURN_TIME	40000	// seconds until auto return

#define CTF_CARRIER_DANGER_PROTECT_BONUS	5	// bonus for fraggin someone who has recently hurt your flag carrier
#define CTF_CARRIER_PROTECT_BONUS			2	// bonus for fraggin someone while either you or your target are near your flag carrier
#define CTF_FLAG_DEFENSE_BONUS				10	// bonus for fraggin someone while either you or your target are near your flag
#define CTF_RETURN_FLAG_ASSIST_BONUS		10	// awarded for returning a flag that causes a capture to happen almost immediately
#define CTF_FRAG_CARRIER_ASSIST_BONUS		10	// award for fragging a flag carrier if a capture happens almost immediately

#define CTF_TARGET_PROTECT_RADIUS			1000	// the radius around an object being defended where a target will be worth extra frags
#define CTF_ATTACKER_PROTECT_RADIUS			1000	// the radius around an object being defended where an attacker will get extra frags when making kills

#define CTF_CARRIER_DANGER_PROTECT_TIMEOUT	8000
#define CTF_FRAG_CARRIER_ASSIST_TIMEOUT		10000
#define CTF_RETURN_FLAG_ASSIST_TIMEOUT		10000

#define CTF_GRAPPLE_SPEED					750 // speed of grapple in flight
#define CTF_GRAPPLE_PULL_SPEED				750	// speed player is pulled at

#define OVERLOAD_ATTACK_BASE_SOUND_TIME		20000

// ======================================================================
// FUNCTION
// ======================================================================

bool Team_GetLocationMsg(gentity_t* ent, char* loc, int loclen);
const char* OtherTeamName(int team);
const char* TeamColorString(int team);
const char* TeamName(int team);
gentity_t* SelectCTFSpawnPoint(team_t team, int teamstate, vec3_t origin, vec3_t angles, bool isbot);
int OtherTeam(int team);
int Pickup_Team(gentity_t* ent, gentity_t* other);
locationData_t* Team_GetLocation(gentity_t* ent);
void AddTeamScore(vec3_t origin, int team, int score);
void CheckTeamStatus(void);
void Team_CheckHurtCarrier(gentity_t* targ, gentity_t* attacker);
void Team_DroppedFlagThink(gentity_t* ent);
void Team_FragBonuses(gentity_t* targ, gentity_t* inflictor, gentity_t* attacker);
void Team_FreeEntity(gentity_t* ent);
void Team_InitGame(void);
void Team_ReturnFlag(int team);
void TeamplayInfoMessage(gentity_t* ent);
