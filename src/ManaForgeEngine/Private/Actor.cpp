#include "EnginePCH.h"

#include "Actor.h"

#include "Helper.h"
#include "Runtime.h"
#include "PhysicsSystem.h"

BOOST_CLASS_EXPORT_IMPLEMENT(Actor);

Actor::Actor()
	: transController(Runtime::get().getPhysicsSystem().newActorTransformController(*this))
	, GUID(Runtime::get().world->addActor(*this))
{
}

Actor::~Actor()
{
	while (components.end() != components.begin()) {
		MFLOG(Info) << "Component not memory managed. It is suggested to use std::unique_ptr or "
					   "std::shared_ptr for components.";
		delete *components.begin();
	}
}
