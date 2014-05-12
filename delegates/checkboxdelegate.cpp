#include "checkboxdelegate.h"
#include "QApplication"
#include "QCheckBox"
#include <QStyle>
checkboxdelegate::checkboxdelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
QWidget *checkboxdelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *de = new QCheckBox(parent);
    return de;
}
void checkboxdelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QCheckBox *de = static_cast<QCheckBox*>(editor);
    de->setChecked(index.model()->data(index, Qt::EditRole).toBool());
}
void checkboxdelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *de = static_cast<QCheckBox*>(editor);
    model->setData(index,(int)de->isChecked(),Qt::EditRole);
}
void checkboxdelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
void checkboxdelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool value = index.data(Qt::DisplayRole).toBool();
    QStyle *style = QApplication::style();
    QStyleOptionButton btn;
    QCheckBox *rad=new QCheckBox;
    rad->setChecked(true);
    btn.initFrom(rad);
    btn.state = value ? style->State_On : style->State_Off;
    btn.rect=option.rect;
    QApplication::style()->drawControl(QStyle::CE_CheckBox,&btn,painter);
}
