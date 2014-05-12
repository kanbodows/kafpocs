#ifndef VEDOMOSTIKURSOVYE_H
#define VEDOMOSTIKURSOVYE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "add_edit_forms/addoredit_vedomostikursovye.h"
#include "add_edit_forms/addoredit_aktykursovye.h"
extern DAL_main  *dal_main;
namespace Ui {
class VedomostiKursovye;
}

class VedomostiKursovye : public QDialog
{
    Q_OBJECT

public:
    explicit VedomostiKursovye(QWidget *parent = 0);
    ~VedomostiKursovye();

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

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_vedom_clicked(const QModelIndex &index);

    void on_pushButto_spisat_clicked();

private:
    Ui::VedomostiKursovye *ui;
    Dal_studentsControl *dal_studentsControl;

public slots:
};

#endif // VEDOMOSTIKURSOVYE_H
