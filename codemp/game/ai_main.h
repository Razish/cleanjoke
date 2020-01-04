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

//#define FORCEJUMP_INSTANTMETHOD 1

#define BASE_FLAGWAIT_DISTANCE     256   // has the enemy flag and is waiting in his own base for his flag to be returned
#define BASE_GETENEMYFLAG_DISTANCE 256   // waiting around to get the enemy's flag
#define BASE_GUARD_DISTANCE        256   // guarding the flag
#define BOT_FLAG_GET_DISTANCE      256
#define BOT_MAX_WEAPON_CHASE_CTF   5000  // time to spend gathering the weapon before persuing the enemy base (in case it takes longer than expected) [ctf-only]
#define BOT_MAX_WEAPON_CHASE_TIME  15000 // time to spend gathering the weapon before persuing the enemy base (in case it takes longer than expected)
#define BOT_MAX_WEAPON_GATHER_TIME 1000  // spend a max of 1 second after spawn issuing orders to gather weapons before attacking enemy base
#define BOT_PLANT_BLOW_DISTANCE    256   // blow det packs if enemy is within this radius and I am further away than the enemy
#define BOT_PLANT_DISTANCE         256   // plant if within this radius from the last spotted enemy position
#define BOT_PLANT_INTERVAL         15000 // only plant once per 15 seconds at max
#define BOT_RUN_HEALTH             40
#define BOT_SABER_THROW_RANGE      800
#define BOT_WPTOUCH_DISTANCE       32
#define ENEMY_FORGET_MS            10000
#define FloatTime()                floattime
#define MAX_ATTACHMENT_NAME        64
#define MAX_CHAT_BUFFER_SIZE       8192
#define MAX_CHAT_LINE_SIZE         128
#define MAX_CHICKENWUSS_TIME       10000 // wait 10 secs between checking which run-away path to take
#define MAX_FORCE_INFO_SIZE        2048
#define MAX_LOVED_ONES             4
#define MAX_NODETABLE_SIZE         16384
#define MELEE_ATTACK_RANGE         256
#define SABER_ATTACK_RANGE         128
#define TABLE_BRANCH_DISTANCE      32
#define WP_KEEP_FLAG_DIST          128

// ======================================================================
// ENUM
// ======================================================================

enum botWeaponRange_e {
	BWEAPONRANGE_MELEE = 1,
	BWEAPONRANGE_MID,
	BWEAPONRANGE_LONG,
	BWEAPONRANGE_SABER,
};

enum levelFlag_e : uint32_t {
	LEVELFLAG_NOPOINTPREDICTION = 1, // don't take waypoint beyond current into account when adjusting path view angles
	LEVELFLAG_IGNOREINFALLBACK =  2, // ignore enemies when in a fallback navigation routine
	LEVELFLAG_IMUSTNTRUNAWAY =    4, // don't be scared
};

enum waypointFlag_e : uint32_t {
	WPFLAG_JUMP =             0x00000010, // jump when we hit this
	WPFLAG_DUCK =             0x00000020, // duck while moving around here
	WPFLAG_NOVIS =            0x00000400, // go here for a bit even with no visibility
	WPFLAG_SNIPEORCAMPSTAND = 0x00000800, // a good position to snipe or camp - stand
	WPFLAG_WAITFORFUNC =      0x00001000, // wait for a func brushent under this point before moving here
	WPFLAG_SNIPEORCAMP =      0x00002000, // a good position to snipe or camp - crouch
	WPFLAG_ONEWAY_FWD =       0x00004000, // can only go forward on the trial from here (e.g. went over a ledge)
	WPFLAG_ONEWAY_BACK =      0x00008000, // can only go backward on the trail from here
	WPFLAG_GOALPOINT =        0x00010000, // make it a goal to get here.. goal points will be decided by setting "weight" values
	WPFLAG_RED_FLAG =         0x00020000, // red flag
	WPFLAG_BLUE_FLAG =        0x00040000, // blue flag
	WPFLAG_NOMOVEFUNC =       0x00200000, // don't move over if a func is under
	WPFLAG_CALCULATED =       0x00400000, // don't calculate it again
	WPFLAG_NEVERONEWAY =      0x00800000, // never flag it as one-way
};

enum bot_ctf_state_t {
	CTFSTATE_NONE,
	CTFSTATE_ATTACKER,
	CTFSTATE_DEFENDER,
	CTFSTATE_RETRIEVAL,
	CTFSTATE_GUARDCARRIER,
	CTFSTATE_GETFLAGHOME,
	CTFSTATE_MAXCTFSTATES
};

enum bot_teamplay_state_t {
	TEAMPLAYSTATE_NONE,
	TEAMPLAYSTATE_FOLLOWING,
	TEAMPLAYSTATE_ASSISTING,
	TEAMPLAYSTATE_REGROUP,
	TEAMPLAYSTATE_MAXTPSTATES
};

// ======================================================================
// STRUCT
// ======================================================================

struct botattachment_t {
	int  level;
	char name[MAX_ATTACHMENT_NAME];
};

struct nodeobject_t {
	vec3_t origin;
	float  weight;
	int    flags;
	int    neighbornum;
	int    inuse;
};

struct boteventtracker_t {
	int   eventSequence;
	int   events[MAX_PS_EVENTS];
	float eventTime;
};

struct botskills_t {
	int   reflex;
	float accuracy;
	float turnspeed;
	float turnspeed_combat;
	float maxturn;
	int   perfectaim;
};

struct bot_state_t {
	int              inuse;             // true if this state is used by a bot client
	int              botthink_residual; // residual for the bot thinks
	int              client;            // client number of the bot
	int              entitynum;         // entity number of the bot
	playerState_t    cur_ps;            // current player state
	usercmd_t        lastucmd;          // usercmd from last frame
	bot_settings_t   settings;          // several bot settings
	float            thinktime;         // time the bot thinks this frame
	vec3_t           origin;            // origin of the bot
	vec3_t           velocity;          // velocity of the bot
	vec3_t           eye;               // eye coordinates of the bot
	int              setupcount;        // true when the bot has just been setup
	float            ltime;             // local bot time
	float            entergame_time;    // time the bot entered the game
	int              ms;                // move state of the bot
	int              gs;                // goal state of the bot
	int              ws;                // weapon state of the bot
	vec3_t           viewangles;        // current view angles
	vec3_t           ideal_viewangles;  // ideal view angles
	vec3_t           viewanglespeed;
	gentity_t       *currentEnemy;
	gentity_t       *revengeEnemy;
	gentity_t       *squadLeader;
	gentity_t       *lastHurt;
	gentity_t       *lastAttacked;
	gentity_t       *wantFlag;
	gentity_t       *touchGoal;
	gentity_t       *shootGoal;
	gentity_t       *dangerousObject;
	vec3_t           staticFlagSpot;
	int              revengeHateLevel;
	int              isSquadLeader;
	int              squadRegroupInterval;
	int              squadCannotLead;
	int              lastDeadTime;
	wpobject_t      *wpCurrent;
	wpobject_t      *wpDestination;
	wpobject_t      *wpStoreDest;
	vec3_t           goalAngles;
	vec3_t           goalMovedir;
	vec3_t           goalPosition;
	vec3_t           lastEnemySpotted;
	vec3_t           hereWhenSpotted;
	int              lastVisibleEnemyIndex;
	int              hitSpotted;
	int              wpDirection;
	float            destinationGrabTime;
	float            wpSeenTime;
	float            wpTravelTime;
	float            wpDestSwitchTime;
	float            wpSwitchTime;
	float            wpDestIgnoreTime;
	float            timeToReact;
	float            enemySeenTime;
	float            chickenWussCalculationTime;
	float            beStill;
	float            duckTime;
	float            jumpTime;
	float            jumpHoldTime;
	float            jumpPrep;
	float            forceJumping;
	float            jDelay;
	float            aimOffsetTime;
	float            aimOffsetAmtYaw;
	float            aimOffsetAmtPitch;
	float            frame_Waypoint_Len;
	int              frame_Waypoint_Vis;
	float            frame_Enemy_Len;
	int              frame_Enemy_Vis;
	int              isCamper;
	float            isCamping;
	wpobject_t      *wpCamping;
	wpobject_t      *wpCampingTo;
	bool         campStanding;
	int              randomNavTime;
	int              randomNav;
	int              saberSpecialist;
	int              canChat;
	int              chatFrequency;
	char             currentChat[MAX_CHAT_LINE_SIZE];
	float            chatTime;
	float            chatTime_stored;
	int              doChat;
	int              chatTeam;
	gentity_t       *chatObject;
	gentity_t       *chatAltObject;
	float            meleeStrafeTime;
	int              meleeStrafeDir;
	float            meleeStrafeDisable;
	int              altChargeTime;
	float            escapeDirTime;
	float            dontGoBack;
	int              doAttack;
	int              doAltAttack;
	int              forceWeaponSelect;
	int              virtualWeapon;
	int              plantTime;
	int              plantDecided;
	int              plantContinue;
	int              plantKillEmAll;
	int              runningLikeASissy;
	int              runningToEscapeThreat;
	botskills_t      skills;
	botattachment_t  loved[MAX_LOVED_ONES];
	int              lovednum;
	int              loved_death_thresh;
	int              deathActivitiesDone;
	float            botWeaponWeights[WP_NUM_WEAPONS];
	int              ctfState;
	int              teamplayState;
	int              jmState;
	int              state_Forced; //set by player ordering menu
	int              saberDefending;
	int              saberDefendDecideTime;
	int              saberBFTime;
	int              saberBTime;
	int              saberSTime;
	int              saberThrowTime;
	bool         saberPower;
	int              saberPowerTime;
	int              botChallengingTime;
	char             forceinfo[MAX_FORCE_INFO_SIZE];
#ifndef FORCEJUMP_INSTANTMETHOD
	int              forceJumpChargeTime;
#endif
	int              doForcePush;
	int              noUseTime;
	bool         doingFallback;
	int              iHaveNoIdeaWhereIAmGoing;
	vec3_t           lastSignificantAreaChange;
	int              lastSignificantChangeTime;
	int              forceMove_Forward;
	int              forceMove_Right;
	int              forceMove_Up;
	//end rww
};

// ======================================================================
// EXTERN VARIABLE
// ======================================================================

extern gentity_t *eFlagRed;
extern gentity_t *eFlagBlue;
extern wpobject_t *flagRed;
extern wpobject_t *flagBlue;
extern float floattime;
extern char gBotChatBuffer[MAX_CLIENTS][MAX_CHAT_BUFFER_SIZE];
extern float gBotEdit;
extern float gDeactivated;
extern int gLastPrintedIndex;
extern int gLevelFlags;
extern wpobject_t *gWPArray[MAX_WPARRAY_SIZE];
extern int gWPNum;
extern int gWPRenderedFrame;
extern float gWPRenderTime;
extern int nodenum;
extern nodeobject_t nodetable[MAX_NODETABLE_SIZE];
extern wpobject_t *oFlagRed;
extern wpobject_t *oFlagBlue;

// ======================================================================
// FUNCTION
// ======================================================================

int BotDoChat(bot_state_t* bs, char* section, int always);
int BotGetWeaponRange(bot_state_t* bs);
int BotIsAChickenWuss(bot_state_t* bs);
int GetBestIdleGoal(bot_state_t* bs);
int GetNearestVisibleWP(vec3_t org, int ignore);
int NumBots(void);
int OrgVisibleBox(vec3_t org1, vec3_t mins, vec3_t maxs, vec3_t org2, int ignore);
int PassLovedOneCheck(bot_state_t* bs, gentity_t* ent);
void B_Free(void* ptr);
void B_TempFree(int size);
void BotResetState(bot_state_t* bs);
void BotUtilizePersonality(bot_state_t* bs);
void BotWaypointRender(void);
void LoadPath_ThisLevel(void);
void StandardBotAI(bot_state_t* bs, float thinktime);
void* B_Alloc(int size);
void* B_TempAlloc(int size);
