#include "VictimFile.h"

VictimFile::VictimFile() : name(""), placeOfOrigin(""), age(0), photograph("") {}

VictimFile::VictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph)
{
	this->name = name;
	this->placeOfOrigin = placeOfOrigin;
	this->age = age;
	this->photograph = photograph;
}

VictimFile::VictimFile(const VictimFile& victimFile)
{
	this->name = victimFile.name;
	this->placeOfOrigin = victimFile.placeOfOrigin;
	this->age = victimFile.age;
	this->photograph = victimFile.photograph;
}


VictimFile& VictimFile::operator=(const VictimFile& victimFile)
{
	if (this == &victimFile)
		return *this; 
	this->name = victimFile.getName();
	this->placeOfOrigin = victimFile.getPlaceOfOrigin();
	this->age = victimFile.getAge();
	this->photograph = victimFile.getPhotograph();
	return *this;
}

bool VictimFile::operator==(const VictimFile& victimFile)
{
	if (this->name == victimFile.getName() && this->placeOfOrigin == victimFile.getPlaceOfOrigin() && this->age == victimFile.getAge() && this->photograph == this->getPhotograph())
		return true;
	return false;
}

string VictimFile::toString()
{
	return this->name + ": " + this->placeOfOrigin + ", " + to_string(this->age) + ", " + this->photograph;
}

vector<string> tokenize(string line, char delimiter)
{
	vector<string> tokens;
	stringstream stringStream(line);
	string token;
	while (getline(stringStream, token, delimiter))
		tokens.push_back(token);
	return tokens;
}

istream& operator>>(istream& inputStream, VictimFile& victimFile)
{
	string line;
	getline(inputStream, line);
	vector<string> tokens = tokenize(line, ',');
	if (tokens.size() != 4)
		return inputStream;
	victimFile.name = tokens[0];
	victimFile.name.erase(remove_if(victimFile.name.begin(), victimFile.name.end(), isspace), victimFile.name.end());
	victimFile.placeOfOrigin = tokens[1];
	victimFile.placeOfOrigin.erase(remove_if(victimFile.placeOfOrigin.begin(), victimFile.placeOfOrigin.end(), isspace), victimFile.placeOfOrigin.end());
	victimFile.age = stoi(tokens[2]);
	victimFile.photograph = tokens[3];
	victimFile.photograph.erase(remove_if(victimFile.photograph.begin(), victimFile.photograph.end(), isspace), victimFile.photograph.end());
	return inputStream;
}

ostream& operator<<(ostream& outputStream, const VictimFile& victimFile)
{
	outputStream << victimFile.getName() << ", " << victimFile.getPlaceOfOrigin() << ", " << to_string(victimFile.getAge()) << ", " << victimFile.getPhotograph() << endl;
	return outputStream;
}
