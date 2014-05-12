#ifndef OLIMPIADA_H
#define OLIMPIADA_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include <QString>
#include "add_edit_forms/addoreditolimp.h"
extern DAL_main  *dal_main;
namespace Ui {
class Olimpiada;
}

class Olimpiada : public QDialog
{
    Q_OBJECT

public:
    explicit Olimpiada(QWidget *parent = 0);
    ~Olimpiada();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_tableView_olimp_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::Olimpiada *ui;
    AddOrEditOlimp *addoreditform;
    QSqlQueryModel *olimpModels;
    Dal_studentsControl *dal_studentsControl;
    bool vidim;
    int count;
    QString Student;
    QString Predmet;
    int ZanyatoeMesto;
    QString naideno;
};

#endif // OLIMPIADA_H
