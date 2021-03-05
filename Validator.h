#pragma once
#include "VictimFile.h"
#include "Exception.h"

class Validator
{
public:
	Validator() {}
	void validateVictimFile(shared_ptr<VictimFile> victimFile);
};