/*
 * CubeShape.cpp
 *
 *  Created on: 09.11.2012
 *      Author: nopper
 */

#include "CubeShape.h"

CubeShape::CubeShape(float halfExtend) : Shape()
{
	glusCreateCubef(&shape, halfExtend);
}

CubeShape::~CubeShape()
{
}
