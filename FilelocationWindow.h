#pragma once

#include <QWidget>
#include "ui_FilelocationWindow.h"
#include <string>
using namespace std;

class FilelocationWindow : public QWidget
{
	Q_OBJECT

public:
	FilelocationWindow(string& location, QWidget *parent = Q_NULLPTR);
	~FilelocationWindow() {}

private:
	string& location;
	void getLocation();

	Ui::FilelocationWindow ui;
};
