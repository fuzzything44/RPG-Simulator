#include "DefaultWorldConfig.h"

#include <ModuleManager.h>
#include <DefaultWorld.h>


World* AddWorld(std::string folder)
{
	return new DefaultWorld(folder);
}


extern "C" DefualtWorld_API void registerModule(ModuleManager& mm)
{
	mm.setWorld(AddWorld);
}

extern "C" DefualtWorld_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}