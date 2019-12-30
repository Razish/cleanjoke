/*
===========================================================================
Copyright (C) 1999 - 2005, Id Software, Inc.
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2005 - 2015, ioquake3 contributors
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

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "qcommon/q_common.h"
#include "qcommon/com_cvars.h"

vm_t *currentVM = nullptr;

static const char *vmNames[MAX_VM] = {
	"jampgame",
	"cgame",
	"ui"
};

const char *vmStrs[MAX_VM] = {
	"GameVM",
	"CGameVM",
	"UIVM",
};

// VM slots are automatically allocated by VM_Create, and freed by VM_Free
// The VM table should never be directly accessed from other files.
// Example usage:
//	cgvm = VM_Create( VM_CGAME );	// vmTable[VM_CGAME] is allocated
//	CGVM_Init( foo, bar );			// internally may use VM_Call( cgvm, CGAME_INIT, foo, bar ) for legacy cgame modules
//	cgvm = VM_Restart( cgvm );		// vmTable[VM_CGAME] is recreated, we update the cgvm pointer
//	VM_Free( cgvm );				// vmTable[VM_CGAME] is deallocated and set to nullptr
//	cgvm = nullptr;					// ...so we update the cgvm pointer

static vm_t *vmTable[MAX_VM];

void VM_Init( void ) {
	memset( vmTable, 0, sizeof(vmTable) );
}

// Reload the data, but leave everything else in place
// This allows a server to do a map_restart without changing memory allocation
vm_t *VM_Restart( vm_t *vm ) {
	const vm_t saved = *vm;

	VM_Free( vm );

	return VM_Create( saved.slot );
}

vm_t *VM_Create( vmSlots_t vmSlot ) {
	vm_t *vm = nullptr;

	// see if we already have the VM
	if ( vmTable[vmSlot] )
		return vmTable[vmSlot];

	// find a free vm
	vmTable[vmSlot] = (vm_t *)Z_Malloc( sizeof(*vm), TAG_VM, true );
	vm = vmTable[vmSlot];

	// initialise it
	vm->slot = vmSlot;
	Q_strncpyz( vm->name, vmNames[vmSlot], sizeof(vm->name) );

	// find the module api
	FS_FindPureDLL( vm->name );
	vm->dllHandle = Sys_LoadGameDll( vm->name, &vm->GetModuleAPI );

	Com_Printf( "VM_Create: %s" ARCH_STRING DLL_EXT, vm->name );
	if ( vm->dllHandle ) {
		if ( developer->integer )
			Com_Printf( " succeeded [0x%" PRIxPTR "+0x%" PRIxPTR "]\n", vm->dllHandle, (intptr_t)vm->GetModuleAPI - (intptr_t)vm->dllHandle );
		else
			Com_Printf( " succeeded\n" );
		return vm;
	}

	VM_Free( vm );
	Com_Printf( " failed!\n" );
	return nullptr;
}

void VM_Free( vm_t *vm ) {
	if ( !vm )
		return;

	// mark the slot as free
	vmTable[vm->slot] = nullptr;

	if ( vm->dllHandle )
		Sys_UnloadDll( vm->dllHandle );

	memset( vm, 0, sizeof(*vm) );

	Z_Free( vm );

	currentVM = nullptr;
}

void VM_Clear( void ) {
	for ( int i = 0; i < MAX_VM; i++ )
		VM_Free( vmTable[i] );

	currentVM = nullptr;
}

void VM_Shifted_Alloc( void **ptr, int size ) {
	void *mem = nullptr;

	if ( !currentVM ) {
		assert( 0 );
		*ptr = nullptr;
		return;
	}

	mem = Z_Malloc( size + 1, TAG_VM_ALLOCATED, false );
	if ( !mem ) {
		assert( 0 );
		*ptr = nullptr;
		return;
	}

	memset( mem, 0, size + 1 );

	*ptr = mem;
}

void VM_Shifted_Free( void **ptr ) {
	void *mem = nullptr;

	if ( !currentVM ) {
		assert( 0 );
		return;
	}

	mem = (void *)*ptr;
	if ( !mem ) {
		assert( 0 );
		return;
	}

	Z_Free( mem );
	*ptr = nullptr;
}
