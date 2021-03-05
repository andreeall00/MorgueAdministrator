#include "VictimsTableModel.h"

int VictimsTableModel::rowCount(const QModelIndex& parent) const{
	if (parent.isValid())
		return 0;
	return this->mylist->repositoryGetVictimFiles().size();
}

int VictimsTableModel::columnCount(const QModelIndex& parent) const{
	return 4;
}

QVariant VictimsTableModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int column = index.column();
	vector<shared_ptr<VictimFile>> victims = mylist->repositoryGetVictimFiles();
	if (victims.size() > index.row()) {
		VictimFile victim = *victims[row];
		if (role == Qt::DisplayRole) {
			switch (column)
			{
			case 0:
				return QString::fromStdString(victim.getName());
			case 1:
				return QString::number(victim.getAge());
			case 2:
				return QString::fromStdString(victim.getPlaceOfOrigin());
			case 3:
				return QString::fromStdString(victim.getPhotograph());
			default:
				break;
			}
		}
	}
	if (role == Qt::FontRole){
		QFont font{ "Verdana", 12 };
		font.setBold(false);
		font.setItalic(false);
		return font;
	}
	return QVariant();
}

QVariant VictimsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section)
			{
			case 0:
				return QString{ "Name" };
			case 1:
				return QString{ "Age" };
			case 2:
				return QString{ "Place of Origin" };
			case 3:
				return QString{ "Photograph" };
			default:
				break;
			}
		}
	}
	if (role == Qt::FontRole) {
		QFont font{ "Verdana", 13 };
		font.setBold(true);
		font.setItalic(false);
		return font;
	}
	return QVariant();
}
