#pragma once
#include <math.h>

class Vector3D
{
public:
	// Data
	float x, y, z;

	// Constructors
	Vector3D( float _x, float _y, float _z ) : x( _x ), y( _y ), z( _z )
		{
		}
	Vector3D( ) : x(0), y(0), z(0)
		{
		}

	// Operator Overloads
	inline bool operator== (const Vector3D& V2) const 
		{
		return (x == V2.x && y == V2.y && z == V2.z);
		}

	inline Vector3D operator+ (const Vector3D& V2) const 
		{
		return Vector3D( x + V2.x,  y + V2.y,  z + V2.z);
		}
	inline Vector3D operator- (const Vector3D& V2) const
		{
		return Vector3D( x - V2.x,  y - V2.y,  z - V2.z);
		}
	inline Vector3D operator- ( ) const
		{
		return Vector3D(-x, -y, -z);
		}

	inline Vector3D operator/ (float S ) const
		{
		float fInv = 1.0f / S;
		return Vector3D (x * fInv , y * fInv, z * fInv);
		}
	inline Vector3D operator/ (const Vector3D& V2) const
		{
		return Vector3D (x / V2.x,  y / V2.y,  z / V2.z);
		}
	inline Vector3D operator* (const Vector3D& V2) const
		{
		return Vector3D (x * V2.x,  y * V2.y,  z * V2.z);
		}
	inline Vector3D operator* (float S) const
		{
		return Vector3D (x * S,  y * S,  z * S);
		}

	inline void operator+= ( const Vector3D& V2 )
		{
		x += V2.x;
		y += V2.y;
		z += V2.z;
		}
	inline void operator-= ( const Vector3D& V2 )
		{
		x -= V2.x;
		y -= V2.y;
		z -= V2.z;
		}

	inline float operator[] ( int i )
		{
		if ( i == 0 ) return x;
		if ( i == 1 ) return y;
		return z;
		}

	// Functions
	inline float Dot( const Vector3D &V1 ) const
		{
		return V1.x*x + V1.y*y + V1.z*z;
		}

	inline Vector3D CrossProduct( const Vector3D &V2 ) const
		{
		return Vector3D(
			y * V2.z  -  z * V2.y,
			z * V2.x  -  x * V2.z,
			x * V2.y  -  y * V2.x 	);
		}

	// Vector Length
	float Length( ) const
		{
		return sqrtf( x*x + y*y + z*z );
		}

	float Distance( const Vector3D &V1 ) const
		{
		return ( *this - V1 ).Length();	
		}

	inline Vector3D Normalize()
		{
		float fMag = ( x*x + y*y + z*z );
		if (fMag == 0) {return Vector3D(0,0,0);}

		float fMult = 1.0f/sqrtf(fMag);            
		x *= fMult;
		y *= fMult;
		z *= fMult;
		return Vector3D(x,y,z);
		}

	static float *arr;

	float *Array()
	{
		arr[0] = x;
		arr[1] = y;
		arr[2] = z;
		return arr;
	}
};
