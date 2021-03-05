#pragma once
#include "Repository.h"

class HTMLRepository : public FileRepository
{
public:
	HTMLRepository(const string& fileLocation);
	vector<shared_ptr<VictimFile>> readFromHTML();
	void writeToHTML(vector<shared_ptr<VictimFile>> victimFiles);

	string getType() override { return "html"; }
	shared_ptr<VictimFile> search(const string& name)  override;
	void addVictimFile(shared_ptr<VictimFile> victimFile) override;
	shared_ptr<VictimFile> updateVictimFile(shared_ptr<VictimFile> victimFile) override;
	void deleteVictimFile(shared_ptr<VictimFile> victimFile) override;
	vector<shared_ptr<VictimFile>> repositoryGetVictimFiles() override { return this->readFromHTML(); }
};

