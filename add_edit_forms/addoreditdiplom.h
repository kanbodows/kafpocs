#ifndef ADDOREDITDIPLOM_H
#define ADDOREDITDIPLOM_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include "styles.h"
extern DAL_main *dal_main;

namespace Ui {
class AddOrEditDiplom;
}

class AddOrEditDiplom : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditDiplom(QWidget *parent = 0);
    ~AddOrEditDiplom();

private slots:
    void on_comboBox_group_activated(int index);

    void on_comboBox_temarus_activated(int index);

    void on_pushButton_addWidjet_clicked();

    void on_pushButton_addBase_clicked();

    void on_tableView_stud_clicked(const QModelIndex &index);

    void on_comboBox_formirKom_activated(int index);

    void on_comboBox_ruk_activated(int index);

    void on_comboBox_temaKg_activated(int index);

    void on_actionDelete_triggered();

private:
    Ui::AddOrEditDiplom *ui;
    Dal_studentsControl *dal_studentsControl;
    QSqlQueryModel *rukModel;
    int group;
    int formirKom;
    int temaRusKg;
    int rukovod;
    Styles *css;
    void addRMToTableWidget(QString FIOStud, QString temaRus,QString temaKg,QString GodFormKom,QString ruk,int FIOStud_id, int tema_id,int GodFormKom_id,int ruk_id);
};

#endif // ADDOREDITDIPLOM_H
