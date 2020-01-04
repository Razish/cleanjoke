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

//undef if binary numbers of the form 0b... or 0B... are not allowed
#define BINARYNUMBERS
//undef if not using the token.intvalue and token.floatvalue
#define NUMBERVALUE
//use dollar sign also as punctuation
#define DOLLAR

//maximum token length
#define MAX_TOKEN					1024

//script flags
#define SCFL_NOERRORS				0x0001
#define SCFL_NOWARNINGS				0x0002
#define SCFL_NOSTRINGWHITESPACES	0x0004
#define SCFL_NOSTRINGESCAPECHARS	0x0008
#define SCFL_PRIMITIVE				0x0010
#define SCFL_NOBINARYNUMBERS		0x0020
#define SCFL_NONUMBERVALUES		0x0040

//token types
#define TT_STRING						1			// string
#define TT_LITERAL					2			// literal
#define TT_NUMBER						3			// number
#define TT_NAME						4			// name
#define TT_PUNCTUATION				5			// punctuation

//string sub type
//		the length of the string
//literal sub type
//		the ASCII code of the literal
//number sub type
#define TT_DECIMAL					0x0008	// decimal number
#define TT_HEX							0x0100	// hexadecimal number
#define TT_OCTAL						0x0200	// octal number
#ifdef BINARYNUMBERS
#define TT_BINARY						0x0400	// binary number
#endif //BINARYNUMBERS
#define TT_FLOAT						0x0800	// floating point number
#define TT_INTEGER					0x1000	// integer number
#define TT_LONG						0x2000	// long number
#define TT_UNSIGNED					0x4000	// unsigned number
//punctuation sub type
#define P_RSHIFT_ASSIGN				1
#define P_LSHIFT_ASSIGN				2
#define P_PARMS						3
#define P_PRECOMPMERGE				4

#define P_LOGIC_AND					5
#define P_LOGIC_OR					6
#define P_LOGIC_GEQ					7
#define P_LOGIC_LEQ					8
#define P_LOGIC_EQ					9
#define P_LOGIC_UNEQ					10

#define P_MUL_ASSIGN					11
#define P_DIV_ASSIGN					12
#define P_MOD_ASSIGN					13
#define P_ADD_ASSIGN					14
#define P_SUB_ASSIGN					15
#define P_INC							16
#define P_DEC							17

#define P_BIN_AND_ASSIGN			18
#define P_BIN_OR_ASSIGN				19
#define P_BIN_XOR_ASSIGN			20
#define P_RSHIFT						21
#define P_LSHIFT						22

#define P_POINTERREF					23
#define P_CPP1							24
#define P_CPP2							25
#define P_MUL							26
#define P_DIV							27
#define P_MOD							28
#define P_ADD							29
#define P_SUB							30
#define P_ASSIGN						31

#define P_BIN_AND						32
#define P_BIN_OR						33
#define P_BIN_XOR						34
#define P_BIN_NOT						35

#define P_LOGIC_NOT					36
#define P_LOGIC_GREATER				37
#define P_LOGIC_LESS					38

#define P_REF							39
#define P_COMMA						40
#define P_SEMICOLON					41
#define P_COLON						42
#define P_QUESTIONMARK				43

#define P_PARENTHESESOPEN			44
#define P_PARENTHESESCLOSE			45
#define P_BRACEOPEN					46
#define P_BRACECLOSE					47
#define P_SQBRACKETOPEN				48
#define P_SQBRACKETCLOSE			49
#define P_BACKSLASH					50

#define P_PRECOMP						51
#define P_DOLLAR						52
#define	P_ATSIGN						53
//name sub type
//		the length of the name

// ======================================================================
// STRUCT
// ======================================================================

//punctuation
typedef struct punctuation_s
{
	const char *p;					//punctuation character(s)
	int n;							//punctuation indication
	struct punctuation_s *next;		//next punctuation
} punctuation_t;

//token
typedef struct token_s
{
	char string[MAX_TOKEN];			//available token
	int type;						//last read token type
	int subtype;					//last read token sub type
#ifdef NUMBERVALUE
	unsigned long int intvalue;	//integer value
	long double floatvalue;			//floating point value
#endif //NUMBERVALUE
	char *whitespace_p;				//start of white space before token
	char *endwhitespace_p;			//start of white space before token
	int line;						//line the token was on
	int linescrossed;				//lines crossed in white space
	struct token_s *next;			//next token in chain
} token_t;

//script file
typedef struct script_s
{
	char filename[1024];			//file name of the script
	char *buffer;					//buffer containing the script
	char *script_p;					//current pointer in the script
	char *end_p;					//pointer to the end of the script
	char *lastscript_p;				//script pointer before reading token
	char *whitespace_p;				//begin of the white space
	char *endwhitespace_p;			//end of the white space
	int length;						//length of the script in bytes
	int line;						//current line in script
	int lastline;					//line before reading token
	int tokenavailable;				//set by UnreadLastToken
	int flags;						//several script flags
	punctuation_t *punctuations;	//the punctuations used in the script
	punctuation_t **punctuationtable;
	token_t token;					//available token
	struct script_s *next;			//next script in a chain
} script_t;

// ======================================================================
// FUNCTION
// ======================================================================

char PS_NextWhiteSpaceChar(script_t* script);
const char* PunctuationFromNum(script_t* script, int num);
int EndOfScript(script_t* script);
int GetScriptFlags(script_t* script);
int PS_CheckTokenString(script_t* script, char* string);
int PS_CheckTokenType(script_t* script, int type, int subtype, token_t* token);
int PS_ExpectAnyToken(script_t* script, token_t* token);
int PS_ExpectTokenString(script_t* script, char* string);
int PS_ExpectTokenType(script_t* script, int type, int subtype, token_t* token);
int PS_ReadToken(script_t* script, token_t* token);
int PS_SkipUntilString(script_t* script, char* string);
long double ReadSignedFloat(script_t* script);
script_t* LoadScriptFile(const char* filename);
script_t* LoadScriptMemory(char* ptr, int length, char* name);
signed long int ReadSignedInt(script_t* script);
void FreeScript(script_t* script);
void PS_SetBaseFolder(char* path);
void PS_UnreadLastToken(script_t* script);
void PS_UnreadToken(script_t* script, token_t* token);
void QDECL ScriptError(script_t* script, char* str, ...) __attribute__((format(printf, 2, 3)));
void QDECL ScriptWarning(script_t* script, char* str, ...) __attribute__((format(printf, 2, 3)));
void ResetScript(script_t* script);
void SetScriptFlags(script_t* script, int flags);
void SetScriptPunctuations(script_t* script, punctuation_t* p);
void StripDoubleQuotes(char* string);
void StripSingleQuotes(char* string);