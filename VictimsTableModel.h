#pragma once
#include <qabstractitemmodel.h>
#include "Repository.h"
#include <QFont>
#include <QBrush>

class VictimsTableModel : public QAbstractTableModel
{
private:
	Repository* mylist;

public:
	VictimsTableModel(Repository* mylist, QObject* parent = NULL) : QAbstractTableModel{ parent }, mylist{ mylist } {}
	int rowCount(const QModelIndex& parent = QModelIndex{}) const override;
	int columnCount(const QModelIndex& parent = QModelIndex{}) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	~VictimsTableModel() {}

public slots:
	void update() { this->beginResetModel(); this->endResetModel(); }
};

