/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2019, OpenJK contributors
Copyright (C) 2019 - 2019, CleanJoKe contributors

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

#include "cgame/cg_local.h"

// Cvar callbacks

static void CG_SVRunningChange( void ) {
	cgs.localServer = sv_running.integer;
}

static void CG_ForceModelChange( void ) {
	int i;

	for ( i=0; i<MAX_CLIENTS; i++ ) {
		const char *clientInfo = CG_ConfigString( CS_PLAYERS+i );

		if ( !VALIDSTRING( clientInfo ) )
			continue;

		CG_NewClientInfo( i, true );
	}
}

static void CG_TeamOverlayChange( void ) {
	// If team overlay is on, ask for updates from the server.  If its off,
	// let the server know so we don't receive it
	if ( cg_drawTeamOverlay.integer > 0 && cgs.gametype >= GT_TEAM)
		trap->Cvar_Set( "teamoverlay", "1" );
	else
		trap->Cvar_Set( "teamoverlay", "0" );
}

static void CVU_GunAlign( void ) {
	float *v = &cg.gunAlign[0];
	if ( sscanf( cg_gunAlign.string, "%f %f %f", &v[0], &v[1], &v[2] ) != 3 ) {
		v[0] = 0.0f;
		v[1] = 0.0f;
		v[2] = 0.0f;
	}
}

static void CVU_GunBob( void ) {
	float *v = &cg.gunBob[0];
	if ( sscanf( cg_gunBob.string, "%f %f %f", &v[0], &v[1], &v[2] ) != 3 ) {
		v[0] = 0.005f;
		v[1] = 0.01f;
		v[2] = 0.005f;
	}
}

static void CVU_GunDrift( void ) {
	float *v = &cg.gunIdleDrift.amount[0];
	if ( sscanf( cg_gunIdleDrift.string, "%f %f %f %f", &v[0], &v[1], &v[2], &cg.gunIdleDrift.speed ) != 4 ) {
		v[0] = 0.01f;
		v[1] = 0.01f;
		v[2] = 0.01f;
		cg.gunIdleDrift.speed = 0.001f;
	}
}

// Cvar table

typedef struct cvarTable_s {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	void		(*update)( void );
	uint32_t	cvarFlags;
} cvarTable_t;

#define XCVAR_DECL
	#include "cgame/cg_xcvar.h"
#undef XCVAR_DECL

static const cvarTable_t cvarTable[] = {
	#define XCVAR_LIST
		#include "cgame/cg_xcvar.h"
	#undef XCVAR_LIST
};
static const size_t cvarTableSize = ARRAY_LEN( cvarTable );

void CG_RegisterCvars( void ) {
	size_t i = 0;
	const cvarTable_t *cv = nullptr;

	for ( i=0, cv=cvarTable; i<cvarTableSize; i++, cv++ ) {
		trap->Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
		if ( cv->update )
			cv->update();
	}
}

void CG_UpdateCvars( void ) {
	size_t i = 0;
	const cvarTable_t *cv = nullptr;

	for ( i=0, cv=cvarTable; i<cvarTableSize; i++, cv++ ) {
		if ( cv->vmCvar ) {
			int modCount = cv->vmCvar->modificationCount;
			trap->Cvar_Update( cv->vmCvar );
			if ( cv->vmCvar->modificationCount != modCount ) {
				if ( cv->update )
					cv->update();
			}
		}
	}
}
