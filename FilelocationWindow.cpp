#include "FilelocationWindow.h"

FilelocationWindow::FilelocationWindow(string& location, QWidget *parent)
	: QWidget(parent), location{location}
{
	ui.setupUi(this);
	QObject::connect(this->ui.doneButton, &QPushButton::clicked, this, &FilelocationWindow::getLocation);
	this->show();
	QEventLoop loop;
	connect(this->ui.doneButton, SIGNAL(clicked()), &loop, SLOT(quit()));
	loop.exec();
}

void FilelocationWindow::getLocation()
{
	this->location = this->ui.location->text().toStdString();
	this->destroy();
}