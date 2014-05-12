#include "doublespinboxdelegate.h"
#include <QDoubleSpinBox>

DoubleSpinBoxDelegate::DoubleSpinBoxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *DoubleSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QDoubleSpinBox *de = new QDoubleSpinBox(parent);
    de->setDecimals(2);
    de->setMinimum(0.05);
    de->setValue(0.05);
    de->setMaximum(100000.00);
    return de;
}

void DoubleSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox *de = static_cast<QDoubleSpinBox*>(editor);
    de->setValue(index.model()->data(index, Qt::EditRole).toDouble());
}

void DoubleSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *de = static_cast<QDoubleSpinBox*>(editor);
    model->setData(index, de->value(), Qt::EditRole);
}
