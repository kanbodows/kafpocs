#ifndef ACTVNEDRENIA_H
#define ACTVNEDRENIA_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include"add_edit_forms/addoreditactvnedrenia.h"
#include <QString>
namespace Ui {
class ActVnedrenia;
}

class ActVnedrenia : public QDialog
{
    Q_OBJECT

public:
    explicit ActVnedrenia(QWidget *parent = 0);
    ~ActVnedrenia();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_dateEdit_dateChanged(const QDate &date);

    void on_dateEdit_end_dateChanged(const QDate &date);

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_actVnedr_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::ActVnedrenia *ui;
    QSqlQueryModel *ActVnefreniaModels;
    Dal_studentsControl *dal_studentsControl;
    AddOrEditActVnedrenia *addoreditform;
    bool vidim;
    int count;
    QString Student;
    QString Group;
    QString naideno;
    bool isDate;
    QDate dateBeginning;
    QDate dateEnd;
};

#endif // ACTVNEDRENIA_H
