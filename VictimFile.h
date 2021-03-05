#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <fstream>
using namespace std;

class VictimFile
{
private:
	string name;
	string placeOfOrigin;
	int age;
	string photograph;

public:
	VictimFile();
	VictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph);
	VictimFile(const VictimFile& victimFile);
	~VictimFile() {}
	string getName() const { return name; }
	string getPlaceOfOrigin() const { return placeOfOrigin; }
	int getAge() const { return age; }
	string getPhotograph() const { return photograph; }
	VictimFile& operator=(const VictimFile& victimFile);
	bool operator==(const VictimFile& victimFile);
	string toString();
	friend istream& operator>>(istream& file, VictimFile& victimFile);
	friend ostream& operator<<(ostream& file, const VictimFile& victimFile);
};