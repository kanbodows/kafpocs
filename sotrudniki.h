#ifndef SOTRUDNIKI_H
#define SOTRUDNIKI_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_prepodcontrol.h"
#include"add_edit_forms/addoreditsotrudnik.h"
#include <QString>
namespace Ui {
class Sotrudniki;
}

class Sotrudniki : public QDialog
{
    Q_OBJECT

public:
    explicit Sotrudniki(QWidget *parent = 0);
    ~Sotrudniki();
    bool static isOpen;
    void refreshData();


private slots:
    void on_comboBox_doljnost_activated(int index);

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_action_podrobno_triggered();

    void on_pushButton_clear_clicked();

    void on_actionDelete_triggered();

    void on_tableView_sotrudniki_doubleClicked(const QModelIndex &index);

    void on_tableView_sotrudniki_clicked(const QModelIndex &index);

private:
    Ui::Sotrudniki *ui;
    QSqlQueryModel *SotrModels;
    Dal_prepodcontrol *dal_prepodcontrol;
    AddOrEditSotrudnik *addoreditform;
    bool vidim;
    int count;
     QString Sotr;
     QString naideno;
     int vozrast;
     int doljnost;
    //    bool isDate;
    //    QDate dateBeginning;
    //    QDate dateEnd;
};

#endif // SOTRUDNIKI_H
