#ifndef COMBOBOXITEMDELEGATE_H
#define COMBOBOXITEMDELEGATE_H
 
#include <QStyledItemDelegate>
 
class ComboBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
 
public:
    ComboBoxItemDelegate(QObject *parent = 0, QString query = "");
    ~ComboBoxItemDelegate();
 QString query;
    virtual QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
    virtual void setEditorData ( QWidget *editor, const QModelIndex &index ) const;
    virtual void setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
 
};
 
#endif // COMBOBOXITEMDELEGATE_H
