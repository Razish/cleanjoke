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

#include "client/cl_local.h"
#include "client/cl_uiapi.h"

void CL_ShutdownUI( void ) {
	Key_SetCatcher( Key_GetCatcher( ) & ~KEYCATCH_UI );

	if ( !cls.uiStarted )
		return;

	cls.uiStarted = false;

	CL_UnbindUI();
}

void CL_InitUI( void ) {
	// load the dll
	CL_BindUI();

	// init for this gamestate
	//rww - changed to <= CA_ACTIVE, because that is the state when we did a vid_restart
	//ingame (was just < CA_ACTIVE before, resulting in ingame menus getting wiped and
	//not reloaded on vid restart from ingame menu)
	UIVM_Init( (bool)(cls.state >= CA_AUTHORIZING && cls.state <= CA_ACTIVE) );
}

// See if the current console command is claimed by the ui
bool UI_GameCommand( void ) {
	if ( !cls.uiStarted )
		return false;

	return UIVM_ConsoleCommand( cls.realtime );
}
