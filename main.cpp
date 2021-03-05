#include "Lab12.h"
#include "FilelocationWindow.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ifstream configurations("configurations.txt");
	string filelocation;
	//repository
	Repository* repository;
	configurations >> filelocation;
	if (filelocation == "repository=memory")
		repository = new InMemoryRepository{};
	else
	{
		string location;
		FilelocationWindow chooseFile{location};
		repository = new FileRepository{ location };
	}
	
	//mylist
	Repository* mylist;
	configurations >> filelocation;
	if (filelocation == "mylist=memory")
		mylist = new InMemoryRepository{};
	else
	{
		string location;
		FilelocationWindow chooseFile{ location };
		if (location.compare(location.size()-4, 4, ".csv") == 0 || location.compare(location.size() - 4, 4, ".txt") == 0)
			mylist = new FileRepository{ location };
		else 
			if (location.compare(location.size() - 5, 5, ".html") == 0)
			mylist = new HTMLRepository{ location };
	}
	configurations.close();
	Validator validator;
	Service service{ repository, mylist, validator };
	Lab12 gui{service};
	gui.show();
	return a.exec();
}
