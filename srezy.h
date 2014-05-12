#ifndef SREZY_H
#define SREZY_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "dals/dal_prepodcontrol.h"
#include "add_edit_forms/addoredit_srez.h"
#include "add_edit_forms/addoredit_srez_bally.h"

//extern DAL_main  *dal_main;
namespace Ui {
class Srezy;
}

class Srezy : public QDialog
{
    Q_OBJECT

public:
    explicit Srezy(QWidget *parent = 0);
    ~Srezy();
    bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_comboBox_gruppa_activated(int index);

    void on_pushButton_addBalls_clicked();

    void on_actionSetBally_triggered();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_clicked(const QModelIndex &index);

    void on_comboBox_spec_activated(const QString &arg1);

private:
    Ui::Srezy *ui;
    QSqlTableModel *disciplineModel;
    Dal_studentsControl *dal_studentsControl;
    int gruppa;
    int spec;
public slots:
   void refreshData();
};

#endif // SREZY_H
