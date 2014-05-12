#ifndef GROUPS_H
#define GROUPS_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class groups;
}

class groups : public QDialog
{
    Q_OBJECT

public:
    explicit groups(QWidget *parent = 0);
    ~groups();
    bool static isOpen;
    bool checkRelation(QString discipline_id);
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_kutator_activated(int index);

    void on_pushButton_search_clicked();

    void on_pushButton_edit_clicked();

private:
    Ui::groups *ui;
    QSqlRelationalTableModel *groupsModel;
    QSqlTableModel *kuratorModel;
    int count;
    int kurator_id;
    QString naideno;
public slots:
    void formingSearch();
};

#endif // GROUPS_H
