#include "PictureDelegate.h"

void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() != 3) {
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}
	QPixmap pixMap(index.data().toString());
	pixMap.scaled(QSize(100, 120), Qt::KeepAspectRatio);
	painter->drawPixmap(option.rect, pixMap);
}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 3)
		return QSize(100, 120);
	return QStyledItemDelegate::sizeHint(option, index);
}
