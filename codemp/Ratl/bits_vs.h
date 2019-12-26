/*
===========================================================================
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

#pragma once

// RAVEN STANDARD TEMPLATE LIBRARY
//  (c) 2002 Activision
// Bit Field
// The bits class is a bit field of any length which supports all the standard bitwise operations in addition to some
//	operators for adding & removing individual bits by their integer indicies and a string conversion method.
// NOTES:
// 	The SIZE template variable determines how many BITS are available in this template, not how much memory (number of
//		ints) were used to store it.

// Includes
#include "Ratl/ratl_common.h"

namespace ratl
{

// The Bit Field Class
template <int	SZ>
class bits_vs : public bits_base<SZ>
{
	// Call This Function To Set All Bits Beyond SIZE to Zero
    void	clear_trailing_bits()
	{
		for (int i=this->SIZE; i<this->ARRAY_SIZE*this->BITS_INT_SIZE; i++)
		{
			this->mV[i>>this->BITS_SHIFT] &= ~(1<<(i&this->BITS_AND));
		}
	}

public:
	// Capacity Enum
	enum
	{
		SIZE			= SZ,
		CAPACITY		= SZ,
	};

	// Standard Constructor
	bits_vs(bool init=true,bool initValue=false) : bits_base<SZ>(init,initValue)
	{
	}
	// Copy Constructor
	bits_vs(const bits_vs &B)
	{
		mem::cpy(this->mV, B.mV,this->BYTE_SIZE);
	}

	// String Constructor (Format: "100010100101")
	bits_vs(const char* Str)
	{
		this->clear();

		for (int b=0; b<SIZE; b++)
		{
			if (!Str[b])
			{
				return;			// Reached The End Of The String
			}
			if (Str[b]=='1')
			{
				this->set_bit(b);		// Found A True Bit
			}
		}
	}

	// Checks If There Are Any Values At All In This Bit Field (Same as operator !())
	bool		empty() const
	{
		for (int i=0; i<this->ARRAY_SIZE; i++)
		{
			if (this->mV[i])
			{
				return false;
			}
		}
		return true;
	}

	// Get The Number Of Bits Represented Here
	int			size()	const
	{
		return SIZE;
	}

	// Execute A Bitwise Flip On All The Bits
	void		invert()
	{
		for (int i=0; i<this->ARRAY_SIZE; i++)
		{
			this->mV[i] = ~this->mV[i];
		}
		clear_trailing_bits();
	}

	// Execute A Bitwise Flip On All The Bits
	void		operator~()
	{
		invert();
	}

	// Query
	bool		get_bit(const int i) const
	{
		// If you hit this assert, then you are trying
		// to query a bit that goes beyond the number
		// of bits this object can hold.
		assert(i>=0 && i < SIZE);
		return ( (this->mV[i>>this->BITS_SHIFT] & (1<<(i&this->BITS_AND)))!=0 );
	}

	// Checks If There Are Any Values At All In This Bit Field
	bool		operator!() const
	{
		return empty();
	}

	// Equality Operator
	bool		operator==(const bits_vs &B) const
	{
		return (mem::eql(this->mV, B.mV,this->BYTE_SIZE));
	}

	// InEquality Operator
	bool		operator!=(const bits_vs &B) const
	{
		return !(operator==(B));
	}

	// Or In From Another Bits Object
	void		operator|=(const bits_vs &B)
	{
		for (int i=0; i<this->ARRAY_SIZE; i++)
		{
			this->mV[i] |= B.mV[i];
		}
	}

	// And In From Another Bits Object
	void		operator&=(const bits_vs &B)
	{
		for (int i=0; i<this->ARRAY_SIZE; i++)
		{
			this->mV[i] &= B.mV[i];
		}
	}

	// xor In From Another Bits Object
	void		operator^=(const bits_vs &B)
	{
		for (int i=0; i<this->ARRAY_SIZE; i++)
		{
			this->mV[i] ^= B.mV[i];
		}
	}

	// Assignment Operator
	void		operator=(const bits_vs &B)
	{
		mem::cpy(this->mV, B.mV,this->BYTE_SIZE);
	}

};

}
