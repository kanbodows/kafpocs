#ifndef STATIYA_H
#define STATIYA_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include"dals/dal_prepodcontrol.h"
#include <QString>
#include"add_edit_forms/addoreditstat.h"
extern DAL_main  *dal_main;
namespace Ui {
class Statiya;
}

class Statiya : public QDialog
{
    Q_OBJECT

public:
    explicit Statiya(QWidget *parent = 0);
    ~Statiya();
 bool static isOpen;
 void refreshData();
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();


    void on_Statiya_accepted();

    void on_pushButton_edit_clicked();

    void on_tableView_statia_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::Statiya *ui;
    QSqlRelationalTableModel *statiaModel;
    QSqlQueryModel *statiaModels;
    Dal_prepodcontrol *dal_prepodcontrol;
    QSqlTableModel *ComboAvtor;
    QSqlTableModel *ComboSoAvtor;
    int count;
    int id_avtor;
    int id_soavtor;
    QString tema;
    QString Avtor;
    QString soAvtor;
    QString naideno;
    AddOrEditStat *addoreditform;
    bool vidim;
public slots:
};

#endif // STATIYA_H
