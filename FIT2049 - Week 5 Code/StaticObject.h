#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include "GameObject.h"

class StaticObject : public GameObject
{
public:
	StaticObject() : GameObject() {}
	StaticObject(Mesh* mesh, Vector3 position) : GameObject(mesh, position) {}

	void Update(float timestep) {}
};

#endif