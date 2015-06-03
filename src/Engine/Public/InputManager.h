#pragma once

#include "Engine.h"
#include "KeyEnum.h"
#include "Window.h"

#include <map>
#include <vector>
#include <functional>

#include <SaveData.h>


class Runtime;

class InputManager
{
public:

	ENGINE_API explicit InputManager(Window* window = nullptr);

	ENGINE_API void setWindow(Window& newWindow);


	ENGINE_API void bindActionMappingPressed(const std::string& name, std::function<void()> callback);
	ENGINE_API void bindActionMappingReleased(const std::string& name, std::function<void()> callback);

	ENGINE_API void bindAxisMapping(const std::string& name, std::function<void(float)> callback);

private:

	ENGINE_API bool update();

	Window* window;

	struct ActionMapping
	{
		friend class boost::serialization::access;

		// default constructor
		ENGINE_API explicit ActionMapping(const std::vector<Keyboard>& keys = std::vector<Keyboard>());

		ENGINE_API ~ActionMapping();

		void setPressedCallback(std::function<void()> fun);

		void setReleasedCallback(std::function<void()> fun);

		std::vector<Keyboard> keysBound;

		// function pointers -- they are pointers to make sure they are all allocated and freed in the same module.
		std::function<void()> pressed;
		std::function<void()> released;

		bool isPressed;

		void operator()();

		template<typename Archive>
		void serialize(Archive& ar, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(keysUsed);
		}
	};

	struct AxisMapping
	{
		friend class boost::serialization::access;

		ENGINE_API AxisMapping(const std::vector<std::pair<Keyboard, float> >& values = std::vector<std::pair<Keyboard, float> >());

		ENGINE_API ~AxisMapping();

		// vector of the values of the keys
		std::vector < std::pair<Keyboard, float> > values;

		std::function<void(float)> callback;

		void setCallback(std::function<void(float)> newCallback);

		void operator()();

		template<typename Archive>
		void serialize(Archive& ar, const uint32 version)
		{
			ar & BOOST_SERIALIZATION_NVP(values);
		}
	};


	std::map<std::string, ActionMapping> actionMappings; 
	std::map<std::string, AxisMapping> axisMappings;

	friend AxisMapping;
	friend ActionMapping;
	friend Runtime;

	friend boost::serialization::access;
	
};


