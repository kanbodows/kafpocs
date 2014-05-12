#include "transactiondatedelegate.h"
#include <QDateTimeEdit>
#include <QApplication>
#include <QPainter>

transactionDateDelegate::transactionDateDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *transactionDateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateTimeEdit *dateTimeDelegate = new QDateTimeEdit(parent);
    dateTimeDelegate->setCalendarPopup(true);
    dateTimeDelegate->setDisplayFormat("dd MMMM yyyy hh:mm:ss");
    return dateTimeDelegate;
}

void transactionDateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateTimeEdit *dateTimeDelegate = static_cast<QDateTimeEdit*>(editor);
    dateTimeDelegate->setDate(index.model()->data(index, Qt::EditRole).toDate());
}

void transactionDateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateTimeEdit *dateTimeDelegate = static_cast<QDateTimeEdit*>(editor);
    model->setData(index,dateTimeDelegate->dateTime().toString("yyyy-MM-dd hh:mm:ss"),Qt::EditRole);
}

void transactionDateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void transactionDateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 display_option(option);
    display_option.text = QDateTime::fromString(index.data().toString(), "yyyy-MM-ddThh:mm:ss").toString("dd MMMM yyyy hh:mm:ss");
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &display_option, painter);
}
