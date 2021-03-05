#include "Lab12.h"

using namespace std;

Lab12::Lab12(Service& service, QWidget *parent)
	: QMainWindow(parent), service{service}
{
	this->tableModel = new VictimsTableModel(service.getPointerMylist());
	ui.setupUi(this);
	this->connectSignalsAndSlots();
	this->populateList();
	this->chart = new QChart{};
	this->chartView = new QChartView{ chart };
	this->setBarChart();
	this->ui.verticalLayout_5->addWidget(chartView);
	this->populateMyList();
}

void Lab12::connectSignalsAndSlots()
{
	QObject::connect(this->ui.listOfVictims, &QListWidget::itemSelectionChanged, this, &Lab12::changeCurrentItem);
	QObject::connect(this->ui.removeButton, &QPushButton::clicked, this, &Lab12::removeVictim);
	QObject::connect(this->ui.addButton, &QPushButton::clicked, this, &Lab12::addWindow);
	QObject::connect(this->ui.updateButton, &QPushButton::clicked, this, &Lab12::updateWindow);
	QObject::connect(this->ui.nextButton, &QPushButton::clicked, this, &Lab12::nextVictim);
	QObject::connect(this->ui.saveButton, &QPushButton::clicked, this, &Lab12::saveVictim);
	QObject::connect(this->ui.filterButton, &QPushButton::clicked, this, &Lab12::filterVictims);
	QObject::connect(this->ui.filteredVictims, &QListWidget::itemSelectionChanged, this, &Lab12::changeCurrentItemFiltered);
	QObject::connect(this->ui.openButton, &QPushButton::clicked, this, &Lab12::open);
	QObject::connect(this->ui.undoRepoButton, &QPushButton::clicked, this, &Lab12::undoRepository);
	QObject::connect(this->ui.redoRepoButton, &QPushButton::clicked, this, &Lab12::redoRepository);
	QObject::connect(this->ui.undoMylistButton, &QPushButton::clicked, this, &Lab12::undoMylist);
	QObject::connect(this->ui.redoMylistButton, &QPushButton::clicked, this, &Lab12::redoMylist);
	QObject::connect(this->ui.mylistButton, &QPushButton::clicked, this, &Lab12::openMylistWidget);
	QObject::connect(this, &Lab12::updateModel, this->tableModel, &VictimsTableModel::update);

}

void Lab12::populateList()
{
	if (this->ui.listOfVictims->count() > 0)
		this->ui.listOfVictims->clear();
	vector <shared_ptr<VictimFile>> victimFiles = this->service.serviceGetVictimFiles();
	for (shared_ptr<VictimFile> victimFile : victimFiles)
	{
		QString name = QString::fromStdString(victimFile->getName() + " - " + victimFile->getPlaceOfOrigin());
		QListWidgetItem* item = new QListWidgetItem{ name };
		this->ui.listOfVictims->addItem(item);
	}
}

int Lab12::getSelectedIndex() const
{
	if (this->ui.listOfVictims->count() == 0)
		return -1;

	QModelIndexList indexes = this->ui.listOfVictims->selectionModel()->selectedIndexes();
	if (indexes.size() == 0)
	{
		this->ui.name->clear();
		this->ui.placeOfOrigin->clear();
		this->ui.age->clear();
		this->ui.photograph->clear();
		this->ui.actualPhotograph->clear();
		return -1;
	}

	int actualIndex = indexes.at(0).row();
	return actualIndex;
}

void Lab12::changeCurrentItem()
{
	int index = this->getSelectedIndex();
	if (index == -1)
		return;
	shared_ptr<VictimFile> victim = this->service.serviceGetVictimFiles()[index];
	this->ui.name->setText(QString::fromStdString(victim->getName()));
	this->ui.placeOfOrigin->setText(QString::fromStdString(victim->getPlaceOfOrigin()));
	this->ui.age->setText(QString::fromStdString(to_string(victim->getAge())));
	this->ui.photograph->setText(QString::fromStdString(victim->getPhotograph()));
	QPixmap* image = new QPixmap(QString::fromStdString(victim->getPhotograph()));
	this->ui.actualPhotograph->setPixmap(*image);
}

void Lab12::removeVictim()
{
	int index = this->getSelectedIndex();
	if (index == -1)
	{
		
		return;
	}
	QString name = this->ui.name->text();
	this->service.serviceDeleteVictimFile(name.toStdString());
	this->populateList();
	this->populateMyList();
	this->setBarChart();
}

void Lab12::addWindow()
{
	QWidget* widget = new QWidget{};
	QGridLayout* layout = new QGridLayout{ widget };

	//name
	QLineEdit* name = new QLineEdit{};
	QLabel* label = new QLabel("&Name");
	label->setFont(font);
	label->setBuddy(name);
	name->setFont(font);
	layout->addWidget(label, 0, 0);
	layout->addWidget(name, 0, 1, 1, 2);
	//place of origin
	QLineEdit* placeOfOrigin = new QLineEdit{};
	QLabel* label1 = new QLabel("&Place of Origin");
	label1->setFont(font);
	label1->setBuddy(placeOfOrigin);
	placeOfOrigin->setFont(font);
	layout->addWidget(label1, 1, 0);
	layout->addWidget(placeOfOrigin, 1, 1, 1, 2);
	//age
	QLineEdit* age = new QLineEdit{};
	QLabel* label2 = new QLabel("&Age");
	label2->setFont(font);
	label2->setBuddy(age);
	age->setFont(font);
	layout->addWidget(label2, 2, 0);
	layout->addWidget(age, 2, 1, 1, 2);
	//photograph
	QLineEdit* photograph = new QLineEdit{};
	QLabel* label3 = new QLabel("&Photograph");
	label3->setFont(font);
	label3->setBuddy(photograph);
	photograph->setFont(font);
	layout->addWidget(label3, 3, 0);
	layout->addWidget(photograph, 3, 1, 1, 2);
	//add button
	QPushButton* addButton = new QPushButton{ "Add" };
	addButton->setFont(font);
	layout->addWidget(addButton, 4, 2);

	QObject::connect(addButton, &QPushButton::clicked, this, [this, widget, name, placeOfOrigin, age, photograph]() {
		try
		{
			this->service.serviceAddVictimFile(name->text().toStdString(), placeOfOrigin->text().toStdString(), stoi(age->text().toStdString()), photograph->text().toStdString());
		}
		catch (exception & error)
		{
			QMessageBox::critical(this, "Error", error.what());
		}
		this->populateList();
		this->populateMyList();
		this->setBarChart();
		widget->close();
		});
	widget->show();
}

void Lab12::updateWindow()
{
	QWidget* widget = new QWidget{};
	QGridLayout* layout = new QGridLayout{ widget };

	//name
	QLineEdit* name = new QLineEdit{};
	QLabel* label = new QLabel("&Name");
	label->setFont(font);
	label->setBuddy(name);
	name->setFont(font);
	name->setText(this->ui.name->text());
	layout->addWidget(label, 0, 0);
	layout->addWidget(name, 0, 1, 1, 2);
	//place of origin
	QLineEdit* placeOfOrigin = new QLineEdit{};
	QLabel* label1 = new QLabel("&Place of Origin");
	label1->setFont(font);
	label1->setBuddy(placeOfOrigin);
	placeOfOrigin->setFont(font);
	placeOfOrigin->setText(this->ui.placeOfOrigin->text());
	layout->addWidget(label1, 1, 0);
	layout->addWidget(placeOfOrigin, 1, 1, 1, 2);
	//age
	QLineEdit* age = new QLineEdit{};
	QLabel* label2 = new QLabel("&Age");
	label2->setFont(font);
	label2->setBuddy(age);
	age->setFont(font);
	age->setText(this->ui.age->text());
	layout->addWidget(label2, 2, 0);
	layout->addWidget(age, 2, 1, 1, 2);
	//photograph
	QLineEdit* photograph = new QLineEdit{};
	QLabel* label3 = new QLabel("&Photograph");
	label3->setFont(font);
	label3->setBuddy(photograph);
	photograph->setFont(font);
	photograph->setText(this->ui.photograph->text());
	layout->addWidget(label3, 3, 0);
	layout->addWidget(photograph, 3, 1, 1, 2);
	//add button
	QPushButton* addButton = new QPushButton{ "Update" };
	addButton->setFont(font);
	layout->addWidget(addButton, 4, 2);

	QObject::connect(addButton, &QPushButton::clicked, this, [this, widget, name, placeOfOrigin, age, photograph]() {
		try
		{
			this->service.serviceUpdateVictimFile(name->text().toStdString(), placeOfOrigin->text().toStdString(), stoi(age->text().toStdString()), photograph->text().toStdString());
		}
		catch (exception & error)
		{
			QMessageBox::critical(this, "Error", error.what());
		}
		this->populateList();
		this->populateMyList();
		this->setBarChart();
		widget->close();
		});

	widget->show();
}

void Lab12::setBarChart()
{
	chart->removeAllSeries();
	chart->removeAxis(this->axisX);
	chart->removeAxis(this->axisY);
	//series
	QBarSet* set = new QBarSet{ "Victims"};
	vector<int> values = this->getValues();
	for (auto value : values)
		*set << value;
	this->series = new QBarSeries{};
	series->append(set);
	//chart
	chart->addSeries(series);
	chart->setAnimationOptions(QChart::SeriesAnimations);
	//x axix
	vector<string> stringCategories = this->getCategories();
	vector<QString> categories{};
	this->axisX = new QBarCategoryAxis{};
	for (auto category : stringCategories)
		this->axisX->append(QString::fromStdString(category));
	chart->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);
	//y axix
	this->axisY = new QValueAxis{};
	axisY->setRange(0, 5);
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);
	//chart view
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setMinimumSize(QSize(800, 500));
}

vector<int> Lab12::getValues()
{
	vector<int> values{};
	int value;
	vector <shared_ptr<VictimFile>> victims = this->service.serviceGetVictimFiles();
	vector<string> categories = getCategories();
	for (string category : categories)
	{
		value = 0;
		for (auto victim : victims)
			if (victim->getPlaceOfOrigin() == category)
				value++;
		values.push_back(value);
	}
	return values;
}

vector<string> Lab12::getCategories()
{
	vector<string> categories{};
	vector <shared_ptr<VictimFile>> victims = this->service.serviceGetVictimFiles();
	for (auto victim : victims)
		if (find_if(categories.begin(), categories.end(), [victim](string category) {return victim->getPlaceOfOrigin() == category; }) == categories.end())
			categories.push_back(victim->getPlaceOfOrigin());
	return categories;
}

void Lab12::undoRepository()
{
	try
	{
		this->service.undoRepository();
		this->populateList();
		this->populateMyList();
	}
	catch (exception & error)
	{
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::redoRepository()
{
	try
	{
	this->service.redoRepository();
	this->populateList();
	this->populateMyList();
	}
	catch (exception & error)
	{
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::populateMyList()
{
	this->ui.tableWidget->clearContents();
	vector<shared_ptr<VictimFile>> victims = this->service.getMyList();
	this->ui.tableWidget->setRowCount(victims.size());
	for (int i = 0; i < victims.size(); i++)
	{
		//name
		QTableWidgetItem* item0 = new QTableWidgetItem;
		item0->setTextAlignment(Qt::AlignVCenter);
		item0->setData(Qt::DisplayRole, QString::fromStdString(victims[i]->getName()));
		this->ui.tableWidget->setItem(i, 0, item0);
		//age
		QTableWidgetItem* item1 = new QTableWidgetItem;
		item1->setTextAlignment(Qt::AlignVCenter);
		item1->setData(Qt::DisplayRole, QString::fromStdString(to_string(victims[i]->getAge())));
		this->ui.tableWidget->setItem(i, 1, item1);
		//place of origin
		QTableWidgetItem* item2 = new QTableWidgetItem;
		item2->setTextAlignment(Qt::AlignVCenter);
		item2->setData(Qt::DisplayRole, QString::fromStdString(victims[i]->getPlaceOfOrigin()));
		this->ui.tableWidget->setItem(i, 2, item2);
		//photograph
		QTableWidgetItem* item3 = new QTableWidgetItem;
		item3->setTextAlignment(Qt::AlignVCenter);
		item3->setData(Qt::DisplayRole, QString::fromStdString(victims[i]->getPhotograph()));
		this->ui.tableWidget->setItem(i, 3, item3);
	}
	emit updateModel();
	for (int i = 0; i < tableViews.size(); i++) {
		tableViews[i]->resizeColumnsToContents();
		tableViews[i]->resizeRowsToContents();
	}
}

void Lab12::nextVictim()
{
	try
	{
		int age = stoi(this->ui.age3->text().toStdString());
		this->filterVictims();
	}
	catch(exception& error){}
	if(this->service.serviceGetVictimFiles().size() == 0)
		QMessageBox::critical(this, "Error", "There are no victims");
	else {
		shared_ptr<VictimFile> victim = this->service.next();
		this->ui.name2->setText(QString::fromStdString(victim->getName()));
		this->ui.placeOfOrigin2->setText(QString::fromStdString(victim->getPlaceOfOrigin()));
		this->ui.age2->setText(QString::fromStdString(to_string(victim->getAge())));
		this->ui.photograph2->setText(QString::fromStdString(victim->getPhotograph()));
		QPixmap* image = new QPixmap(QString::fromStdString(victim->getPhotograph()));
		this->ui.actualPhotograph2->setPixmap(*image);
	}
}

void Lab12::saveVictim()
{
	try
	{
		this->service.save(this->ui.name2->text().toStdString());
		this->populateMyList();
	}
	catch (exception & error)
	{
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::filterVictims()
{
	string placeOfOrigin = this->ui.placeOfOrigin3->text().toStdString();
	int age;
	try {
		age = stoi(this->ui.age3->text().toStdString());
		vector<shared_ptr<VictimFile>> victims = this->service.getFiltered(placeOfOrigin, age);
		if (this->ui.filteredVictims->count() > 0)
			this->ui.filteredVictims->clear();
		for (shared_ptr<VictimFile> victimFile : victims)
		{
			QString name = QString::fromStdString(victimFile->getName() + " - " + victimFile->getPlaceOfOrigin());
			QListWidgetItem* item = new QListWidgetItem{ name };
			this->ui.filteredVictims->addItem(item);
		}
	}
	catch (exception & error) {
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::changeCurrentItemFiltered()
{
	QModelIndexList indexes = this->ui.filteredVictims->selectionModel()->selectedIndexes();
	if (indexes.size() == 0)
		return;
	int index = indexes.at(0).row();
	if (index == -1)
		return;
	string placeOfOrigin = this->ui.placeOfOrigin3->text().toStdString();
	int age;
	try {
		age = stoi(this->ui.age3->text().toStdString());
		shared_ptr<VictimFile> victim = this->service.getFiltered(placeOfOrigin, age)[index];
		this->ui.name2->setText(QString::fromStdString(victim->getName()));
		this->ui.placeOfOrigin2->setText(QString::fromStdString(victim->getPlaceOfOrigin()));
		this->ui.age2->setText(QString::fromStdString(to_string(victim->getAge())));
		this->ui.photograph2->setText(QString::fromStdString(victim->getPhotograph()));
		QPixmap* image = new QPixmap(QString::fromStdString(victim->getPhotograph()));
		this->ui.actualPhotograph2->setPixmap(*image);
	}
	catch (exception & error) {
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::open()
{
	if (this->service.getFileType() == "txt")
	{
		ShellExecuteA(NULL, NULL, "notepad", this->service.getFileLocation().c_str(), NULL, SW_SHOWNORMAL);
	}
	else
		if (this->service.getFileType() == "html")
			ShellExecuteA(NULL, NULL, "chrome.exe", this->service.getFileLocation().c_str(), NULL, SW_SHOWNORMAL);
}

void Lab12::undoMylist()
{
	try
	{
		string actionName = this->service.undoMylist();
		this->populateList();
		this->populateMyList();
		if (actionName == "next") {
			shared_ptr<VictimFile> victim = this->service.previous();
			if (victim == nullptr) {
				this->ui.name2->clear();
				this->ui.placeOfOrigin2->clear();
				this->ui.age2->clear();
				this->ui.photograph2->clear();
				this->ui.actualPhotograph2->clear();
			}
			else {
				this->ui.name2->setText(QString::fromStdString(victim->getName()));
				this->ui.placeOfOrigin2->setText(QString::fromStdString(victim->getPlaceOfOrigin()));
				this->ui.age2->setText(QString::fromStdString(to_string(victim->getAge())));
				this->ui.photograph2->setText(QString::fromStdString(victim->getPhotograph()));
				QPixmap* image = new QPixmap(QString::fromStdString(victim->getPhotograph()));
				this->ui.actualPhotograph2->setPixmap(*image);
			}
		}
	}
	catch (exception & error)
	{
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::redoMylist()
{
	try
	{
		string actionName = this->service.redoMylist();
		this->populateList();
		this->populateMyList();
		if (actionName == "next") {
			shared_ptr<VictimFile> victim = this->service.next();
			this->ui.name2->setText(QString::fromStdString(victim->getName()));
			this->ui.placeOfOrigin2->setText(QString::fromStdString(victim->getPlaceOfOrigin()));
			this->ui.age2->setText(QString::fromStdString(to_string(victim->getAge())));
			this->ui.photograph2->setText(QString::fromStdString(victim->getPhotograph()));
			QPixmap* image = new QPixmap(QString::fromStdString(victim->getPhotograph()));
			this->ui.actualPhotograph2->setPixmap(*image);
		}
	}
	catch (exception & error)
	{
		QMessageBox::critical(this, "Error", error.what());
	}
}

void Lab12::openMylistWidget()
{
	QWidget* widget = new QWidget{};
	QHBoxLayout* layout = new QHBoxLayout{ widget };

	tableViews.push_back(new QTableView{ widget });
	tableViews[tableViews.size()-1] = new QTableView{ widget };
	tableViews[tableViews.size() - 1]->setModel(this->tableModel);
	tableViews[tableViews.size() - 1]->setItemDelegate(new PictureDelegate{});
	tableViews[tableViews.size() - 1]->resizeColumnsToContents();
	tableViews[tableViews.size() - 1]->resizeRowsToContents();

	layout->addWidget(tableViews[tableViews.size() - 1]);
	widget->setMinimumSize(700, 600);
	widget->show();
}
