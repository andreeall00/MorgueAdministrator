#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab12.h"
#include "Service.h"
#include "VictimsTableModel.h"
#include "PictureDelegate.h"
#include <QMessageBox>
#include <QtCharts>
#include <Windows.h>

class Lab12 : public QMainWindow
{
	Q_OBJECT

public:
	Lab12(Service& service, QWidget *parent = Q_NULLPTR);
	~Lab12() {}

private:
	Service& service;
	Ui::Lab12Class ui;
	QFont font{ "Verdana", 12 };
	QBarSeries* series;
	QBarCategoryAxis* axisX;
	QValueAxis* axisY;
	QChart* chart;
	QChartView* chartView;
	VictimsTableModel* tableModel;
	vector<QTableView*> tableViews;

	void connectSignalsAndSlots();
	void populateList();
	int getSelectedIndex()const;
	void changeCurrentItem();
	void removeVictim();
	void addWindow();
	void updateWindow();
	void setBarChart();
	vector<int> getValues();
	vector<string> getCategories();
	void undoRepository();
	void redoRepository();

	void populateMyList();
	void nextVictim();
	void saveVictim();
	void filterVictims();
	void changeCurrentItemFiltered();
	void open();
	void undoMylist();
	void redoMylist();
	void openMylistWidget();

signals:
	void updateModel();
};
