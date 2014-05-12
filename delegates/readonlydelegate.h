#ifndef READONLYDELEGATE_H
#define READONLYDELEGATE_H

#include <QObject>
#include <QItemDelegate>

class readOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit readOnlyDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
signals:

public slots:

};

#endif // READONLYDELEGATE_H
