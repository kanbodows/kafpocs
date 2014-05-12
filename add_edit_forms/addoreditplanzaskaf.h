#ifndef ADDOREDITPLANZASKAF_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITPLANZASKAF_H

#include <QDialog>
#include "dals/dal_prepodcontrol.h"
#include "styles.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditPlanZasKaf;
}

class AddOrEditPlanZasKaf : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditPlanZasKaf(QWidget *parent = 0, int select = 0, int id = -1, QString GodS1="", QString GodPo1="");
    ~AddOrEditPlanZasKaf();
    bool isEdit;

private slots:
    void on_pushButtonAddTableWidjet_clicked();

    void on_checkBoxAllPrepod_clicked(bool checked);

    void on_checkBox_Aspirant_clicked(bool checked);

    void on_pushButtonCancel_clicked();

    void on_pushButton_AddToDataBase_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::AddOrEditPlanZasKaf *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    void addRMToTableWidget(QString Vopros, QString Ispolnit,QString SrokiIspoln);
    QString ispolnit;
    int sel_ct;
    int rec_id;
};

#endif // ADDOREDITPLANZASKAF_H
