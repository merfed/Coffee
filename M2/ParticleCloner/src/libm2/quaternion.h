#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include "vec3d.h"

class Vec4D {
public:
	float x,y,z,w;

	Vec4D(float x0=0.0f, float y0=0.0f, float z0=0.0f, float w0=0.0f) : x(x0), y(y0), z(z0), w(w0) {}

	Vec4D(const Vec4D& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	Vec4D(const Vec3D& v, const float w0) : x(v.x), y(v.y), z(v.z), w(w0) {}

	Vec4D& operator= (const Vec4D &v) {
        x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const Vec4D operator+ (const Vec4D &v) const
	{
		Vec4D r(x+v.x,y+v.y,z+v.z,w+v.w);
        return r;
	}

	const Vec4D operator- (const Vec4D &v) const
	{
		Vec4D r(x-v.x,y-v.y,z-v.z,w-v.w);
		return r;
	}

	const Vec4D operator* (float d) const
	{
		Vec4D r(x*d,y*d,z*d,w*d);
        return r;
	}

	friend Vec4D operator* (float d, const Vec4D& v)
	{
		return v * d;
	}

	Vec4D& operator+= (const Vec4D &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	Vec4D& operator-= (const Vec4D &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	Vec4D& operator*= (float d)
	{
		x *= d;
		y *= d;
		z *= d;
		w *= d;
		return *this;
	}

	float operator* (const Vec4D &v) const
	{
        return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	float lengthSquared() const
	{
		return x*x+y*y+z*z+w*w;
	}

	float length() const
	{
        return sqrtf(x*x+y*y+z*z+w*w);
	}

	Vec4D& normalize()
	{
		this->operator*= (1.0f/length());
		return *this;
	}

	operator float*()
	{
		return (float*)this;
	}

	Vec3D xyz() const
	{
		return Vec3D(x,y,z);
	}
};

class Quaternion: public Vec4D {
public:
	Quaternion(float x0=0.0f, float y0=0.0f, float z0=0.0f, float w0=1.0f): Vec4D(x0,y0,z0,w0) {}

	Quaternion(const Vec4D& v) : Vec4D(v) {}

	Quaternion(const Vec3D& v, const float w0) : Vec4D(v, w0) {}

	/*Quaternion(SQuaternion t){
			x=float(t.x < 0? t.x + 32768 : t.x - 32767)/ 32767.0f;
			y=float(t.y < 0? t.y + 32768 : t.y - 32767)/ 32767.0f;
			z=float(t.z < 0? t.z + 32768 : t.z - 32767)/ 32767.0f;
			w=float(t.w < 0? t.w + 32768 : t.w - 32767)/ 32767.0f;
	}*/

	static const Quaternion slerp(const float r, const Quaternion &v1, const Quaternion &v2)
	{
		// SLERP
		float dot = v1*v2;

		if (fabs(dot) > 0.9995f) {
			// fall back to LERP
			return Quaternion::lerp(r, v1, v2);
		}

		float a = acosf(dot) * r;
		Quaternion q = (v2 - v1 * dot);
		q.normalize();

		return v1 * cosf(a) + q * sinf(a);
	}

	static const Quaternion lerp(const float r, const Quaternion &v1, const Quaternion &v2)
	{
		return v1*(1.0f-r) + v2*r;
	}

};

//==ShortQuaternion
//for WoW
//todo:add conversion Quaternion/Vec4D<>SQuaterinion
class SQuaternion {
public:
	short x,y,z,w;

	SQuaternion(short x0=0, short y0=0, short z0=0, short w0=0) : x(x0), y(y0), z(z0), w(w0) {}

	SQuaternion(const SQuaternion& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	SQuaternion(Quaternion q32){
		x =(short)(q32.x > 0 ? q32.x * 32767.0 - 32768 : q32.x * 32767.0 + 32768);
		y =(short)(q32.y > 0 ? q32.y * 32767.0 - 32768 : q32.y * 32767.0 + 32768);
		z =(short)(q32.z > 0 ? q32.z * 32767.0 - 32768 : q32.z * 32767.0 + 32768);
		w =(short)(q32.w > 0 ? q32.w * 32767.0 - 32768 : q32.w * 32767.0 + 32768);
	}
	SQuaternion& operator= (const SQuaternion &v) {
        x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	const SQuaternion operator+ (const SQuaternion &v) const
	{
		SQuaternion r(x+v.x,y+v.y,z+v.z,w+v.w);
        return r;
	}

	const SQuaternion operator- (const SQuaternion &v) const
	{
		SQuaternion r(x-v.x,y-v.y,z-v.z,w-v.w);
		return r;
	}

	const SQuaternion operator* (short d) const
	{
		SQuaternion r(x*d,y*d,z*d,w*d);
        return r;
	}

	friend SQuaternion operator* (short d, const SQuaternion& v)
	{
		return v * d;
	}

	SQuaternion& operator+= (const SQuaternion &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	SQuaternion& operator-= (const SQuaternion &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	SQuaternion& operator*= (short d)
	{
		x *= d;
		y *= d;
		z *= d;
		w *= d;
		return *this;
	}

	short operator* (const SQuaternion &v) const
	{
        return x*v.x + y*v.y + z*v.z + w*v.w;
	}

	short lengthSquared() const
	{
		return x*x+y*y+z*z+w*w;
	}

	float length() const
	{
        return sqrtf(x*x+y*y+z*z+w*w);
	}

	SQuaternion& normalize()
	{
		this->operator*= (1/length());
		return *this;
	}

	operator short*()
	{
		return (short*)this;
	}


};


#endif


