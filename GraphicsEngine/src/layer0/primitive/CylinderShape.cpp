/*
 * CylinderShape.cpp
 *
 *  Created on: 09.11.2012
 *      Author: nopper
 */

#include "CylinderShape.h"

using namespace boost;

CylinderShape::CylinderShape(float halfExtend, float radius, uint32_t numberSlices) : Shape()
{
	glusCreateCylinderf(&shape, halfExtend, radius, numberSlices);
}

CylinderShape::~CylinderShape()
{
}
