#pragma once
#include <qstyleditemdelegate.h>
#include <QPixmap>
#include <QPainter>

class PictureDelegate : public QStyledItemDelegate
{
public:
	PictureDelegate(QWidget* parent = 0): QStyledItemDelegate{ parent } {}
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	~PictureDelegate() {}
};

