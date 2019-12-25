/*
===========================================================================
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

#pragma once

void		GVM_InitGame						( int levelTime, int randomSeed, int restart );
void		GVM_ShutdownGame					( int restart );
char *		GVM_ClientConnect					( int clientNum, bool firstTime, bool isBot );
void		GVM_ClientBegin						( int clientNum );
bool	GVM_ClientUserinfoChanged			( int clientNum );
void		GVM_ClientDisconnect				( int clientNum );
void		GVM_ClientCommand					( int clientNum );
void		GVM_ClientThink						( int clientNum, usercmd_t *ucmd );
void		GVM_RunFrame						( int levelTime );
bool	GVM_ConsoleCommand					( void );
int			GVM_BotAIStartFrame					( int time );
void		GVM_ROFF_NotetrackCallback			( int entID, const char *notetrack );
void		GVM_SpawnRMGEntity					( void );
int			GVM_ICARUS_PlaySound				( void );
bool	GVM_ICARUS_Set						( void );
void		GVM_ICARUS_Lerp2Pos					( void );
void		GVM_ICARUS_Lerp2Origin				( void );
void		GVM_ICARUS_Lerp2Angles				( void );
int			GVM_ICARUS_GetTag					( void );
void		GVM_ICARUS_Lerp2Start				( void );
void		GVM_ICARUS_Lerp2End					( void );
void		GVM_ICARUS_Use						( void );
void		GVM_ICARUS_Kill						( void );
void		GVM_ICARUS_Remove					( void );
void		GVM_ICARUS_Play						( void );
int			GVM_ICARUS_GetFloat					( void );
int			GVM_ICARUS_GetVector				( void );
int			GVM_ICARUS_GetString				( void );
void		GVM_ICARUS_SoundIndex				( void );
int			GVM_ICARUS_GetSetIDForString		( void );
bool	GVM_NAV_ClearPathToPoint			( int entID, vec3_t pmins, vec3_t pmaxs, vec3_t point, int clipmask, int okToHitEnt );
bool	GVM_NPC_ClearLOS2					( int entID, const vec3_t end );
int			GVM_NAVNEW_ClearPathBetweenPoints	( vec3_t start, vec3_t end, vec3_t mins, vec3_t maxs, int ignore, int clipmask );
bool	GVM_NAV_CheckNodeFailedForEnt		( int entID, int nodeNum );
bool	GVM_NAV_EntIsUnlockedDoor			( int entityNum );
bool	GVM_NAV_EntIsDoor					( int entityNum );
bool	GVM_NAV_EntIsBreakable				( int entityNum );
bool	GVM_NAV_EntIsRemovableUsable		( int entNum );
void		GVM_NAV_FindCombatPointWaypoints	( void );
int			GVM_BG_GetItemIndexByTag			( int tag, int type );

void SV_BindGame( void );
void SV_UnbindGame( void );
void SV_InitGame( bool restart );
void SV_RestartGame( void );
