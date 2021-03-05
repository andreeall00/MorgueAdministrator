#include "HTMLRepository.h"

HTMLRepository::HTMLRepository(const string& fileLocation) : FileRepository(fileLocation)
{
	ifstream file(this->fileLocation);
	string line;
	getline(file, line);
	if (line.size() == 0)
	{
		file.close();
		ofstream file;
		file.open(this->fileLocation, ios::trunc);
		this->writeToHTML(vector<shared_ptr<VictimFile>>{});
		file.close();
	}
	else
		file.close();
}

vector<shared_ptr<VictimFile>> HTMLRepository::readFromHTML()
{
	vector<shared_ptr<VictimFile>> victimFiles{};
	ifstream file(this->fileLocation);
	if (!file.is_open())
		throw exception("Unable to open file!");
	vector<string> arguments;
	string line;
	getline(file, line);
	while (line != "\t\t</tr>")
		getline(file, line);
	getline(file, line); // <tr>
	while (line != "\t</table>")
	{
		for (int i = 1; i <= 3; i++)
		{
			getline(file, line); // name, placeOfOrigin, age
			line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
			line.erase(0, 4);
			for (int i = 1; i <= 5; i++)
				line.pop_back();
			arguments.push_back(line);
		}
		getline(file, line); // photograph
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());
		line.erase(0, 13);
		for (int i = 1; i <= 8; i++)
			line.pop_back();
		arguments.push_back(line);
		getline(file, line); // </tr>
		getline(file, line); // <tr> or </table>
		shared_ptr<VictimFile> victim = make_shared<VictimFile>(arguments[0], arguments[1], stoi(arguments[2]), arguments[3]);
		victimFiles.push_back(victim);
		arguments.clear();
	}
	file.close();
	return victimFiles;
}

void HTMLRepository::writeToHTML(vector<shared_ptr<VictimFile>> victimFiles) {
	ofstream file;
	file.open(this->fileLocation, ios::trunc);
	if (file.is_open())
	{
		file << "<!DOCTYPE html>\n<html>\n<head>\n\t<title>VictimFiles</title>\n</head>\n<body>\n\t<table border=\"1\">\n\t\t<tr>\n\t\t\t<td>Name</td>\n\t\t\t<td>Place of origin</td>\n\t\t\t<td>Age</td>\n\t\t\t<td>Photograph</td>\n\t\t</tr>";
		string outputString;
		for (auto victimFile : victimFiles)
		{
			outputString = "\n\t\t<tr>\n\t\t\t<td>" + victimFile->getName() + "</td>\n\t\t\t<td>" + victimFile->getPlaceOfOrigin() + "</td>\n\t\t\t<td>" + to_string(victimFile->getAge()) + "</td>\n\t\t\t<td><img src=\"" + victimFile->getPhotograph() + "\"/></td>\n\t\t</tr>";
			file << outputString;
		}
		file << "\n\t</table>\n</body>\n</html>";
		file.close();
	}
}

shared_ptr<VictimFile> HTMLRepository::search(const string& name){
	vector<shared_ptr<VictimFile>> victimFiles = this->readFromHTML();
	for (auto existingFile : victimFiles)
		if (existingFile->getName() == name)
			return existingFile;
	return NULL;
}

void HTMLRepository::addVictimFile(shared_ptr<VictimFile> victimFile) {
	vector<shared_ptr<VictimFile>> victimFiles = this->readFromHTML();
	for (shared_ptr<VictimFile> existingFile : victimFiles)
		if (*existingFile == *victimFile)
			throw RepositoryException("The file already exists!");
	victimFiles.push_back(victimFile);
	this->writeToHTML(victimFiles);
}

shared_ptr<VictimFile> HTMLRepository::updateVictimFile(shared_ptr<VictimFile> victimFile){
	shared_ptr<VictimFile> oldVictimFile;
	vector<shared_ptr<VictimFile>> victimFiles = this->readFromHTML();
	vector<shared_ptr<VictimFile>>::iterator iterator = find_if(victimFiles.begin(), victimFiles.end(), [victimFile](shared_ptr<VictimFile> existingVictimFile) {return existingVictimFile->getName() == victimFile->getName(); });
	if (iterator == victimFiles.end())
		throw RepositoryException("The victim file doesn't exist!");
	else {
		oldVictimFile = *iterator;
		*iterator = victimFile;
	}
	this->writeToHTML(victimFiles);
	return oldVictimFile;
}

void HTMLRepository::deleteVictimFile(shared_ptr<VictimFile> victimFile){
	vector<shared_ptr<VictimFile>> victimFiles = this->readFromHTML();
	if (this->search(victimFile->getName())->getName() == "")
		throw RepositoryException("The victim file doesn't exist!");
	remove(victimFiles.begin(), victimFiles.end(), victimFile);
	victimFiles.resize(victimFiles.size() - 1);
	this->writeToHTML(victimFiles);
}
