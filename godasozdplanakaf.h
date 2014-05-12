#ifndef GODASOZDPLANAKAF_H
#define GODASOZDPLANAKAF_H

#include <QDialog>
#include "dals/dal_main.h"
#include <QString>
#include "add_edit_forms/addoreditplanzaskaf.h"
#include "prosmotrplanazaskaf.h"
#include "dals/dal_prepodcontrol.h"
extern DAL_main  *dal_main;
namespace Ui {
class GodaSozdPlanaKaf;
}

class GodaSozdPlanaKaf : public QDialog
{
    Q_OBJECT

public:
    explicit GodaSozdPlanaKaf(QWidget *parent = 0);
    ~GodaSozdPlanaKaf();
    bool static isOpen;

private slots:
    void on_pushButton_createplan_clicked();

    void on_pushButton_viewPlan_clicked();

    void on_pushButtonDelPlan_clicked();

    void on_tableViewGodSozdPlKaf_doubleClicked(const QModelIndex &index);

    void on_actionDelPlanZasedKaf_triggered();

private:
    Ui::GodaSozdPlanaKaf *ui;
    QSqlTableModel *GodSozdPlKafModel;
    AddOrEditPlanZasKaf *AddOrEditPlanZasKafForm;
    ProsmotrPlanaZasKaf *ProsmotrPlanaZasKafForm;
    Dal_prepodcontrol *dal_prepodcontrol;
};

#endif // GODASOZDPLANAKAF_H
