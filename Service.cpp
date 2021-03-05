#include "Service.h"

void Service::serviceAddVictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph){
	shared_ptr<VictimFile> victimFile = make_shared<VictimFile>(name, placeOfOrigin, age, photograph);
	this->validator.validateVictimFile(victimFile);
	unique_ptr<Action> action = make_unique<ActionAdd>(this->repository, this->myList, victimFile, false);
	this->repository->addVictimFile(move(victimFile));
	undoStackRepository.push_back(move(action));
	redoStackRepository.clear();
}


void Service::serviceUpdateVictimFile(const string& name, const string& placeOfOrigin, int age, const string& photograph){
	bool execute = false;
	shared_ptr<VictimFile> newVictimFile = make_shared<VictimFile>(name, placeOfOrigin, age, photograph );
	this->validator.validateVictimFile(newVictimFile);
	shared_ptr<VictimFile> oldVictimFile =  this->repository->updateVictimFile(newVictimFile);
	if (this->myList->search(name) != NULL) {
		this->myList->updateVictimFile(newVictimFile);
		execute = true;
	}
	unique_ptr<Action> action2 = make_unique<ActionUpdate>(this->repository, this->myList, oldVictimFile, newVictimFile, execute);
	undoStackRepository.push_back(move(action2));
	redoStackRepository.clear();
}


void Service::serviceDeleteVictimFile(const string& name){
	bool execute = false;
	shared_ptr<VictimFile> victimFile = this->repository->search(name);
	this->repository->deleteVictimFile(victimFile);
	if (this->myList->search(name) != NULL) {
		this->myList->deleteVictimFile(victimFile);
		execute = true;
	}
	unique_ptr<Action> action2 = make_unique<ActionRemove>(this->repository, this->myList, victimFile, execute);
	undoStackRepository.push_back(move(action2));
	redoStackRepository.clear();
}


vector<shared_ptr<VictimFile>> Service::serviceGetVictimFiles(){
	return this->repository->repositoryGetVictimFiles();
}


void Service::undoRepository(){
	if (undoStackRepository.size() == 0)
		throw exception("No more actions to undo!");
	unique_ptr<Action> action = move(undoStackRepository.back());
	action->executeUndo();
	undoStackRepository.pop_back();
	redoStackRepository.push_back(move(action));
}


void Service::redoRepository(){
	if (redoStackRepository.size() == 0)
		throw exception("No more actions to redo!");
	unique_ptr<Action> action = move(redoStackRepository.back());
	action->executeRedo();
	redoStackRepository.pop_back();
	undoStackRepository.push_back(move(action));
}




shared_ptr<VictimFile> Service::next(){
	vector<shared_ptr<VictimFile>> victimFiles = this->repository->repositoryGetVictimFiles();
	if (victimFiles.size() == 0)
		return NULL;
	this->first++;
	if (this->currentPosition + 1 == victimFiles.size() || this->currentPosition >= victimFiles.size())
	{
		unique_ptr<ActionMylist> action = make_unique<ActionNext>("next");
		undoStackMylist.push_back(move(action));
		this->currentPosition = 0;
		return victimFiles[victimFiles.size() - 1];
	}
	unique_ptr<ActionMylist> action = make_unique<ActionNext>("next");
	undoStackMylist.push_back(move(action));
	return victimFiles[this->currentPosition++];
}


shared_ptr<VictimFile> Service::previous()
{
	vector<shared_ptr<VictimFile>> victimFiles = this->repository->repositoryGetVictimFiles();
	if (victimFiles.size() == 0)
		return NULL;
	this->first--;
	if (this->first == 0) {
		this->currentPosition--;
		return shared_ptr<VictimFile>(make_shared<VictimFile>(VictimFile{}));
	}
	this->currentPosition -= 2;
	if (this->currentPosition == -1 || this->currentPosition >= victimFiles.size()) {
		this->currentPosition++;
		return victimFiles[victimFiles.size() - 1];
	}
	return victimFiles[this->currentPosition++];
}


void Service::save(const string& name){
	shared_ptr<VictimFile> victimFile = this->repository->search(name);
	if (victimFile == NULL)
		throw exception("The victim file doesn't exist!");
	this->myList->addVictimFile(victimFile);
	unique_ptr<ActionMylist> action = make_unique<ActionSave>(this->repository, this->myList, victimFile->getName(), "save");
	undoStackMylist.push_back(move(action));
	redoStackMylist.clear();
	for (int i = 0; i < undoStackRepository.size(); i++) {
		if (undoStackRepository[i]->getVictim() == *victimFile) {
			undoStackRepository[i]->setExecute(true);
		}
	}
}


vector<shared_ptr<VictimFile>> Service::getMyList() const{
	return this->myList->repositoryGetVictimFiles();
}


vector<shared_ptr<VictimFile>> Service::getFiltered(const string& placeOfOrigin, int age) const{
	vector<shared_ptr<VictimFile>> allVictimFiles = this->repository->repositoryGetVictimFiles();
	vector<shared_ptr<VictimFile>> filteredVictimFiles(allVictimFiles.size());
	auto iterator = copy_if(allVictimFiles.begin(), allVictimFiles.end(), filteredVictimFiles.begin(), [placeOfOrigin, age](shared_ptr<VictimFile> victimFile) { return (victimFile->getAge() < age && (placeOfOrigin == "" || victimFile->getPlaceOfOrigin() == placeOfOrigin)); });
	filteredVictimFiles.resize(distance(filteredVictimFiles.begin(), iterator));
	return filteredVictimFiles;
}


string Service::getFileType(){
	if (dynamic_cast<InMemoryRepository*>(this->myList))
		return "";
	return dynamic_cast<FileRepository*>(this->myList)->getType();
}


string Service::getFileLocation(){
	return dynamic_cast<FileRepository*>(this->myList)->getFileLocation();
}


string Service::undoMylist()
{
	if (undoStackMylist.size() == 0)
		throw exception("No more actions to undo!");
	unique_ptr<ActionMylist> action = move(undoStackMylist.back());
	undoStackMylist.pop_back();
	if (action->getAction() == "save") {
		ActionSave* actionSave = dynamic_cast<ActionSave*>(action.get());
		if (actionSave->stillExists()) {
			actionSave->executeUndo();

			for (int i = 0; i < undoStackRepository.size(); i++) {
				if (undoStackRepository[i]->getVictim() == actionSave->getVictim()) {
					undoStackRepository[i]->setExecute(false);
				}
			}
			for (int i = 0; i < redoStackRepository.size(); i++) {
				if (redoStackRepository[i]->getVictim() == actionSave->getVictim()) {
					redoStackRepository[i]->setExecute(false);
				}
			}

			redoStackMylist.push_back(move(action));
		}
		else {
			redoStackMylist.push_back(move(action));
			throw exception("The victim doesn't exist anymore!");
		}
		return "save";
	}
	redoStackMylist.push_back(move(action));
	return "next";
}


string Service::redoMylist()
{
	if (redoStackMylist.size() == 0)
		throw exception("No more actions to redo!");
	unique_ptr<ActionMylist> action = move(redoStackMylist.back());
	redoStackMylist.pop_back();
	if (action->getAction() == "save") {
		ActionSave* actionSave = dynamic_cast<ActionSave*>(action.get());
		if (actionSave->stillExists()) {
			actionSave->executeRedo();

			for (int i = 0; i < undoStackRepository.size(); i++) {
				if (undoStackRepository[i]->getVictim() == actionSave->getVictim()) {
					undoStackRepository[i]->setExecute(true);
				}
			}
			for (int i = 0; i < redoStackRepository.size(); i++) {
				if (redoStackRepository[i]->getVictim() == actionSave->getVictim()) {
					redoStackRepository[i]->setExecute(true);
				}
			}

			undoStackMylist.push_back(move(action));
		}
		else {
			redoStackMylist.push_back(move(action));
			throw exception("The victim doesn't exist anymore!");
		}
		return "save";
	}
	undoStackMylist.push_back(move(action));
	return "next";
}
