#ifndef PRACWORK_H
#define PRACWORK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "add_edit_forms/addoreditpracwork.h"
extern DAL_main  *dal_main;

namespace Ui {
class PracWork;
}

class PracWork : public QDialog
{
    Q_OBJECT

public:
    explicit PracWork(QWidget *parent = 0);
    ~PracWork();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButton_search_2_clicked();

    void on_pushButton_clear_2_clicked();

    void on_pushButton_find_2_clicked();

    void on_dateEdit_2_dateChanged(const QDate &date);

    void on_dateEdit_end_2_dateChanged(const QDate &date);

    void on_comboBox_type_practika_activated(int index);

    void on_pushButton_del_2_clicked();

    void on_pushButton_update_2_clicked();

    void on_pushButton_add_2_clicked();

    void on_pushButton_edit_2_clicked();

    void on_tableView_pracwork_doubleClicked(const QModelIndex &index);

    void on_actionDetails_triggered();

    void on_actionDel_triggered();

private:
    Ui::PracWork *ui;
    QSqlQueryModel *pracWorkModels;
    AddOrEditPracWork *addoreditform;
    Dal_studentsControl *dal_studentsControl;
    bool vidim;
    int count;
    QString Student;
    QString Group;
    int type_prac;
    QString naideno;
    bool isDate;
    QDate dateBeginning;
    QDate dateEnd;
};

#endif // PRACWORK_H
