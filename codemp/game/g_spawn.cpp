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

#include "game/g_local.h"
#include "game/g_ICARUScb.h"

bool	G_SpawnString( const char *key, const char *defaultString, char **out ) {
	int		i;

	if ( !level.spawning ) {
		*out = (char *)defaultString;
//		trap->Error( ERR_DROP, "G_SpawnString() called while not spawning" );
	}

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		if ( !Q_stricmp( key, level.spawnVars[i][0] ) ) {
			*out = level.spawnVars[i][1];
			return true;
		}
	}

	*out = (char *)defaultString;
	return false;
}

bool	G_SpawnFloat( const char *key, const char *defaultString, float *out ) {
	char		*s;
	bool	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atof( s );
	return present;
}

bool	G_SpawnInt( const char *key, const char *defaultString, int *out ) {
	char		*s;
	bool	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atoi( s );
	return present;
}

bool	G_SpawnVector( const char *key, const char *defaultString, float *out ) {
	char		*s;
	bool	present;

	present = G_SpawnString( key, defaultString, &s );
	if ( sscanf( s, "%f %f %f", &out[0], &out[1], &out[2] ) != 3 ) {
		trap->Print( "G_SpawnVector: Failed sscanf on %s (default: %s)\n", key, defaultString );
		VectorClear( out );
		return false;
	}
	return present;
}

bool	G_SpawnBoolean( const char *key, const char *defaultString, bool *out ) {
	char		*s;
	bool	present;

	present = G_SpawnString( key, defaultString, &s );

	if ( !Q_stricmp( s, "true" ) || !Q_stricmp( s, "true" ) || !Q_stricmp( s, "yes" ) || !Q_stricmp( s, "1" ) )
		*out = true;
	else if ( !Q_stricmp( s, "false" ) || !Q_stricmp( s, "false" ) || !Q_stricmp( s, "no" ) || !Q_stricmp( s, "0" ) )
		*out = false;
	else
		*out = false;

	return present;
}

// fields are needed for spawning from the entity string
typedef enum {
	F_INT,
	F_FLOAT,
	F_STRING,			// string on disk, pointer in memory
	F_VECTOR,
	F_ANGLEHACK,
	F_PARM1,			// Special case for parms
	F_PARM2,			// Special case for parms
	F_PARM3,			// Special case for parms
	F_PARM4,			// Special case for parms
	F_PARM5,			// Special case for parms
	F_PARM6,			// Special case for parms
	F_PARM7,			// Special case for parms
	F_PARM8,			// Special case for parms
	F_PARM9,			// Special case for parms
	F_PARM10,			// Special case for parms
	F_PARM11,			// Special case for parms
	F_PARM12,			// Special case for parms
	F_PARM13,			// Special case for parms
	F_PARM14,			// Special case for parms
	F_PARM15,			// Special case for parms
	F_PARM16			// Special case for parms
} fieldtype_t;

typedef struct gfield_s {
	const char	*name;
	size_t		ofs;
	fieldtype_t	type;
} gfield_t;

const gfield_t fields[] = {
	{ "alliedteam",         FOFS( alliedTeam ),                  F_INT },//for misc_turrets
	{ "angerscript",        FOFS( behaviorSet[BSET_ANGER] ),     F_STRING },//name of script to run
	{ "angle",              FOFS( s.angles ),                    F_ANGLEHACK },
	{ "angles",             FOFS( s.angles ),                    F_VECTOR },
	{ "attackscript",       FOFS( behaviorSet[BSET_ATTACK] ),    F_STRING },//name of script to run
	{ "awakescript",        FOFS( behaviorSet[BSET_AWAKE] ),     F_STRING },//name of script to run
	{ "blockedscript",      FOFS( behaviorSet[BSET_BLOCKED] ),   F_STRING },//name of script to run
	{ "chunksize",          FOFS( mass ),                        F_FLOAT },//for func_breakables
	{ "classname",          FOFS( classname ),                   F_STRING },
	{ "closetarget",        FOFS( closetarget ),                 F_STRING },//for doors
	{ "count",              FOFS( count ),                       F_INT },
	{ "deathscript",        FOFS( behaviorSet[BSET_DEATH] ),     F_STRING },//name of script to run
	{ "delay",              FOFS( delay ),                       F_INT },
	{ "delayscript",        FOFS( behaviorSet[BSET_DELAYED] ),   F_STRING },//name of script to run
	{ "delayscripttime",    FOFS( delayScriptTime ),             F_INT },//name of script to run
	{ "dmg",                FOFS( damage ),                      F_INT },
	{ "ffdeathscript",      FOFS( behaviorSet[BSET_FFDEATH] ),   F_STRING },//name of script to run
	{ "ffirescript",        FOFS( behaviorSet[BSET_FFIRE] ),     F_STRING },//name of script to run
	{ "fleescript",         FOFS( behaviorSet[BSET_FLEE] ),      F_STRING },//name of script to run
	{ "fullname",           FOFS( fullName ),                    F_STRING },
	{ "goaltarget",         FOFS( goaltarget ),                  F_STRING },//for siege
	{ "healingclass",       FOFS( healingclass ),                F_STRING },
	{ "healingrate",        FOFS( healingrate ),                 F_INT },
	{ "healingsound",       FOFS( healingsound ),                F_STRING },
	{ "health",             FOFS( health ),                      F_INT },
	{ "linear",             FOFS( alt_fire ),                    F_INT },//for movers to use linear movement
	{ "lostenemyscript",    FOFS( behaviorSet[BSET_LOSTENEMY] ), F_STRING },//name of script to run
	{ "message",            FOFS( message ),                     F_STRING },
	{ "mindtrickscript",    FOFS( behaviorSet[BSET_MINDTRICK] ), F_STRING },//name of script to run
	{ "model",              FOFS( model ),                       F_STRING },
	{ "model2",             FOFS( model2 ),                      F_STRING },
	{ "numchunks",          FOFS( radius ),                      F_FLOAT },//for func_breakables
	{ "opentarget",         FOFS( opentarget ),                  F_STRING },//for doors
	{ "origin",             FOFS( s.origin ),                    F_VECTOR },
	{ "ownername",          FOFS( ownername ),                   F_STRING },
	{ "painscript",         FOFS( behaviorSet[BSET_PAIN] ),      F_STRING },//name of script to run
	{ "paintarget",         FOFS( paintarget ),                  F_STRING },//for doors
	{ "parm1",              0,                                   F_PARM1 },
	{ "parm10",             0,                                   F_PARM10 },
	{ "parm11",             0,                                   F_PARM11 },
	{ "parm12",             0,                                   F_PARM12 },
	{ "parm13",             0,                                   F_PARM13 },
	{ "parm14",             0,                                   F_PARM14 },
	{ "parm15",             0,                                   F_PARM15 },
	{ "parm16",             0,                                   F_PARM16 },
	{ "parm2",              0,                                   F_PARM2 },
	{ "parm3",              0,                                   F_PARM3 },
	{ "parm4",              0,                                   F_PARM4 },
	{ "parm5",              0,                                   F_PARM5 },
	{ "parm6",              0,                                   F_PARM6 },
	{ "parm7",              0,                                   F_PARM7 },
	{ "parm8",              0,                                   F_PARM8 },
	{ "parm9",              0,                                   F_PARM9 },
	{ "radius",             FOFS( radius ),                      F_FLOAT },
	{ "random",             FOFS( random ),                      F_FLOAT },
	{ "roffname",           FOFS( roffname ),                    F_STRING },
	{ "rofftarget",         FOFS( rofftarget ),                  F_STRING },
	{ "script_targetname",  FOFS( script_targetname ),           F_STRING },//scripts look for this when "affecting"
	{ "soundset",           FOFS( soundSet ),                    F_STRING },
	{ "spawnflags",         FOFS( spawnflags ),                  F_INT },
	{ "spawnscript",        FOFS( behaviorSet[BSET_SPAWN] ),     F_STRING },//name of script to run
	{ "speed",              FOFS( speed ),                       F_FLOAT },
	{ "target",             FOFS( target ),                      F_STRING },
	{ "target2",            FOFS( target2 ),                     F_STRING },
	{ "target3",            FOFS( target3 ),                     F_STRING },
	{ "target4",            FOFS( target4 ),                     F_STRING },
	{ "target5",            FOFS( target5 ),                     F_STRING },
	{ "target6",            FOFS( target6 ),                     F_STRING },
	{ "targetname",         FOFS( targetname ),                  F_STRING },
	{ "targetshadername",   FOFS( targetShaderName ),            F_STRING },
	{ "targetshadernewname",FOFS( targetShaderNewName ),         F_STRING },
	{ "team",               FOFS( team ),                        F_STRING },
	{ "teamnodmg",          FOFS( teamnodmg ),                   F_INT },
	{ "teamowner",          FOFS( s.teamowner ),                 F_INT },
	{ "teamuser",           FOFS( alliedTeam ),                  F_INT },
	{ "usescript",          FOFS( behaviorSet[BSET_USE] ),       F_STRING },//name of script to run
	{ "victoryscript",      FOFS( behaviorSet[BSET_VICTORY] ),   F_STRING },//name of script to run
	{ "wait",               FOFS( wait ),                        F_FLOAT },
};

typedef struct spawn_s {
	const char	*name;
	void		(*spawn)(gentity_t *ent);
} spawn_t;

void SP_item_botroam( gentity_t *ent ) { }

void SP_gametype_item ( gentity_t* ent )
{
	gitem_t *item = nullptr;
	char *value;
	int team = -1;

	G_SpawnString("teamfilter", "", &value);

	G_SetOrigin( ent, ent->s.origin );

	// If a team filter is set then override any team settings for the spawns
	if ( level.mTeamFilter[0] )
	{
		if ( Q_stricmp ( level.mTeamFilter, "red") == 0 )
		{
			team = TEAM_RED;
		}
		else if ( Q_stricmp ( level.mTeamFilter, "blue") == 0 )
		{
			team = TEAM_BLUE;
		}
	}

	if (ent->targetname && ent->targetname[0])
	{
		if (team != -1)
		{
			if (strstr(ent->targetname, "flag"))
			{
				if (team == TEAM_RED)
				{
					item = BG_FindItem("team_CTF_redflag");
				}
				else
				{ //blue
					item = BG_FindItem("team_CTF_blueflag");
				}
			}
		}
		else if (strstr(ent->targetname, "red_flag"))
		{
			item = BG_FindItem("team_CTF_redflag");
		}
		else if (strstr(ent->targetname, "blue_flag"))
		{
			item = BG_FindItem("team_CTF_blueflag");
		}
		else
		{
			item = nullptr;
		}

		if (item)
		{
			ent->targetname = nullptr;
			ent->classname = item->classname;
			G_SpawnItem( ent, item );
		}
	}
}

const spawn_t	spawns[] = {
	{ "emplaced_gun",                      SP_emplaced_gun },
	{ "func_bobbing",                      SP_func_bobbing },
	{ "func_breakable",                    SP_func_breakable },
	{ "func_button",                       SP_func_button },
	{ "func_door",                         SP_func_door },
	{ "func_glass",                        SP_func_glass },
	{ "func_group",                        SP_info_null },
	{ "func_pendulum",                     SP_func_pendulum },
	{ "func_plat",                         SP_func_plat },
	{ "func_rotating",                     SP_func_rotating },
	{ "func_static",                       SP_func_static },
	{ "func_timer",                        SP_func_timer }, // rename trigger_timer?
	{ "func_train",                        SP_func_train },
	{ "func_usable",                       SP_func_usable },
	{ "func_wall",                         SP_func_wall },
	{ "fx_rain",                           SP_CreateRain },
	{ "fx_runner",                         SP_fx_runner },
	{ "fx_snow",                           SP_CreateSnow },
	{ "fx_spacedust",                      SP_CreateSpaceDust },
	{ "fx_wind",                           SP_CreateWind },
	{ "gametype_item",                     SP_gametype_item },
	{ "info_camp",                         SP_info_camp },
	{ "info_jedimaster_start",             SP_info_jedimaster_start },
	{ "info_notnull",                      SP_info_notnull }, // use target_position instead
	{ "info_null",                         SP_info_null },
	{ "info_player_deathmatch",            SP_info_player_deathmatch },
	{ "info_player_duel",                  SP_info_player_duel },
	{ "info_player_duel1",                 SP_info_player_duel1 },
	{ "info_player_duel2",                 SP_info_player_duel2 },
	{ "info_player_intermission",          SP_info_player_intermission },
	{ "info_player_intermission_blue",     SP_info_player_intermission_blue },
	{ "info_player_intermission_red",      SP_info_player_intermission_red },
	{ "info_player_start",                 SP_info_player_start },
	{ "info_player_start_blue",            SP_info_player_start_blue },
	{ "info_player_start_red",             SP_info_player_start_red },
	{ "item_botroam",                      SP_item_botroam },
	{ "light",                             SP_light },
	{ "misc_ammo_floor_unit",              SP_misc_ammo_floor_unit },
	{ "misc_bsp",                          SP_misc_bsp },
	{ "misc_cubemap",                      SP_misc_cubemap },
	{ "misc_faller",                       SP_misc_faller },
	{ "misc_G2model",                      SP_misc_G2model },
	{ "misc_holocron",                     SP_misc_holocron },
	{ "misc_maglock",                      SP_misc_maglock },
	{ "misc_model",                        SP_misc_model },
	{ "misc_model_ammo_power_converter",   SP_misc_model_ammo_power_converter },
	{ "misc_model_breakable",              SP_misc_model_breakable },
	{ "misc_model_health_power_converter", SP_misc_model_health_power_converter },
	{ "misc_model_shield_power_converter", SP_misc_model_shield_power_converter },
	{ "misc_model_static",                 SP_misc_model_static },
	{ "misc_portal_camera",                SP_misc_portal_camera },
	{ "misc_portal_surface",               SP_misc_portal_surface },
	{ "misc_shield_floor_unit",            SP_misc_shield_floor_unit },
	{ "misc_skyportal",                    SP_misc_skyportal },
	{ "misc_skyportal_orient",             SP_misc_skyportal_orient },
	{ "misc_teleporter_dest",              SP_misc_teleporter_dest },
	{ "misc_turret",                       SP_misc_turret },
	{ "misc_turretG2",                     SP_misc_turretG2 },
	{ "misc_weapon_shooter",               SP_misc_weapon_shooter },
	{ "misc_weather_zone",                 SP_misc_weather_zone },
	{ "path_corner",                       SP_path_corner },
	{ "ref_tag",                           SP_reference_tag },
	{ "ref_tag_huge",                      SP_reference_tag },
	{ "shooter_blaster",                   SP_shooter_blaster },
	{ "target_activate",                   SP_target_activate },
	{ "target_counter",                    SP_target_counter },
	{ "target_deactivate",                 SP_target_deactivate },
	{ "target_delay",                      SP_target_delay },
	{ "target_give",                       SP_target_give },
	{ "target_kill",                       SP_target_kill },
	{ "target_laser",                      SP_target_laser },
	{ "target_level_change",               SP_target_level_change },
	{ "target_location",                   SP_target_location },
	{ "target_play_music",                 SP_target_play_music },
	{ "target_position",                   SP_target_position },
	{ "target_print",                      SP_target_print },
	{ "target_push",                       SP_target_push },
	{ "target_random",                     SP_target_random },
	{ "target_relay",                      SP_target_relay },
	{ "target_remove_powerups",            SP_target_remove_powerups },
	{ "target_score",                      SP_target_score },
	{ "target_screenshake",                SP_target_screenshake },
	{ "target_scriptrunner",               SP_target_scriptrunner },
	{ "target_speaker",                    SP_target_speaker },
	{ "target_teleporter",                 SP_target_teleporter },
	{ "team_CTF_blueplayer",               SP_team_CTF_blueplayer },
	{ "team_CTF_bluespawn",                SP_team_CTF_bluespawn },
	{ "team_CTF_redplayer",                SP_team_CTF_redplayer },
	{ "team_CTF_redspawn",                 SP_team_CTF_redspawn },
	{ "terrain",                           SP_terrain },
	{ "trigger_always",                    SP_trigger_always },
	{ "trigger_asteroid_field",            SP_trigger_asteroid_field },
	{ "trigger_hurt",                      SP_trigger_hurt },
	{ "trigger_lightningstrike",           SP_trigger_lightningstrike },
	{ "trigger_multiple",                  SP_trigger_multiple },
	{ "trigger_once",                      SP_trigger_once },
	{ "trigger_push",                      SP_trigger_push },
	{ "trigger_space",                     SP_trigger_space },
	{ "trigger_teleport",                  SP_trigger_teleport },
	{ "waypoint",                          SP_waypoint },
	{ "waypoint_navgoal",                  SP_waypoint_navgoal },
	{ "waypoint_navgoal_1",                SP_waypoint_navgoal_1 },
	{ "waypoint_navgoal_2",                SP_waypoint_navgoal_2 },
	{ "waypoint_navgoal_4",                SP_waypoint_navgoal_4 },
	{ "waypoint_navgoal_8",                SP_waypoint_navgoal_8 },
	{ "waypoint_small",                    SP_waypoint_small },
};

// Finds the spawn function for the entity and calls it, returning false if not found
static int spawncmp( const void *a, const void *b ) {
	return Q_stricmp( (const char *)a, ((spawn_t*)b)->name );
}

bool G_CallSpawn( gentity_t *ent ) {
	spawn_t	*s;
	gitem_t	*item;

	if ( !ent->classname ) {
		trap->Print( "G_CallSpawn: nullptr classname\n" );
		return false;
	}

	// check item spawn functions
	//TODO: cant reorder items because compat so....?
	for ( item=bg_itemlist+1 ; item->classname ; item++ ) {
		if ( !strcmp(item->classname, ent->classname) ) {
			G_SpawnItem( ent, item );
			return true;
		}
	}

	// check normal spawn functions
	s = (spawn_t *)Q_LinearSearch( ent->classname, spawns, ARRAY_LEN( spawns ), sizeof( spawn_t ), spawncmp );
	if ( s )
	{// found it
		if ( VALIDSTRING( ent->healingsound ) )
			G_SoundIndex( ent->healingsound );

		s->spawn( ent );
		return true;
	}

	trap->Print( "%s doesn't have a spawn function\n", ent->classname );
	return false;
}

// Builds a copy of the string, translating \n to real linefeeds so message texts can be multi-line
char *G_NewString( const char *string )
{
	char *newb=nullptr, *new_p=nullptr;
	int i=0, len=0;

	len = strlen( string )+1;
	new_p = newb = (char *)G_Alloc( len );

	for ( i=0; i<len; i++ )
	{// turn \n into a real linefeed
		if ( string[i] == '\\' && i < len-1 )
		{
			if ( string[i+1] == 'n' )
			{
				*new_p++ = '\n';
				i++;
			}
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}

	return newb;
}

char *G_NewString_Safe( const char *string )
{
	char *newb=nullptr, *new_p=nullptr;
	int i=0, len=0;

	len = strlen( string )+1;
	new_p = newb = (char *)malloc( len );

	if ( !new_p )
		return nullptr;

	for ( i=0; i<len; i++ )
	{// turn \n into a real linefeed
		if ( string[i] == '\\' && i < len-1 )
		{
			if ( string[i+1] == 'n' )
			{
				*new_p++ = '\n';
				i++;
			}
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}

	return newb;
}

static int fieldcmp( const void *a, const void *b ) {
	return Q_stricmp( (const char *)a, ((gfield_t*)b)->name );
}

// Takes a key/value pair and sets the binary values in a gentity
void G_ParseField( const char *key, const char *value, gentity_t *ent )
{
	gfield_t	*f;
	byte	*b;
	float	v;
	vec3_t	vec;

	f = (gfield_t *)Q_LinearSearch( key, fields, ARRAY_LEN( fields ), sizeof( gfield_t ), fieldcmp );
	if ( f )
	{// found it
		b = (byte *)ent;

		switch( f->type ) {
		case F_STRING:
			*(char **)(b+f->ofs) = G_NewString (value);
			break;
		case F_VECTOR:
			if ( sscanf( value, "%f %f %f", &vec[0], &vec[1], &vec[2] ) == 3 ) {
				((float *)(b+f->ofs))[0] = vec[0];
				((float *)(b+f->ofs))[1] = vec[1];
				((float *)(b+f->ofs))[2] = vec[2];
			}
			else {
				trap->Print( "G_ParseField: Failed sscanf on F_VECTOR (key/value: %s/%s)\n", key, value );
				((float *)(b+f->ofs))[0] = ((float *)(b+f->ofs))[1] = ((float *)(b+f->ofs))[2] = 0.0f;
			}
			break;
		case F_INT:
			*(int *)(b+f->ofs) = atoi(value);
			break;
		case F_FLOAT:
			*(float *)(b+f->ofs) = atof(value);
			break;
		case F_ANGLEHACK:
			v = atof(value);
			((float *)(b+f->ofs))[0] = 0;
			((float *)(b+f->ofs))[1] = v;
			((float *)(b+f->ofs))[2] = 0;
			break;
		case F_PARM1:
		case F_PARM2:
		case F_PARM3:
		case F_PARM4:
		case F_PARM5:
		case F_PARM6:
		case F_PARM7:
		case F_PARM8:
		case F_PARM9:
		case F_PARM10:
		case F_PARM11:
		case F_PARM12:
		case F_PARM13:
		case F_PARM14:
		case F_PARM15:
		case F_PARM16:
			Q3_SetParm( ent->s.number, (f->type - F_PARM1), (char *) value );
			break;
		}
		return;
	}
}

static void Adjust_AreaPortal( gentity_t *ent ) {
	if ( ent->s.eType == ET_MOVER ) {
		trap->LinkEntity( (sharedEntity_t *)ent );
		trap->AdjustAreaPortalState( (sharedEntity_t *)ent, true );
	}
}

// Spawn an entity and fill in all of the level fields from level.spawnVars[], then call the class specfic spawn
//	function
void G_SpawnGEntityFromSpawnVars( bool inSubBSP ) {
	int			i;
	gentity_t	*ent;
	char		*s, *value, *gametypeName;
	static char *gametypeNames[GT_MAX_GAME_TYPE] = {"ffa", "holocron", "jedimaster", "duel", "powerduel", "team", "ctf", "cty"};

	// get the next free entity
	ent = G_Spawn();

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], ent );
	}

	// check for "notteam" flag (GT_FFA, GT_DUEL)
	if ( level.gametype >= GT_TEAM ) {
		G_SpawnInt( "notteam", "0", &i );
		if ( i ) {
			Adjust_AreaPortal( ent );
			G_FreeEntity( ent );
			return;
		}
	} else {
		G_SpawnInt( "notfree", "0", &i );
		if ( i ) {
			Adjust_AreaPortal( ent );
			G_FreeEntity( ent );
			return;
		}
	}

	if( G_SpawnString( "gametype", nullptr, &value ) ) {
		if( level.gametype >= GT_FFA && level.gametype < GT_MAX_GAME_TYPE ) {
			gametypeName = gametypeNames[level.gametype];

			s = strstr( value, gametypeName );
			if( !s ) {
				Adjust_AreaPortal( ent );
				G_FreeEntity( ent );
				return;
			}
		}
	}

	// move editor origin to pos
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );

	// if we didn't get a classname, don't bother spawning anything
	if ( !G_CallSpawn( ent ) ) {
		G_FreeEntity( ent );
	}

	//Tag on the ICARUS scripting information only to valid recipients
	if ( trap->ICARUS_ValidEnt( (sharedEntity_t *)ent ) )
	{
		trap->ICARUS_InitEnt( (sharedEntity_t *)ent );

		if ( ent->classname && ent->classname[0] )
		{
			G_ActivateBehavior( ent, BSET_SPAWN );
		}
	}
}

char *G_AddSpawnVarToken( const char *string ) {
	int		l;
	char	*dest;

	l = strlen( string );
	if ( level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS ) {
		trap->Error( ERR_DROP, "G_AddSpawnVarToken: MAX_SPAWN_VARS_CHARS" );
	}

	dest = level.spawnVarChars + level.numSpawnVarChars;
	memcpy( dest, string, l+1 );

	level.numSpawnVarChars += l + 1;

	return dest;
}

void AddSpawnField(char *field, char *value)
{
	int	i;

	for(i=0;i<level.numSpawnVars;i++)
	{
		if (Q_stricmp(level.spawnVars[i][0], field) == 0)
		{
			level.spawnVars[ i ][1] = G_AddSpawnVarToken( value );
			return;
		}
	}

	level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( field );
	level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( value );
	level.numSpawnVars++;
}

#define NOVALUE "novalue"

static void HandleEntityAdjustment(void)
{
	char		*value;
	vec3_t		origin, newOrigin, angles;
	char		temp[MAX_QPATH];
	float		rotation;

	G_SpawnString("origin", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		if ( sscanf( value, "%f %f %f", &origin[0], &origin[1], &origin[2] ) != 3 ) {
			trap->Print( "HandleEntityAdjustment: failed sscanf on 'origin' (%s)\n", value );
			VectorClear( origin );
		}
	}
	else
	{
		origin[0] = origin[1] = origin[2] = 0.0;
	}

	rotation = DEG2RAD(level.mRotationAdjust);
	newOrigin[0] = origin[0]*cos(rotation) - origin[1]*sin(rotation);
	newOrigin[1] = origin[0]*sin(rotation) + origin[1]*cos(rotation);
	newOrigin[2] = origin[2];
	VectorAdd(newOrigin, level.mOriginAdjust, newOrigin);
	// damn VMs don't handle outputing a float that is compatible with sscanf in all cases
	Com_sprintf(temp, sizeof( temp ), "%0.0f %0.0f %0.0f", newOrigin[0], newOrigin[1], newOrigin[2]);
	AddSpawnField("origin", temp);

	G_SpawnString("angles", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		if ( sscanf( value, "%f %f %f", &angles[0], &angles[1], &angles[2] ) != 3 ) {
			trap->Print( "HandleEntityAdjustment: failed sscanf on 'angles' (%s)\n", value );
			VectorClear( angles );
		}

		angles[YAW] = fmod(angles[YAW] + level.mRotationAdjust, 360.0f);
		// damn VMs don't handle outputing a float that is compatible with sscanf in all cases
		Com_sprintf(temp, sizeof( temp ), "%0.0f %0.0f %0.0f", angles[0], angles[1], angles[2]);
		AddSpawnField("angles", temp);
	}
	else
	{
		G_SpawnString("angle", NOVALUE, &value);
		if (Q_stricmp(value, NOVALUE) != 0)
		{
			angles[YAW] = atof( value );
		}
		else
		{
			angles[YAW] = 0.0;
		}
		angles[YAW] = fmod(angles[YAW] + level.mRotationAdjust, 360.0f);
		Com_sprintf(temp, sizeof( temp ), "%0.0f", angles[YAW]);
		AddSpawnField("angle", temp);
	}

	// RJR experimental code for handling "direction" field of breakable brushes
	// though direction is rarely ever used.
	G_SpawnString("direction", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		if ( sscanf( value, "%f %f %f", &angles[0], &angles[1], &angles[2] ) != 3 ) {
			trap->Print( "HandleEntityAdjustment: failed sscanf on 'direction' (%s)\n", value );
			VectorClear( angles );
		}
	}
	else
	{
		angles[0] = angles[1] = angles[2] = 0.0;
	}
	angles[YAW] = fmod(angles[YAW] + level.mRotationAdjust, 360.0f);
	Com_sprintf(temp, sizeof( temp ), "%0.0f %0.0f %0.0f", angles[0], angles[1], angles[2]);
	AddSpawnField("direction", temp);

	AddSpawnField("BSPInstanceID", level.mTargetAdjust);

	G_SpawnString("targetname", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("targetname", temp);
	}

	G_SpawnString("target", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("target", temp);
	}

	G_SpawnString("killtarget", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("killtarget", temp);
	}

	G_SpawnString("brushparent", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("brushparent", temp);
	}

	G_SpawnString("brushchild", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("brushchild", temp);
	}

	G_SpawnString("enemy", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("enemy", temp);
	}

	G_SpawnString("ICARUSname", NOVALUE, &value);
	if (Q_stricmp(value, NOVALUE) != 0)
	{
		Com_sprintf(temp, sizeof( temp ), "%s%s", level.mTargetAdjust, value);
		AddSpawnField("ICARUSname", temp);
	}
}

// Parses a brace bounded set of key / value pairs out of the level's entity strings into level.spawnVars[]
// This does not actually spawn an entity.
bool G_ParseSpawnVars( bool inSubBSP ) {
	char		keyname[MAX_TOKEN_CHARS];
	char		com_token[MAX_TOKEN_CHARS];

	level.numSpawnVars = 0;
	level.numSpawnVarChars = 0;

	// parse the opening brace
	if ( !trap->GetEntityToken( com_token, sizeof( com_token ) ) ) {
		// end of spawn string
		return false;
	}
	if ( com_token[0] != '{' ) {
		trap->Error( ERR_DROP, "G_ParseSpawnVars: found %s when expecting {",com_token );
	}

	// go through all the key / value pairs
	while ( 1 ) {
		// parse key
		if ( !trap->GetEntityToken( keyname, sizeof( keyname ) ) ) {
			trap->Error( ERR_DROP, "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( keyname[0] == '}' ) {
			break;
		}

		// parse value
		if ( !trap->GetEntityToken( com_token, sizeof( com_token ) ) ) {
			trap->Error( ERR_DROP, "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( com_token[0] == '}' ) {
			trap->Error( ERR_DROP, "G_ParseSpawnVars: closing brace without data" );
		}
		if ( level.numSpawnVars == MAX_SPAWN_VARS ) {
			trap->Error( ERR_DROP, "G_ParseSpawnVars: MAX_SPAWN_VARS" );
		}
		level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( keyname );
		level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( com_token );
		level.numSpawnVars++;
	}

	if (inSubBSP)
	{
		HandleEntityAdjustment();
	}

	return true;
}

static	char *defaultStyles[32][3] =
{
	{	// 0 normal
		"z",
		"z",
		"z"
	},
	{	// 1 FLICKER (first variety)
		"mmnmmommommnonmmonqnmmo",
		"mmnmmommommnonmmonqnmmo",
		"mmnmmommommnonmmonqnmmo"
	},
	{	// 2 SLOW STRONG PULSE
		"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb",
		"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb",
		"abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcb"
	},
	{	// 3 CANDLE (first variety)
		"mmmmmaaaaammmmmaaaaaabcdefgabcdefg",
		"mmmmmaaaaammmmmaaaaaabcdefgabcdefg",
		"mmmmmaaaaammmmmaaaaaabcdefgabcdefg"
	},
	{	// 4 FAST STROBE
		"mamamamamama",
		"mamamamamama",
		"mamamamamama"
	},
	{	// 5 GENTLE PULSE 1
		"jklmnopqrstuvwxyzyxwvutsrqponmlkj",
		"jklmnopqrstuvwxyzyxwvutsrqponmlkj",
		"jklmnopqrstuvwxyzyxwvutsrqponmlkj"
	},
	{	// 6 FLICKER (second variety)
		"nmonqnmomnmomomno",
		"nmonqnmomnmomomno",
		"nmonqnmomnmomomno"
	},
	{	// 7 CANDLE (second variety)
		"mmmaaaabcdefgmmmmaaaammmaamm",
		"mmmaaaabcdefgmmmmaaaammmaamm",
		"mmmaaaabcdefgmmmmaaaammmaamm"
	},
	{	// 8 CANDLE (third variety)
		"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa",
		"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa",
		"mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa"
	},
	{	// 9 SLOW STROBE (fourth variety)
		"aaaaaaaazzzzzzzz",
		"aaaaaaaazzzzzzzz",
		"aaaaaaaazzzzzzzz"
	},
	{	// 10 FLUORESCENT FLICKER
		"mmamammmmammamamaaamammma",
		"mmamammmmammamamaaamammma",
		"mmamammmmammamamaaamammma"
	},
	{	// 11 SLOW PULSE NOT FADE TO BLACK
		"abcdefghijklmnopqrrqponmlkjihgfedcba",
		"abcdefghijklmnopqrrqponmlkjihgfedcba",
		"abcdefghijklmnopqrrqponmlkjihgfedcba"
	},
	{	// 12 FAST PULSE FOR JEREMY
		"mkigegik",
		"mkigegik",
		"mkigegik"
	},
	{	// 13 Test Blending
		"abcdefghijklmqrstuvwxyz",
		"zyxwvutsrqmlkjihgfedcba",
		"aammbbzzccllcckkffyyggp"
	},
	{	// 14
		"",
		"",
		""
	},
	{	// 15
		"",
		"",
		""
	},
	{	// 16
		"",
		"",
		""
	},
	{	// 17
		"",
		"",
		""
	},
	{	// 18
		"",
		"",
		""
	},
	{	// 19
		"",
		"",
		""
	},
	{	// 20
		"",
		"",
		""
	},
	{	// 21
		"",
		"",
		""
	},
	{	// 22
		"",
		"",
		""
	},
	{	// 23
		"",
		"",
		""
	},
	{	// 24
		"",
		"",
		""
	},
	{	// 25
		"",
		"",
		""
	},
	{	// 26
		"",
		"",
		""
	},
	{	// 27
		"",
		"",
		""
	},
	{	// 28
		"",
		"",
		""
	},
	{	// 29
		"",
		"",
		""
	},
	{	// 30
		"",
		"",
		""
	},
	{	// 31
		"",
		"",
		""
	}
};

void *precachedKyle = 0;

/*QUAKED worldspawn (0 0 0) ?

Every map should have exactly one worldspawn.
"music"		music wav file
"gravity"	800 is default gravity
"message"	Text to print during connection process

BSP Options
"gridsize"     size of lighting grid to "X Y Z". default="64 64 128"
"ambient"      scale of global light (from _color)
"fog"          shader name of the global fog texture - must include the full path, such as "textures/rj/fog1"
"distancecull" value for vis for the maximum viewing distance
"chopsize"     value for bsp on the maximum polygon / portal size
"ls_Xr"	override lightstyle X with this pattern for Red.
"ls_Xg"	green (valid patterns are "a-z")
"ls_Xb"	blue (a is OFF, z is ON)

"fogstart"		override fog start distance and force linear
"radarrange" for Siege/Vehicle radar - default range is 2500
*/
float g_cullDistance;
void SP_worldspawn( void )
{
	char		*text, temp[32];
	int			i;
	int			lengthRed, lengthBlue, lengthGreen;

	//I want to "cull" entities out of net sends to clients to reduce
	//net traffic on our larger open maps -rww
	G_SpawnFloat("distanceCull", "6000.0", &g_cullDistance);
	trap->SetServerCull(g_cullDistance);

	G_SpawnString( "classname", "", &text );
	if ( Q_stricmp( text, "worldspawn" ) ) {
		trap->Error( ERR_DROP, "SP_worldspawn: The first entity isn't 'worldspawn'" );
	}

	for ( i = 0 ; i < level.numSpawnVars ; i++ )
	{
		if ( Q_stricmp( "spawnscript", level.spawnVars[i][0] ) == 0 )
		{//ONly let them set spawnscript, we don't want them setting an angle or something on the world.
			G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], &g_entities[ENTITYNUM_WORLD] );
		}
	}
	//The server will precache the standard model and animations, so that there is no hit
	//when the first client connnects.
	if (!BGPAFtextLoaded)
	{
		BG_ParseAnimationFile("models/players/_humanoid/animation.cfg", bgHumanoidAnimations, true);
	}

	if (!precachedKyle)
	{
		int defSkin;

		trap->G2API_InitGhoul2Model(&precachedKyle, "models/players/" DEFAULT_MODEL "/model.glm", 0, 0, -20, 0, 0);

		if (precachedKyle)
		{
			defSkin = trap->R_RegisterSkin("models/players/" DEFAULT_MODEL "/model_default.skin");
			trap->G2API_SetSkin(precachedKyle, 0, defSkin, defSkin);
		}
	}

	if (!g2SaberInstance)
	{
		trap->G2API_InitGhoul2Model(&g2SaberInstance, DEFAULT_SABER_MODEL, 0, 0, -20, 0, 0);

		if (g2SaberInstance)
		{
			// indicate we will be bolted to model 0 (ie the player) on bolt 0 (always the right hand) when we get copied
			trap->G2API_SetBoltInfo(g2SaberInstance, 0, 0);
			// now set up the gun bolt on it
			trap->G2API_AddBolt(g2SaberInstance, 0, "*blade1");
		}
	}

	// make some data visible to connecting client
	trap->SetConfigstring( CS_GAME_VERSION, GAME_VERSION );

	trap->SetConfigstring( CS_LEVEL_START_TIME, va("%i", level.startTime ) );

	G_SpawnString( "music", "", &text );
	trap->SetConfigstring( CS_MUSIC, text );

	G_SpawnString( "message", "", &text );
	trap->SetConfigstring( CS_MESSAGE, text );				// map specific message

	trap->SetConfigstring( CS_MOTD, g_motd.string );		// message of the day

	G_SpawnString( "gravity", "800", &text );
	trap->Cvar_Set( "g_gravity", text );
	trap->Cvar_Update( &g_gravity );

	G_SpawnString( "enableBreath", "0", &text );

	G_SpawnString( "soundSet", "default", &text );
	trap->SetConfigstring( CS_GLOBAL_AMBIENT_SET, text );

	g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
	g_entities[ENTITYNUM_WORLD].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_WORLD].classname = "worldspawn";

	g_entities[ENTITYNUM_NONE].s.number = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_NONE].classname = "nothing";

	// see if we want a warmup time
	trap->SetConfigstring( CS_WARMUP, "" );
	if ( g_restarted.integer ) {
		trap->Cvar_Set( "g_restarted", "0" );
		trap->Cvar_Update( &g_restarted );
		level.warmupTime = 0;
	}
	else if ( g_doWarmup.integer && level.gametype != GT_DUEL && level.gametype != GT_POWERDUEL ) { // Turn it on
		level.warmupTime = -1;
		trap->SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
		G_LogPrintf( "Warmup:\n" );
	}

	trap->SetConfigstring(CS_LIGHT_STYLES+(LS_STYLES_START*3)+0, defaultStyles[0][0]);
	trap->SetConfigstring(CS_LIGHT_STYLES+(LS_STYLES_START*3)+1, defaultStyles[0][1]);
	trap->SetConfigstring(CS_LIGHT_STYLES+(LS_STYLES_START*3)+2, defaultStyles[0][2]);

	for(i=1;i<LS_NUM_STYLES;i++)
	{
		Com_sprintf(temp, sizeof(temp), "ls_%dr", i);
		G_SpawnString(temp, defaultStyles[i][0], &text);
		lengthRed = strlen(text);
		trap->SetConfigstring(CS_LIGHT_STYLES+((i+LS_STYLES_START)*3)+0, text);

		Com_sprintf(temp, sizeof(temp), "ls_%dg", i);
		G_SpawnString(temp, defaultStyles[i][1], &text);
		lengthGreen = strlen(text);
		trap->SetConfigstring(CS_LIGHT_STYLES+((i+LS_STYLES_START)*3)+1, text);

		Com_sprintf(temp, sizeof(temp), "ls_%db", i);
		G_SpawnString(temp, defaultStyles[i][2], &text);
		lengthBlue = strlen(text);
		trap->SetConfigstring(CS_LIGHT_STYLES+((i+LS_STYLES_START)*3)+2, text);

		if (lengthRed != lengthGreen || lengthGreen != lengthBlue)
		{
			Com_Error(ERR_DROP, "Style %d has inconsistent lengths: R %d, G %d, B %d",
				i, lengthRed, lengthGreen, lengthBlue);
		}
	}
}

//rww - Planning on having something here?
bool SP_bsp_worldspawn ( void )
{
	return true;
}

void G_PrecacheSoundsets( void )
{
	gentity_t	*ent = nullptr;
	int i;
	int countedSets = 0;

	for ( i = 0; i < MAX_GENTITIES; i++ )
	{
		ent = &g_entities[i];

		if (ent->inuse && ent->soundSet && ent->soundSet[0])
		{
			if (countedSets >= MAX_AMBIENT_SETS)
			{
				Com_Error(ERR_DROP, "MAX_AMBIENT_SETS was exceeded! (too many soundsets)\n");
			}

			ent->s.soundSetIndex = G_SoundSetIndex(ent->soundSet);
			countedSets++;
		}
	}
}

void G_LinkLocations( void ) {
	int i, n;

	if ( level.locations.linked )
		return;

	level.locations.linked = true;

	trap->SetConfigstring( CS_LOCATIONS, "unknown" );

	for ( i=0, n=1; i<level.locations.num; i++ ) {
		level.locations.data[i].cs_index = n;
		trap->SetConfigstring( CS_LOCATIONS + n, level.locations.data[i].message );
		n++;
	}
	// All linked together now
}

// Parses textual entity definitions out of an entstring and spawns gentities.
void G_SpawnEntitiesFromString( bool inSubBSP ) {
	// allow calls to G_Spawn*()
	level.spawning = true;
	level.numSpawnVars = 0;

	// the worldspawn is not an actual entity, but it still
	// has a "spawn" function to perform any global setup
	// needed by a level (setting configstrings or cvars, etc)
	if ( !G_ParseSpawnVars(false) ) {
		trap->Error( ERR_DROP, "SpawnEntities: no entities" );
	}

	if (!inSubBSP)
	{
		SP_worldspawn();
	}
	else
	{
		// Skip this guy if its worldspawn fails
		if ( !SP_bsp_worldspawn() )
		{
			return;
		}
	}

	// parse ents
	while( G_ParseSpawnVars(inSubBSP) ) {
		G_SpawnGEntityFromSpawnVars(inSubBSP);
	}

	if( g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN] && g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN][0] )
	{//World has a spawn script, but we don't want the world in ICARUS and running scripts,
		//so make a scriptrunner and start it going.
		gentity_t *script_runner = G_Spawn();
		if ( script_runner )
		{
			script_runner->behaviorSet[BSET_USE] = g_entities[ENTITYNUM_WORLD].behaviorSet[BSET_SPAWN];
			script_runner->count = 1;
			script_runner->think = scriptrunner_run;
			script_runner->nextthink = level.time + 100;

			if ( script_runner->inuse )
			{
				trap->ICARUS_InitEnt( (sharedEntity_t *)script_runner );
			}
		}
	}

	if (!inSubBSP)
	{
		level.spawning = false;			// any future calls to G_Spawn*() will be errors
	}

	G_LinkLocations();

	G_PrecacheSoundsets();
}

