#ifndef PROSMOTRPLANAZASKAF_H
#define INSERT 1
#define EDIT 2
#define PROSMOTRPLANAZASKAF_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include "add_edit_forms/addoreditplanzaskaf.h"
namespace Ui {
class ProsmotrPlanaZasKaf;
}

class ProsmotrPlanaZasKaf : public QDialog
{
    Q_OBJECT

public:
    explicit ProsmotrPlanaZasKaf(QWidget *parent = 0, QString GodSS="", QString GodPo="");
    ~ProsmotrPlanaZasKaf();

private slots:
    void on_pushButtonAdd_clicked();

    void on_pushButton_Edit_clicked();

    void on_actionPostavitGalochku_triggered();

    void on_pushButton_Del_clicked();

    void on_pushButton_upd_clicked();

private:
    Ui::ProsmotrPlanaZasKaf *ui;
    QSqlTableModel *ProsmotrPlanaZasKafModel;
    Dal_prepodcontrol *dal_prepodcontrol;
    AddOrEditPlanZasKaf *AddOrEditPlanZasKafForm;
    QString GodStart;
    QString GodEnd;
};

#endif // PROSMOTRPLANAZASKAF_H
