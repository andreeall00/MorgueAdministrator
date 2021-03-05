#include "Validator.h"

void Validator::validateVictimFile(shared_ptr<VictimFile> victimFile)
{
	string errors;
	if (victimFile->getName() == "")
		errors = errors + "Invalid name!";
	if(victimFile->getPlaceOfOrigin() == "")
		errors = errors + "Invalid place of origins!";
	if(victimFile->getAge() <= 0)
		errors = errors + "Invalid age!";
	if (victimFile->getPhotograph() == "")
		errors = errors + "Invalid photograph!";
	if (errors.length() > 0)
		throw ValidatorException(errors);
}
