#include "datedelegate.h"
#include <QDateEdit>
#include <QApplication>
#include <QPainter>
dateDelegate::dateDelegate(QObject *parent)
{
}
QWidget *dateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDateEdit *de = new QDateEdit(parent);
    de->setCalendarPopup(true);
    de->setDisplayFormat("dd.MM.yyyy");
    return de;
}
void dateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    de->setDate(index.model()->data(index, Qt::EditRole).toDate());
}
void dateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDateEdit *de = static_cast<QDateEdit*>(editor);
    model->setData(index,de->date().toString("yyyy-MM-dd"),Qt::EditRole);
}
void dateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
void dateDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 display_option(option);
    display_option.text = QDate::fromString(index.data().toString(), Qt::ISODate).toString("dd MMMM yyyy");
    QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &display_option, painter);
}
