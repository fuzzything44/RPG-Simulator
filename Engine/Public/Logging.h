#pragma once
#include "Engine.h"
#include "ENGException.h"

#include <fstream>
#include <iostream>

/// <summary> A logging.</summary>
class logging
{
public:
	/// <summary> Initialises this object.</summary>
	static ENGINE_API void init();

	static ENGINE_API std::fstream logfile;

private:

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOG(ans)logging::logfile << ans; std::cout << ans;

/// <summary> Called upon a fatal error.</summary>
#define FATAL_ERR(message, err) throw ENGException("message", err, __FILE__, __LINE__)
