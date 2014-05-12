#include "readonlydelegate.h"
#include <QLineEdit>
#include <QModelIndex>

readOnlyDelegate::readOnlyDelegate(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget *readOnlyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *edit = new QLineEdit(parent);
    edit->setReadOnly(true);
    return edit;
}

void readOnlyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(index.data().toString());
}

void readOnlyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
