﻿#pragma once

#include "Engine.h"
#include <exception>
#include <string>

class ENGException : std::exception
{
public:
	virtual ENGINE_API const char* what() const override;

	ENGINE_API ENGException(std::string reasonIn, std::string stack);
	

private:
	std::string reason;
};
