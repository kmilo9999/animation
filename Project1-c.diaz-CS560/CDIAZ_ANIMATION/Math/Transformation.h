#pragma once
#include "Vector3.h"
#include "Quaternion.h"

class Transformation
{
public:
	Transformation();
	Transformation(Vector3, Quaternion, Vector3);
	Transformation(Transformation&);
	~Transformation();
	

	

	Vector3 Position;
	Quaternion Orientation;
	Vector3 Scale;

private:

	
	
};

