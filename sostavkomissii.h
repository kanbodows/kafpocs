#ifndef SOSTAVKOMISSII_H
#define SOSTAVKOMISSII_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "dals/dal_prepodcontrol.h"
#include "add_edit_forms/addoreditsostavkom.h"

namespace Ui {
class SostavKomissii;
}

class SostavKomissii : public QDialog
{
    Q_OBJECT

public:
    explicit SostavKomissii(QWidget *parent = 0);
    ~SostavKomissii();
    bool static isOpen;

private:
    Ui::SostavKomissii *ui;
    QSqlQueryModel *SostavKomModel;
    Dal_prepodcontrol *dal_prepodcontrol;
    AddOrEditSostavKom *AddOrEditSostavKomform;
    int count;
    int nomerRaporta;
public slots:
    void refreshData();
private slots:
    void on_pushButton_search_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_add_clicked();
    void on_lineEdit_nomerRaporta_editingFinished();
    void on_pushButton_edit_clicked();
    void on_actionEdit_triggered();
    void on_actionDelete_triggered();
    void on_tableView_sostavKomissii_clicked(const QModelIndex &index);
};

#endif // SOSTAVKOMISSII_H
