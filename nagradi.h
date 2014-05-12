#ifndef NAGRADI_H
#define NAGRADI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QString>
#include "add_edit_forms/addoredit_nagradi.h"

//extern DAL_main  *dal_main;
namespace Ui {
class Nagradi;
}

class Nagradi : public QDialog
{
    Q_OBJECT

public:
    explicit Nagradi(QWidget *parent = 0);
    ~Nagradi();

 bool static isOpen;

 void refreshData();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_nagradi_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_nagradi_clicked(const QModelIndex &index);

private:
    Ui::Nagradi *ui;
    Dal_prepodcontrol *dal_prepodcontrol;

public slots:
};

#endif
