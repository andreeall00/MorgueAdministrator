#pragma once
#include "VictimFile.h"
#include "Exception.h"


class Repository
{
public:
	virtual shared_ptr<VictimFile> search(const string& name) = 0;
	virtual void addVictimFile(shared_ptr<VictimFile> victimFile) = 0;
	virtual shared_ptr<VictimFile> updateVictimFile(shared_ptr<VictimFile> victimFile) = 0;
	virtual void deleteVictimFile(shared_ptr<VictimFile> victimFile) = 0;
	virtual vector<shared_ptr<VictimFile>> repositoryGetVictimFiles() = 0;
	virtual ~Repository() {}
};


class InMemoryRepository: public Repository
{
protected:
	 vector<shared_ptr<VictimFile>> victimFiles;

public:
	InMemoryRepository() {}

	shared_ptr<VictimFile> search(const string& name) override;
	void addVictimFile(shared_ptr<VictimFile> victimFile) override;
	shared_ptr<VictimFile> updateVictimFile(shared_ptr<VictimFile> victimFile) override;
	void deleteVictimFile(shared_ptr<VictimFile> victimFile) override;
	vector<shared_ptr<VictimFile>> repositoryGetVictimFiles() override { return victimFiles; }
};


class FileRepository : public Repository
{
protected:
	string fileLocation;

public:
	FileRepository(const string& fileLocation);
	string getFileLocation() { return this->fileLocation; }
	virtual string getType() { return "txt"; }

	shared_ptr<VictimFile> search(const string& name);
	void addVictimFile(shared_ptr<VictimFile> victimFile);
	shared_ptr<VictimFile> updateVictimFile(shared_ptr<VictimFile> victimFile);
	void deleteVictimFile(shared_ptr<VictimFile> victimFile);
	vector<shared_ptr<VictimFile>> repositoryGetVictimFiles();
};

