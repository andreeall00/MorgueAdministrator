#pragma once
#include "HTMLRepository.h"
#include "Validator.h"
#include "Action.h"

class Service
{
private:
	Repository* repository;
	Validator validator;
	Repository* myList;
	int currentPosition;
	int first;
	vector< unique_ptr<Action> > undoStackRepository{};
	vector< unique_ptr<Action> > redoStackRepository{};
	vector< unique_ptr<ActionMylist> > undoStackMylist{};
	vector< unique_ptr<ActionMylist> > redoStackMylist{};

public:
	Service(Repository* repository, Repository* myList, Validator validator) : repository{ repository }, myList{ myList }, validator{ validator }, currentPosition{ 0 }, first{ 0 } {}
	void serviceAddVictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph);
	void serviceUpdateVictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph);
	void serviceDeleteVictimFile(const string& name);
	vector<shared_ptr<VictimFile>> serviceGetVictimFiles();
	void undoRepository();
	void redoRepository();

	Repository* getPointerMylist() { return myList; }
	shared_ptr<VictimFile> next();
	shared_ptr<VictimFile> previous();
	void save(const string& name);
	vector<shared_ptr<VictimFile>> getMyList() const;
	vector<shared_ptr<VictimFile>> getFiltered(const string& placeOfOrigin, int age) const;
	string getFileType();
	string getFileLocation();
	string undoMylist();
	string redoMylist();
};