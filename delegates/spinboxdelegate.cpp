#include "spinboxdelegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSpinBox *de = new QSpinBox(parent);
    de->setMinimum(0);
    de->setMaximum(100);
    de->setValue(0);
    de->setSingleStep(1);
    return de;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox *de = static_cast<QSpinBox*>(editor);
    de->setValue(index.model()->data(index, Qt::EditRole).toInt());
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox *de = static_cast<QSpinBox*>(editor);
    model->setData(index, de->value(), Qt::EditRole);
}
