#include "EnginePCH.h"

#include "InputManager.h"

#include "Runtime.h"
#include "SaveData.h"
#include "WindowWidget.h"

#include <boost/archive/xml_iarchive.hpp>

#include <boost/lexical_cast.hpp>

InputManager::InputManager(WindowWidget* window)
	: window(window)
{
	std::ifstream i_stream{"bindings.txt"};

	if (!i_stream.is_open()) {
		MFLOG(Fatal) << "bindings.txt not found";
	}

	try
	{

		boost::archive::xml_iarchive i_archive{i_stream};

		i_archive >> BOOST_SERIALIZATION_NVP(actionMappings);
		i_archive >> BOOST_SERIALIZATION_NVP(axisMappings);
	}
	catch (boost::archive::archive_exception& e)
	{
		MFLOG(Fatal) << "ARCHIVE EXCEPTION OCCURED IN LOADING INPUT MANAGER. Code: " << e.code
					 << " Error message: " << e.what();
	}
	catch (std::exception& e)
	{
		MFLOG(Fatal) << e.what();
	}
}

void InputManager::setWindow(WindowWidget& newWindow) { window = &newWindow; }

void InputManager::bindActionMappingPressed(const std::string& name, std::function<void()> callback)
{
	auto iter = actionMappings.find(name);

	if (iter != actionMappings.end()) {
		iter->second.pressed = callback;
	}
	else
	{
		MFLOG(Warning) << "action mapping \"" << name << "\" not found";
	}
}

void InputManager::bindActionMappingReleased(const std::string& name, std::function<void()> callback)
{
	auto iter = actionMappings.find(name);

	if (iter != actionMappings.end()) {
		iter->second.released = callback;
	}
	else
	{
		MFLOG(Warning) << "action mapping \"" << name << "\" not found";
	}
}

void InputManager::bindAxisMapping(const std::string& name, std::function<void(float)> callback)
{
	auto iter = axisMappings.find(name);

	if (iter != axisMappings.end()) {
		iter->second.callback = callback;
	}
	else
	{
		MFLOG(Warning) << "axis mapping \"" << name << "\" not found";
	}
}

bool InputManager::update()
{

	for (auto& elem : actionMappings) {
		// call the operator() function
		elem.second();
	}

	for (auto& elem : axisMappings) {
		// call the operator() function
		elem.second();
	}

	return true;
}

InputManager::ActionMapping::ActionMapping(const std::vector<Keyboard>& keys)
	: keysBound(keys)
	, isPressed(false)
{
}

InputManager::ActionMapping::~ActionMapping() {}

void InputManager::ActionMapping::setPressedCallback(std::function<void()> fun) { pressed = fun; }

void InputManager::ActionMapping::setReleasedCallback(std::function<void()> fun) { released = fun; }

void InputManager::ActionMapping::operator()()
{
	// if neither of the events exist, return.
	if (!pressed && !released) {
		return;
	}

	bool isPressedNew = false;
	for (auto& elem : keysBound) {
		// if there is one that is pressed, the entire binding is pressed,
		// so break
		if (Runtime::get().getInputManager().window->getIsKeyPressed(elem)) {
			isPressedNew = true;
			break;
		}
	}

	// if there is no change, return
	if (isPressedNew == isPressed) return;

	// if isPressedNew is true and isPressed is false, then we just pressed the key.
	// also checks the pressed function pointer
	if (isPressedNew && !isPressed && pressed) pressed();

	if (!isPressedNew && isPressed && released) released();
}

InputManager::AxisMapping::AxisMapping(const std::vector<std::pair<Keyboard, float>>& values)
	: values(values)
{
}

InputManager::AxisMapping::~AxisMapping() {}

void InputManager::AxisMapping::setCallback(std::function<void(float)> newCallback)
{
	callback = newCallback;
}

void InputManager::AxisMapping::operator()()
{
	float totalValue = 0.f;

	for (auto& elem : values) {
		// compute the amount to add -- without branches
		totalValue += Runtime::get().getInputManager().window->getIsKeyPressed(elem.first) * elem.second;
	}

	if (callback) callback(totalValue);
}