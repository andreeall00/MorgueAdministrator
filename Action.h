#pragma once
#include "Repository.h"


class Action {
public:
	virtual void executeUndo() = 0;
	virtual void executeRedo() = 0;
	virtual void setExecute(bool execute) = 0;
	virtual VictimFile getVictim() = 0;
	virtual ~Action() {}
};


class ActionAdd : public Action
{
private:
	Repository* repository;
	Repository* mylist;
	shared_ptr<VictimFile> victim;
	bool executeInMylist;

public:
	ActionAdd(Repository* repository, Repository* mylist, shared_ptr<VictimFile> victim, bool executeInMylist) : repository{ repository }, mylist{ mylist }, victim{ victim }, executeInMylist{ executeInMylist } {}
	void executeUndo() { 
		repository->deleteVictimFile(victim);
		if(executeInMylist)
			mylist->deleteVictimFile(victim);
	}
	void executeRedo() { 
		repository->addVictimFile(victim); 
		if (executeInMylist)
			mylist->addVictimFile(victim);
	}
	void setExecute(bool execute) { executeInMylist = execute; }
	virtual VictimFile getVictim() { return *victim; }
};


class ActionRemove : public Action
{
private:
	Repository* repository;
	Repository* mylist;
	shared_ptr<VictimFile> victim;
	bool executeInMylist;

public:
	ActionRemove(Repository* repository, Repository* mylist, shared_ptr<VictimFile> victim, bool executeInMylist) : repository{ repository }, mylist{ mylist }, victim{ victim }, executeInMylist{ executeInMylist } {}
	void executeUndo() {
		repository->addVictimFile(victim); 
		if (executeInMylist)
			mylist->addVictimFile(victim);
	}
	void executeRedo() {
		repository->deleteVictimFile(victim); 
		if (executeInMylist)
			mylist->deleteVictimFile(victim);
	}
	void setExecute(bool execute) { executeInMylist = execute; }
	virtual VictimFile getVictim() { return *victim; }
};


class ActionUpdate : public Action
{
private:
	Repository* repository;
	Repository* mylist;
	shared_ptr<VictimFile> victimBefore;
	shared_ptr<VictimFile> victimAfter;
	VictimFile victimLast;
	bool executeInMylist;

public:
	ActionUpdate(Repository* repository, Repository* mylist, shared_ptr<VictimFile> victimB, shared_ptr<VictimFile> victimA, bool executeInMylist) : repository{ repository }, mylist{ mylist }, victimBefore{ victimB }, victimAfter{ victimA }, victimLast{ *victimA }, executeInMylist{ executeInMylist }{}
	void executeUndo() { 
		repository->updateVictimFile(victimBefore); 
		victimLast = *victimBefore; 
		if (executeInMylist)
			mylist->updateVictimFile(victimBefore);
	}
	void executeRedo() { 
		repository->updateVictimFile(victimAfter); 
		victimLast = *victimBefore; 
		if (executeInMylist)
			mylist->updateVictimFile(victimAfter);
	}
	bool getExecute() { return executeInMylist; }
	void setExecute(bool execute) { executeInMylist = execute; }
	virtual VictimFile getVictim() { return victimLast; }
};




class ActionMylist{
public:
	virtual string getAction() = 0;
	virtual ~ActionMylist() {}
};


class ActionSave: public ActionMylist
{
private:
	Repository* repository;
	Repository* mylist;
	string name;
	string actionName;

public:
	ActionSave(Repository* repository, Repository* mylist, string name, string actionName) : repository{ repository }, mylist{ mylist }, name{ name }, actionName{ actionName } {}
	void executeUndo() {
		shared_ptr<VictimFile> victim = repository->search(name);
		mylist->deleteVictimFile(victim);
	}
	void executeRedo() {
		shared_ptr<VictimFile> victim = repository->search(name);
		mylist->addVictimFile(victim);
	}
	bool stillExists() { 
		shared_ptr<VictimFile> victim = repository->search(name);
		if (victim == NULL)
			return false;
		return true;
	}
	VictimFile getVictim() { return *repository->search(name); }
	string getAction() { return actionName; }
};


class ActionNext : public ActionMylist
{
private:
	string actionName;

public:
	ActionNext(string actionName) : actionName{ actionName } {}
	string getAction() { return actionName; }
};
