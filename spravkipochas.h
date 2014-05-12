#ifndef SPRAVKIPOCHAS_H
#define SPRAVKIPOCHAS_H

#include <QDialog>
#include "dals/dal_main.h"
//#include "dals/dal_studentscontrol.h"
#include "dals/dal_prepodcontrol.h"
#include <QString>
#include "add_edit_forms/addoredit_spravkapochas.h"
extern DAL_main  *dal_main;
namespace Ui {
class SpravkiPochas;
}

class SpravkiPochas : public QDialog
{
    Q_OBJECT

public:
    explicit SpravkiPochas(QWidget *parent = 0);
    ~SpravkiPochas();

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


    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::SpravkiPochas *ui;
//    Dal_studentsControl *dal_studentsControl;
    Dal_prepodcontrol *dal_prepodcontrol;

public slots:
};

#endif // VEDOMOSTIKURSOVYE_H
