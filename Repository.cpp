#include "Repository.h"

shared_ptr<VictimFile> InMemoryRepository::search(const string& name){
	vector<shared_ptr<VictimFile>>::iterator iterator = find_if(this->victimFiles.begin(), this->victimFiles.end(), [name](shared_ptr<VictimFile> victimFile) {return victimFile->getName() == name; });
	if (iterator != this->victimFiles.end())
		return *iterator;
	return NULL;
}

void InMemoryRepository::addVictimFile(shared_ptr<VictimFile> victimFile){
	if (this->search(victimFile->getName()) != NULL)
		throw RepositoryException("The victim file already exists!");
	this->victimFiles.push_back(victimFile);
}

shared_ptr<VictimFile> InMemoryRepository::updateVictimFile(shared_ptr<VictimFile> victimFile){
	shared_ptr<VictimFile> oldVictimFile;
	vector<shared_ptr<VictimFile>>::iterator iterator = find_if(this->victimFiles.begin(), this->victimFiles.end(), [victimFile](shared_ptr<VictimFile> existingVictimFile) {return existingVictimFile->getName() == victimFile->getName(); });
	if (iterator == this->victimFiles.end())
		throw RepositoryException("The victim file doesn't exist!");
	else {
		oldVictimFile = *iterator;
		*iterator = victimFile;
	}
	return oldVictimFile;
}

void InMemoryRepository::deleteVictimFile(shared_ptr<VictimFile> victimFile){
	if (this->search(victimFile->getName())->getName() == "")
		throw RepositoryException("The victim file doesn't exist!");
	remove(this->victimFiles.begin(), this->victimFiles.end(), victimFile);
	this->victimFiles.resize(this->victimFiles.size() - 1);
}





FileRepository::FileRepository(const string& fileLocation) : fileLocation{ fileLocation }{
	ifstream file(fileLocation);
	if (!file.is_open())
	{
		ofstream file;
		file.open(fileLocation);
		file.close();
	}
	else
		file.close();
}

shared_ptr<VictimFile> FileRepository::search(const string& name){
	VictimFile victimFile;
	ifstream file(this->fileLocation);
	if (file.is_open()) {
		while (!file.eof())
		{
			file >> victimFile;
			if (victimFile.getName() == name)
			{
				file.close();
				return make_shared<VictimFile>(victimFile);
			}
		}
		file.close();
	}
	return NULL;
}

void FileRepository::addVictimFile(shared_ptr<VictimFile> victimFile){
	if (this->search(victimFile->getName()) != NULL)
		throw RepositoryException("The victim file already exists!");
	ofstream file;
	file.open(this->fileLocation, ios::app);
	if (file.is_open())
	{
		file << *victimFile;
		file.close();
	}
}

shared_ptr<VictimFile> FileRepository::updateVictimFile(shared_ptr<VictimFile> victimFile){
	bool exists = false;
	vector<shared_ptr<VictimFile>> victimFiles;
	VictimFile existingVictimFile;
	VictimFile oldVictimFile;
	ifstream file(this->fileLocation);
	if (file.is_open()) {
		while (file >> existingVictimFile) {
			if (existingVictimFile.getName() == victimFile->getName()) {
				oldVictimFile = existingVictimFile;
				existingVictimFile = *victimFile;
				exists = true;
			}
			shared_ptr<VictimFile> victim = make_shared<VictimFile>(existingVictimFile);
			victimFiles.push_back(victim);
		}
		file.close();
		if (!exists)
			throw RepositoryException("The victim file doesn't exist!");
		ofstream fileOutput;
		fileOutput.open(this->fileLocation, ios::trunc);
		if (fileOutput.is_open()) {
			for (shared_ptr<VictimFile> victimFile : victimFiles)
				fileOutput << *victimFile;
			fileOutput.close();
		}
	}
	return make_shared<VictimFile>(oldVictimFile);
}

void FileRepository::deleteVictimFile(shared_ptr<VictimFile> victimFile){
	bool exists = false;
	vector<shared_ptr<VictimFile>> victimFiles;
	VictimFile existingVictimFile;
	ifstream file(this->fileLocation);
	if (file.is_open()) {
		while (file >> existingVictimFile) {
			if (existingVictimFile.getName() == victimFile->getName())
				exists = true;
			else {
				shared_ptr<VictimFile> victim = make_shared<VictimFile>(existingVictimFile);
				victimFiles.push_back(victim);
			}
		}
		file.close();
		if (!exists)
			throw RepositoryException("The victim file doesn't exist!");
		ofstream fileOutput;
		fileOutput.open(this->fileLocation, ios::trunc);
		if (fileOutput.is_open()) {
			for (shared_ptr<VictimFile> victimFile : victimFiles)
				fileOutput << *victimFile;
			fileOutput.close();
		}
	}
}

vector<shared_ptr<VictimFile>> FileRepository::repositoryGetVictimFiles() {
	vector<shared_ptr<VictimFile>> victimFiles{};
	VictimFile existingVictimFile;
	ifstream file(this->fileLocation);
	if (file.is_open()) {
		while (file >> existingVictimFile) {
			shared_ptr<VictimFile> victim = make_shared<VictimFile>(existingVictimFile);
			victimFiles.push_back(victim);
		}
		file.close();
	}
	return victimFiles;
}
