#include "EnginePCH.h"

#include "Actor.h"

#include "Helper.h"
#include "Runtime.h"
#include "PhysicsSystem.h"



BOOST_CLASS_EXPORT_IMPLEMENT(Actor);

Actor::Actor()
	:transController(Runtime::get().moduleManager.getPhysicsSystem().newActorTransformController(*this)),
	GUID(Runtime::get().world->addActor(*this))
{
}

Actor::~Actor()
{
	
	for (auto elem : components)
	{
		delete elem;
	}

}



bool Actor::needsSave()
{
	// by default returns false
	return false;
}

void Actor::tick(float deltaTime)
{
	
}
