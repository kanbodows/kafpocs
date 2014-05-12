#ifndef BOLEZNISTUDENTOV_H
#define BOLEZNISTUDENTOV_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include"add_edit_forms/addoreditboleznistud.h"
extern DAL_main  *dal_main;
namespace Ui {
class BolezniStudentov;
}

class BolezniStudentov : public QDialog
{
    Q_OBJECT

public:
    explicit BolezniStudentov(QWidget *parent = 0);
    ~BolezniStudentov();
    bool static isOpen;
    void refreshData();
private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_find_clicked();

    void on_dateEdit_start_dateChanged(const QDate &date);

    void on_dateEdit_end_dateChanged(const QDate &date);

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_comboBox_groups_activated(int index);

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_studBolez_doubleClicked(const QModelIndex &index);

private:
    Ui::BolezniStudentov *ui;
    Dal_studentsControl *dal_studentsControl;
    QSqlTableModel *ComboGroups;
    QSqlQueryModel *boleznistudModels;

    int count;
    int id_groip;
    QString bolezn;
    QDate dateBeginning;
    QDate dateEnd;
    QString naideno;
    bool isDate;
    AddOrEditBolezniStud *AddOrEditBolezniStudform;
    bool vidim;
};

#endif // BOLEZNISTUDENTOV_H
