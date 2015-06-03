#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"

#include <functional>
#include <list>
#include <chrono>

#include <boost/algorithm/string.hpp>

#include <boost/timer/timer.hpp>



// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath)
	:moduleManager((changeDir(), logging::init(), *this)), 
	propManager("props.json")
{

	// update current runtime to be the most recently created one
	currentRuntime = this;

	// query the property manager to get the modules
	std::string modulesStr = propManager.queryValue<std::string>("modules");

	// split into the individual modules
	std::vector<std::string> modules;
	boost::algorithm::split(modules, modulesStr, boost::algorithm::is_any_of("\t\n "));

	// load modules from the property sheet
	for (auto& elem : modules)
	{
		if (elem != "" && elem != "\t" && elem != "\n")
			moduleManager.loadModule(elem);
	}

	// load the world
	world = moduleManager.newWorld(worldPath);

	Window& window = moduleManager.getRenderer().getWindow();
	inputManager.setWindow(window);
}

Runtime::~Runtime()
{
	moduleManager.deleteWorld(world);
}

void Runtime::run()
{
	{
		// time the init time
		boost::timer::cpu_timer t;

		std::list<std::function<void()>* >& initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			(*callback)();
		}
		ENG_LOGLN("init completed. Timestamp: " << t.format());

		
	}

	// YA WE REALLY NEED PLAYER CONTROLLERS -- the gate shouldn't get to control the camera
	Actor* gate = moduleManager.spawnActor("TestContent.Gate", Transform{ });

	uvec2 windowSize = moduleManager.getRenderer().getWindow().getWindowProps().size;

	float aspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	aspectRatio *= 10;

	mat4 mat = glm::ortho(-aspectRatio, aspectRatio, -10.f, 10.f, .1f, 100.f);
	CameraComponent* c = new CameraComponent{ gate, Transform{}, mat };
	moduleManager.getRenderer().setCurrentCamera(c);

	




	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	float scale = 1.f;
	vec2 location{ 0.f, 0.f };
	float baseSpeed = 100.f;

	do {

		// calculate tick time
		clock::time_point CurrentTick = clock::now();
		float delta = static_cast<float>((CurrentTick - LastTick).count()) * 
			((float)clock::time_point::duration::period::num / (float)clock::time_point::duration::period::den);

		LastTick = CurrentTick;

		inputManager.update();


		Window& window = moduleManager.getRenderer().getWindow();


		float speedUpdated = window.getIsKeyPressed(Keyboard::KEY_LEFT_SHIFT) || window.getIsKeyPressed(Keyboard::KEY_RIGHT_SHIFT) ?
			5.f * baseSpeed : baseSpeed;

		if (window.getIsKeyPressed(Keyboard::KEY_Q))
		{
			scale *= 1.01f;
		}
		if (window.getIsKeyPressed(Keyboard::KEY_E))
		{
			scale *= 1.f / 1.01f;
		}
		if (window.getIsKeyPressed(Keyboard::KEY_A))
		{
			location += vec2(speedUpdated * delta, 0.f);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_D))
		{
			location += vec2(-speedUpdated * delta, 0.f);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_W))
		{
			location += vec2(0.f, -speedUpdated * delta);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_S))
		{
			location += vec2(0.f, speedUpdated * delta);
		}

		mat4 view = glm::ortho(-aspectRatio, aspectRatio, -10.f, 10.f, .1f, 100.f);

		view = glm::scale(view, vec3(scale, scale, 1.f));
		view = glm::translate(view, vec3(location.x, location.y, 0.f));

		c->setViewMat(view);

		// recieve the update callbacks
		std::list<std::function<bool()>* >& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if(!(*callback)())
			{
				shouldContinue = false;
			}
		}
		


	} while (shouldContinue);

}

Runtime& Runtime::get()
{
	if (!currentRuntime)
	{
		FATAL_ERR("NO RUNTIME OBJECT!");
	}

	return *currentRuntime;
}