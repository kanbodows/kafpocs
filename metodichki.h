#ifndef METODICHKI_H
#define METODICHKI_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "dals/dal_prepodcontrol.h"
#include "add_edit_forms/addoredit_metodichki.h"

//extern DAL_main  *dal_main;
namespace Ui {
class Metodichki;
}

class Metodichki : public QDialog
{
    Q_OBJECT

public:
    explicit Metodichki(QWidget *parent = 0);
    ~Metodichki();
    bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_comboBox_met_type_activated(int index);

    void on_comboBox_discipline_activated(int index);

    void on_pushButton_edit_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_actionOpenFile_triggered();

private:
    Ui::Metodichki *ui;
    QSqlTableModel *met_typesModel;
    QSqlTableModel *disciplineModel;
     Dal_prepodcontrol *dal_prepodcontrol;
    int met_type;
    int discipline;
public slots:
   void refreshData();
};

#endif // METODICHKI_H
