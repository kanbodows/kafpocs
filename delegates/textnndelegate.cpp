#include "textnndelegate.h"
#include "QLineEdit"
#include "QMessageBox"
#include <QDebug>
textNnDelegate::textNnDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
QWidget *textNnDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *editor = new QLineEdit(parent);
    return editor;
}
void textNnDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    edit->setText(index.model()->data(index, Qt::EditRole).toString());
}
void textNnDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *edit = static_cast<QLineEdit*>(editor);
    if (edit->text().isEmpty())
    {
        QMessageBox::warning(NULL,tr("Ошибка!"),tr("Поле не может быть пустым!"));
        model->setData(index,QVariant(), Qt::EditRole);
    }
    else
         model->setData(index,edit->text(),Qt::EditRole);
}
void textNnDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
