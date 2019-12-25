/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

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

// elementary actions

#include "qcommon/q_shared.h"
#include "botlib/l_memory.h"
#include "botlib/l_script.h"
#include "botlib/l_precomp.h"
#include "botlib/l_struct.h"
#include "botlib/botlib.h"
#include "botlib/be_interface.h"
#include "botlib/be_ea.h"

#define MAX_USERMOVE				400
#define MAX_COMMANDARGUMENTS		10
#define ACTION_JUMPEDLASTFRAME		0x0800000//128

bot_input_t *botinputs;

void EA_Say(int client, const char *str)
{
	botimport.BotClientCommand(client, va("say %s", str) );
} //end of the function EA_Say
void EA_SayTeam(int client, const char *str)
{
	botimport.BotClientCommand(client, va("say_team %s", str));
} //end of the function EA_SayTeam
void EA_Tell(int client, int clientto, const char *str)
{
	botimport.BotClientCommand(client, va("tell %d, %s", clientto, str));
} //end of the function EA_SayTeam
void EA_UseItem(int client, const char *it)
{
	botimport.BotClientCommand(client, va("use %s", it));
} //end of the function EA_UseItem
void EA_DropItem(int client, const char *it)
{
	botimport.BotClientCommand(client, va("drop %s", it));
} //end of the function EA_DropItem
void EA_UseInv(int client, const char *inv)
{
	botimport.BotClientCommand(client, va("invuse %s", inv));
} //end of the function EA_UseInv
void EA_DropInv(int client, const char *inv)
{
	botimport.BotClientCommand(client, va("invdrop %s", inv));
} //end of the function EA_DropInv
void EA_Gesture(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_GESTURE;
} //end of the function EA_Gesture
void EA_Command(int client, const char *command)
{
	botimport.BotClientCommand(client, command);
} //end of the function EA_Command
void EA_SelectWeapon(int client, int weapon)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->weapon = weapon;
} //end of the function EA_SelectWeapon
void EA_Attack(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_ATTACK;
} //end of the function EA_Attack
void EA_Alt_Attack(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_ALT_ATTACK;
} //end of the function EA_Alt_Attack
void EA_ForcePower(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_FORCEPOWER;
} //end of the function EA_ForcePower
void EA_Talk(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_TALK;
} //end of the function EA_Talk
void EA_Use(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_USE;
} //end of the function EA_Use
void EA_Respawn(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_RESPAWN;
} //end of the function EA_Respawn
void EA_Jump(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	if (bi->actionflags & ACTION_JUMPEDLASTFRAME)
	{
		bi->actionflags &= ~ACTION_JUMP;
	} //end if
	else
	{
		bi->actionflags |= ACTION_JUMP;
	} //end if
} //end of the function EA_Jump
void EA_DelayedJump(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	if (bi->actionflags & ACTION_JUMPEDLASTFRAME)
	{
		bi->actionflags &= ~ACTION_DELAYEDJUMP;
	} //end if
	else
	{
		bi->actionflags |= ACTION_DELAYEDJUMP;
	} //end if
} //end of the function EA_DelayedJump
void EA_Crouch(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_CROUCH;
} //end of the function EA_Crouch
void EA_Walk(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_WALK;
} //end of the function EA_Walk
void EA_Action(int client, int action)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= action;
} //end of function EA_Action
void EA_MoveUp(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVEUP;
} //end of the function EA_MoveUp
void EA_MoveDown(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVEDOWN;
} //end of the function EA_MoveDown
void EA_MoveForward(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVEFORWARD;
} //end of the function EA_MoveForward
void EA_MoveBack(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVEBACK;
} //end of the function EA_MoveBack
void EA_MoveLeft(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVELEFT;
} //end of the function EA_MoveLeft
void EA_MoveRight(int client)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	bi->actionflags |= ACTION_MOVERIGHT;
} //end of the function EA_MoveRight
void EA_Move(int client, vec3_t dir, float speed)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	VectorCopy(dir, bi->dir);
	//cap speed
	if (speed > MAX_USERMOVE) speed = MAX_USERMOVE;
	else if (speed < -MAX_USERMOVE) speed = -MAX_USERMOVE;
	bi->speed = speed;
} //end of the function EA_Move
void EA_View(int client, vec3_t viewangles)
{
	bot_input_t *bi;

	bi = &botinputs[client];

	VectorCopy(viewangles, bi->viewangles);
} //end of the function EA_View
void EA_EndRegular(int client, float thinktime)
{
} //end of the function EA_EndRegular
void EA_GetInput(int client, float thinktime, bot_input_t *input)
{
	bot_input_t *bi;

	bi = &botinputs[client];
	bi->thinktime = thinktime;
	Com_Memcpy(input, bi, sizeof(bot_input_t));
} //end of the function EA_GetInput
void EA_ResetInput(int client)
{
	bot_input_t *bi;
	bool jumped = false;

	bi = &botinputs[client];
	bi->actionflags &= ~ACTION_JUMPEDLASTFRAME;

	bi->thinktime = 0;
	VectorClear(bi->dir);
	bi->speed = 0;
	jumped = bi->actionflags & ACTION_JUMP;
	bi->actionflags = 0;
	if (jumped) bi->actionflags |= ACTION_JUMPEDLASTFRAME;
} //end of the function EA_ResetInput
int EA_Setup(void)
{
	//initialize the bot inputs
	botinputs = (bot_input_t *) GetClearedHunkMemory(
									botlibglobals.maxclients * sizeof(bot_input_t));
	return BLERR_NOERROR;
} //end of the function EA_Setup
void EA_Shutdown(void)
{
	FreeMemory(botinputs);
	botinputs = NULL;
} //end of the function EA_Shutdown
