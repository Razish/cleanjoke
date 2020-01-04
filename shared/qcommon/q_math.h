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
// INCLUDE
// ======================================================================

#include <cstdint>
#include <math.h>

#include "qcommon/q_type.h"

// ======================================================================
// DEFINE
// ======================================================================

#ifndef M_PI
#define M_PI		3.14159265358979323846f	// matches value in gcc v2 math.h
#endif

#define	MAX_QINT			0x7fffffff
#define	MIN_QINT			(-MAX_QINT-1)

#define QRAND_MAX 32768

#define minimum( x, y ) ((x) < (y) ? (x) : (y))
#define maximum( x, y ) ((x) > (y) ? (x) : (y))

#define PI_DIV_180		0.017453292519943295769236907684886f
#define INV_PI_DIV_180	57.295779513082320876798154814105f

#define DEG2RAD( a ) ( ( (a) * PI_DIV_180 ) )
#define RAD2DEG( a ) ( ( (a) * INV_PI_DIV_180 ) )

#define SQRTFAST( x ) ( (x) * Q_rsqrt( x ) )
#define Square(x) ((x)*(x))

#define Q_min(x,y) ((x)<(y)?(x):(y))
#define Q_max(x,y) ((x)>(y)?(x):(y))

// angle indexes
#define	PITCH	0		// up / down
#define	YAW		1		// left / right
#define	ROLL	2		// fall over

// plane types are used to speed some tests
// 0-2 are axial planes
#define	PLANE_X			0
#define	PLANE_Y			1
#define	PLANE_Z			2
#define	PLANE_NON_AXIAL	3

// vec2
#define VectorScale2M(v, factor, dst) \
	(dst)[0] = (v[0]) * (factor), \
	(dst)[1] = (v[1]) * (factor)

#define VectorCopy2M(src, dst) \
	(dst)[0] = (src[0]), \
	(dst)[1] = (src[1])

#define VectorClear2M(dst) \
	memset((dst), 0, sizeof((dst)[0]) * 2)

// vec3
#define VectorScaleM(v, factor, dst) \
	(dst)[0] = (v[0]) * (factor), \
	(dst)[1] = (v[1]) * (factor), \
	(dst)[2] = (v[2]) * (factor)

#define VectorCopyM(src, dst) \
	(dst)[0] = (src[0]), \
	(dst)[1] = (src[1]), \
	(dst)[2] = (src[2])

#define VectorClearM(dst) \
	memset((dst), 0, sizeof((dst)[0]) * 3)

#define VectorScaleVector(a,b,c)		(((c)[0]=(a)[0]*(b)[0]),((c)[1]=(a)[1]*(b)[1]),((c)[2]=(a)[2]*(b)[2]))
#define VectorInverseScaleVector(a,b,c)	((c)[0]=(a)[0]/(b)[0],(c)[1]=(a)[1]/(b)[1],(c)[2]=(a)[2]/(b)[2])
#define VectorScaleVectorAdd(c,a,b,o)	((o)[0]=(c)[0]+((a)[0]*(b)[0]),(o)[1]=(c)[1]+((a)[1]*(b)[1]),(o)[2]=(c)[2]+((a)[2]*(b)[2]))
#define VectorAverage(a,b,c)			(((c)[0]=((a)[0]+(b)[0])*0.5f),((c)[1]=((a)[1]+(b)[1])*0.5f),((c)[2]=((a)[2]+(b)[2])*0.5f))
#define VectorNegate(a,b)				((b)[0]=-(a)[0],(b)[1]=-(a)[1],(b)[2]=-(a)[2])

// ======================================================================
// STRUCT
// ======================================================================

// plane_t structure
	typedef struct cplane_s
	{
		vec3_t	normal;
		float	dist;
		byte	type;			// for fast side tests: 0,1,2 = axial, 3 = nonaxial
		byte	signbits;		// signx + (signy<<1) + (signz<<2), used as lookup during collision
		byte	pad[2];
	} cplane_t;

// ======================================================================
// EXTERN VARIABLE
// ======================================================================

extern matrix3_t axisDefault;
extern vec2_t vec3_zero;
extern vec3_t vec3_origin;

// ======================================================================
// FUNCTION
// ======================================================================

// DIRECTION ENCODING
int DirToByte(vec3_t dir);
void ByteToDir(int b, vec3_t dir);
void NormalToLatLong(const vec3_t normal, byte bytes[2]);

// RANDOM NUMBER GENERATION
float erandom(float mean);
float flrand(float min, float max);
float Q_crandom(int* seed);
float Q_flrand(float min, float max);
float Q_random(int* seed);
int irand(int min, int max);
int Q_irand(int value1, int value2);
int Q_rand(int* seed);
void Rand_Init(int seed);

// MATH UTILITIES
#if defined(_MSC_VER)
static __inline long Q_ftol( float f )
{
	return (long)f;
}
#else
static inline long Q_ftol( float f )
{
	return (long)f;
}
#endif

float AngleDelta(float angle1, float angle2);
float AngleMod(float a);
float AngleNormalize180(float angle);
float AngleNormalize360(float angle);
float AngleSubtract(float a1, float a2);
float Com_AbsClamp(float min, float max, float value);
float Com_Clamp(float min, float max, float value);
float LerpAngle(float from, float to, float frac);
float Q_acos(float c);
float Q_asin(float c);
float Q_fabs(float f);
float Q_powf(float x, int y);
float Q_rsqrt(float number);
int Com_AbsClampi(int min, int max, int value);
int Com_Clampi(int min, int max, int value);
int Q_log2(int val);
bool Q_isnan(float f);
signed char ClampChar(int i);
signed short ClampShort(int i);
void AnglesSubtract(vec3_t v1, vec3_t v2, vec3_t v3);
void Q_LerpColour(const vec4_t start, const vec4_t end, vec4_t out, float point);

// GEOMETRIC UTILITIES
float G_PointDistFromLineSegment(const vec3_t start, const vec3_t end, const vec3_t from);
bool G_FindClosestPointOnLineSegment(const vec3_t start, const vec3_t end, const vec3_t from, vec3_t result);
bool PlaneFromPoints(vec4_t plane, const vec3_t a, const vec3_t b, const vec3_t c);
vec_t GetYawForDirection(const vec3_t p1, const vec3_t p2);
void GetAnglesForDirection(const vec3_t p1, const vec3_t p2, vec3_t out);
void MatrixMultiply(float in1[3][3], float in2[3][3], float out[3][3]);
void ProjectPointOnPlane(vec3_t dst, const vec3_t p, const vec3_t normal);
void RotateAroundDirection(matrix3_t axis, float yaw);
void RotatePointAroundVector(vec3_t dst, const vec3_t dir, const vec3_t point, float degrees);
void vectoangles(const vec3_t value1, vec3_t angles);

// BOUNDING BOX
float RadiusFromBounds(const vec3_t mins, const vec3_t maxs);
void AddPointToBounds(const vec3_t v, vec3_t mins, vec3_t maxs);
void ClearBounds(vec3_t mins, vec3_t maxs);

// PLANE
int BoxOnPlaneSide(vec3_t emins, vec3_t emaxs, cplane_t* p);
int	PlaneTypeForNormal(vec3_t normal);
void SetPlaneSignbits(cplane_t* out);

// AXIS
void AnglesToAxis(const vec3_t angles, matrix3_t axis);
void AxisClear(matrix3_t axis);
void AxisCopy(matrix3_t in, matrix3_t out);

// VEC2
void VectorAdd2(const vec2_t vec1, const vec2_t vec2, vec2_t vecOut);
void VectorClear2(vec2_t vec);
void VectorCopy2(const vec2_t vecIn, vec2_t vecOut);
void VectorMA2(const vec2_t vec1, float scale, const vec2_t vec2, vec2_t vecOut);
void VectorScale2(const vec2_t vecIn, float scale, vec2_t vecOut);
void VectorSet2(vec2_t vec, float x, float y);
void VectorSubtract2(const vec2_t vec1, const vec2_t vec2, vec2_t vec2_t);

// VEC3
float Distance(const vec3_t p1, const vec3_t p2);
float DistanceHorizontal(const vec3_t p1, const vec3_t p2);
float DistanceHorizontalSquared(const vec3_t p1, const vec3_t p2);
float DistanceSquared(const vec3_t p1, const vec3_t p2);
float DotProduct(const vec3_t vec1, const vec3_t vec2);
float DotProductNormalize(const vec3_t inVec1, const vec3_t inVec2);
float VectorLength(const vec3_t vec);
float VectorLengthSquared(const vec3_t vec);
float VectorNormalize(vec3_t vec);
float VectorNormalize2(const vec3_t vec, vec3_t vecOut);
bool VectorCompare(const vec3_t vec1, const vec3_t vec2);
bool VectorCompare2(const vec3_t v1, const vec3_t v2);
void AngleVectors(const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void CrossProduct(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut);
void MakeNormalVectors(const vec3_t forward, vec3_t right, vec3_t up);
void PerpendicularVector(vec3_t dst, const vec3_t src);
void SnapVector(float* v);
void VectorAdd(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut);
void VectorAdvance(const vec3_t veca, const float scale, const vec3_t vecb, vec3_t vecc);
void VectorClear(vec3_t vec);
void VectorCopy(const vec3_t vecIn, vec3_t vecOut);
void VectorDec(vec3_t vec);
void VectorInc(vec3_t vec);
void VectorInverse(vec3_t vec);
void VectorMA(const vec3_t vec1, float scale, const vec3_t vec2, vec3_t vecOut);
void VectorNormalizeFast(vec3_t vec);
void VectorRotate(const vec3_t in, matrix3_t matrix, vec3_t out);
void VectorScale(const vec3_t vecIn, float scale, vec3_t vecOut);
void VectorSet(vec3_t vec, float x, float y, float z);
void VectorSubtract(const vec3_t vec1, const vec3_t vec2, vec3_t vecOut);

// VEC4
void VectorClear4(vec4_t vec);
void VectorCopy4(const vec4_t vecIn, vec4_t vecOut);
void VectorScale4(const vec4_t vecIn, float scale, vec4_t vecOut);
void VectorSet4(vec4_t vec, float x, float y, float z, float w);

// VEC5
void VectorSet5(vec5_t vec, float x, float y, float z, float w, float u);

static inline int Round(float value)
{
	return((int)floorf(value + 0.5f));
}