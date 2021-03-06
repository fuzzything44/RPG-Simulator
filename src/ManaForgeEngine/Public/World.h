#pragma once
#include "Engine.h"

#include <boost/signals2/connection.hpp>

#include <string>

class Actor;
class ModuleManager;
class Pawn;
class PlayerController;

// Struct used to find actor in the world.
struct ActorLocation
{
	virtual ~ActorLocation() = default;
};

class World
{
public:
	virtual ~World() = default;

	virtual void init(const std::string& name) = 0;

	virtual void saveWorld() = 0;

	virtual std::unique_ptr<PlayerController> makePlayerController() = 0;
	virtual std::unique_ptr<Pawn> makePawn() = 0;

	virtual std::unique_ptr<ActorLocation> addActor(Actor& toAdd) = 0;

	virtual boost::signals2::connection registerTickingActor(const std::function<void(float)>& tickFun) = 0;
};