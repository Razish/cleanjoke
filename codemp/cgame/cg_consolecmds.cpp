/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
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

// cg_consolecmds.c -- text commands typed in at the local console, or
// executed by a key binding

#include "cgame/cg_local.h"
#include "ui/ui_shared.h"

void CG_TargetCommand_f( void ) {
	int		targetNum;
	char	test[4];

	targetNum = CG_CrosshairPlayer();
	if ( targetNum == -1 ) {
		return;
	}

	trap->Cmd_Argv( 1, test, 4 );
	trap->SendClientCommand( va( "gc %i %i", targetNum, atoi( test ) ) );
}

// Keybinding command
static void CG_SizeUp_f (void) {
	trap->Cvar_Set( "cg_viewsize", va( "%i", Q_min( cg_viewsize.integer + 10, 100 ) ) );
}

// Keybinding command
static void CG_SizeDown_f (void) {
	trap->Cvar_Set( "cg_viewsize", va( "%i", Q_max( cg_viewsize.integer - 10, 30 ) ) );
}

// Debugging command to print the current position
static void CG_Viewpos_f (void) {
	trap->Print ("%s (%i %i %i) : %i\n", cgs.mapname, (int)cg.refdef.vieworg[0],
		(int)cg.refdef.vieworg[1], (int)cg.refdef.vieworg[2],
		(int)cg.refdef.viewangles[YAW]);
}

static void CG_ScoresDown_f( void ) {

	CG_BuildSpectatorString();
	if ( cg.scoresRequestTime + 2000 < cg.time ) {
		// the scores are more than two seconds out of data,
		// so request new ones
		cg.scoresRequestTime = cg.time;
		trap->SendClientCommand( "score" );

		// leave the current scores up if they were already
		// displayed, but if this is the first hit, clear them out
		if ( !cg.showScores ) {
			cg.showScores = true;
			cg.numScores = 0;
		}
	} else {
		// show the cached contents even if they just pressed if it
		// is within two seconds
		cg.showScores = true;
	}
}

static void CG_ScoresUp_f( void ) {
	if ( cg.showScores ) {
		cg.showScores = false;
		cg.scoreFadeTime = cg.time;
	}
}

void CG_ClientList_f( void )
{
	clientInfo_t *ci;
	int i;
	int count = 0;

	for( i = 0; i < MAX_CLIENTS; i++ )
	{
		ci = &cgs.clientinfo[ i ];
		if( !ci->infoValid )
			continue;

		switch( ci->team )
		{
		case TEAM_FREE:
			Com_Printf( "%2d " S_COLOR_YELLOW "F   " S_COLOR_WHITE "%s" S_COLOR_WHITE "%s\n", i, ci->name, (ci->botSkill != -1) ? " (bot)" : "" );
			break;

		case TEAM_RED:
			Com_Printf( "%2d " S_COLOR_RED "R   " S_COLOR_WHITE "%s" S_COLOR_WHITE "%s\n", i,
				ci->name, (ci->botSkill != -1) ? " (bot)" : "" );
			break;

		case TEAM_BLUE:
			Com_Printf( "%2d " S_COLOR_BLUE "B   " S_COLOR_WHITE "%s" S_COLOR_WHITE "%s\n", i,
				ci->name, (ci->botSkill != -1) ? " (bot)" : "" );
			break;

		default:
		case TEAM_SPECTATOR:
			Com_Printf( "%2d " S_COLOR_YELLOW "S   " S_COLOR_WHITE "%s" S_COLOR_WHITE "%s\n", i, ci->name, (ci->botSkill != -1) ? " (bot)" : "" );
			break;
		}

		count++;
	}

	Com_Printf( "Listed %2d clients\n", count );
}

static void CG_TellTarget_f( void ) {
	int		clientNum;
	char	command[MAX_SAY_TEXT+10];
	char	message[MAX_SAY_TEXT];

	clientNum = CG_CrosshairPlayer();
	if ( clientNum == -1 ) {
		return;
	}

	trap->Cmd_Args( message, sizeof(message) );
	Com_sprintf( command, sizeof(command), "tell %i %s", clientNum, message );
	trap->SendClientCommand( command );
}

static void CG_TellAttacker_f( void ) {
	int		clientNum;
	char	command[MAX_SAY_TEXT + 10];
	char	message[MAX_SAY_TEXT];

	clientNum = CG_LastAttacker();
	if ( clientNum == -1 ) {
		return;
	}

	trap->Cmd_Args( message, sizeof(message) );
	Com_sprintf( command, sizeof(command), "tell %i %s", clientNum, message );
	trap->SendClientCommand( command );
}

static void CG_StartOrbit_f( void ) {
	if ( !developer.integer ) {
		return;
	}
	if (cg_cameraOrbit.value != 0) {
		trap->Cvar_Set ("cg_cameraOrbit", "0");
		trap->Cvar_Set("cg_thirdPerson", "0");
	} else {
		trap->Cvar_Set("cg_cameraOrbit", "5");
		trap->Cvar_Set("cg_thirdPerson", "1");
		trap->Cvar_Set("cg_thirdPersonAngle", "0");
		trap->Cvar_Set("cg_thirdPersonRange", "100");
	}
}

static void CG_LoadHud_f( void ) {
	const char *hudSet = cg_hudFiles.string;
	if ( hudSet[0] == '\0' ) {
		hudSet = DEFAULT_HUDSET;
	}

	String_Init();
	Menu_Reset();
	CG_LoadMenus( hudSet );
}

typedef struct consoleCommand_s {
	const char	*cmd;
	void		(*func)(void);
} consoleCommand_t;

int cmdcmp( const void *a, const void *b ) {
	return Q_stricmp( (const char *)a, ((consoleCommand_t*)b)->cmd );
}

static const consoleCommand_t commands[] = {
	{ "-scores",       CG_ScoresUp_f },
	{ "+scores",       CG_ScoresDown_f },
	{ "clientlist",    CG_ClientList_f },
	{ "forcenext",     CG_NextForcePower_f },
	{ "forceprev",     CG_PrevForcePower_f },
	{ "invnext",       CG_NextInventory_f },
	{ "invprev",       CG_PrevInventory_f },
	{ "loaddeferred",  CG_LoadDeferredPlayers },
	{ "loadhud",       CG_LoadHud_f },
	{ "nextframe",     CG_TestModelNextFrame_f },
	{ "nextskin",      CG_TestModelNextSkin_f },
	{ "prevframe",     CG_TestModelPrevFrame_f },
	{ "prevskin",      CG_TestModelPrevSkin_f },
	{ "sizedown",      CG_SizeDown_f },
	{ "sizeup",        CG_SizeUp_f },
	{ "startOrbit",    CG_StartOrbit_f },
	{ "tcmd",          CG_TargetCommand_f },
	{ "tell_attacker", CG_TellAttacker_f },
	{ "tell_target",   CG_TellTarget_f },
	{ "testgun",       CG_TestGun_f },
	{ "testmodel",     CG_TestModel_f },
	{ "viewpos",       CG_Viewpos_f },
	{ "weapnext",      CG_NextWeapon_f },
	{ "weapon",        CG_Weapon_f },
	{ "weaponclean",   CG_WeaponClean_f },
	{ "weapprev",      CG_PrevWeapon_f },
};

static const size_t numCommands = ARRAY_LEN( commands );

// The string has been tokenized and can be retrieved with Cmd_Argc() / Cmd_Argv()
bool CG_ConsoleCommand( void ) {
	consoleCommand_t	*command = nullptr;

	command = (consoleCommand_t *)Q_LinearSearch( CG_Argv( 0 ), commands, numCommands, sizeof( commands[0] ), cmdcmp );

	if ( !command || !command->func )
		return false;

	command->func();
	return true;
}

static const char *gcmds[] = {
	"addbot",
	"callteamvote",
	"callvote",
	"duelteam",
	"follow",
	"follownext",
	"followprev",
	"forcechanged",
	"give",
	"god",
	"kill",
	"levelshot",
	"loaddefered",
	"noclip",
	"notarget",
	"NPC",
	"say",
	"say_team",
	"setviewpos",
	"stats",
	//"stopfollow",
	"team",
	"teamtask",
	"teamvote",
	"tell",
	"voice_cmd",
	"vote",
	"where",
	"zoom"
};
static const size_t numgcmds = ARRAY_LEN( gcmds );

// Let the client system know about all of our commands so it can perform tab completion
void CG_InitConsoleCommands( void ) {
	size_t i;

	for ( i = 0; i < numCommands; i++ )
		trap->AddCommand( commands[i].cmd );

	// the game server will interpret these commands, which will be automatically
	// forwarded to the server after they are not recognized locally

	for( i = 0; i < numgcmds; i++ )
		trap->AddCommand( gcmds[i] );
}
