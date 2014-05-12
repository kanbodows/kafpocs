#ifndef TRANSACTIONDATEDELEGATEWITHOUSEC_H
#define TRANSACTIONDATEDELEGATEWITHOUSEC_H

#include <QItemDelegate>

class transactiondatedelegatewithousec : public QItemDelegate
{
    Q_OBJECT
public:
    transactiondatedelegatewithousec(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
signals:

public slots:

};

#endif // TRANSACTIONDATEDELEGATE_H
