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

// elementary actions

#pragma once

// ======================================================================
// FUNCTION
// ======================================================================

//ClientCommand elementary actions
void EA_Action(int client, int action);
void EA_Alt_Attack(int client);
void EA_Attack(int client);
void EA_Command(int client, const char* command);
void EA_Crouch(int client);
void EA_ForcePower(int client);
void EA_Gesture(int client);
void EA_MoveBack(int client);
void EA_MoveDown(int client);
void EA_MoveForward(int client);
void EA_MoveLeft(int client);
void EA_MoveRight(int client);
void EA_MoveUp(int client);
void EA_Respawn(int client);
void EA_Say(int client, const char* str);
void EA_SayTeam(int client, const char* str);
void EA_Talk(int client);
void EA_Use(int client);
void EA_Walk(int client);

//regular elementary actions
void EA_DelayedJump(int client);
void EA_Jump(int client);
void EA_Move(int client, vec3_t dir, float speed);
void EA_SelectWeapon(int client, int weapon);
void EA_View(int client, vec3_t viewangles);

//send regular input to the server
void EA_EndRegular(int client, float thinktime);
void EA_GetInput(int client, float thinktime, bot_input_t* input);
void EA_ResetInput(int client);

//setup and shutdown routines
int EA_Setup(void);
void EA_Shutdown(void);