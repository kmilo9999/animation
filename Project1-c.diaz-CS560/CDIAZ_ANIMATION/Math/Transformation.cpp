#include "Transformation.h"


Transformation::Transformation() : Position(),Orientation(),Scale(Vector3(1,1,1))
{
}

Transformation::Transformation(Vector3 position, Quaternion orientation, Vector3 scale) :
							Position(position), Orientation(orientation), Scale(scale)
{
}

Transformation::Transformation(Transformation& t) : Position(t.Position), Orientation(t.Orientation),
													Scale(t.Scale)
{

}

Transformation::~Transformation()
{
}






