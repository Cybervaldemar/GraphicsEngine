/*
 * Vector3.h
 *
 *  Created on: 22.04.2011
 *      Author: Norbert Nopper
 */

#ifndef VECTOR3_H_
#define VECTOR3_H_

class Point4;

class Vector3
{

	friend class Matrix4x4;
	friend class Matrix3x3;
	friend class Plane;
	friend class Point4;

private:

	float v[3];

public:

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(const float other[3]);
	Vector3(const Vector3 & other);
	~Vector3();

	Vector3 operator -() const;

	Vector3 operator +(const Vector3& other) const;
	Vector3 operator -(const Vector3& other) const;
	Vector3 operator *(const Vector3& other) const;
	Vector3 operator *(float scalar) const;

	Vector3& operator =(const Vector3& vector);

	Vector3& operator +=(const Vector3& vector);
	Vector3& operator -=(const Vector3& vector);

	float getX() const;
	void setX(float x);
	float getY() const;
	void setY(float y);
	float getZ() const;
	void setZ(float z);

	const float* getV() const;

	Vector3 normalize() const;
	float length() const;
	float dot(const Vector3& other) const;
	Vector3 cross(const Vector3& other) const;

};

#endif /* VECTOR3_H_ */
