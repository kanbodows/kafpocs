#ifndef PLANUMR_H
#define PLANUMR_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_prepodcontrol.h"
#include"add_edit_forms/addoreditplanumr.h"
#include <QString>
#include <QList>
#include <QStringList>
#include "myprint.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>

namespace Ui {
class PlanUMR;
}

class PlanUMR : public QDialog
{
    Q_OBJECT

public:
    explicit PlanUMR(QWidget *parent = 0);
    ~PlanUMR();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButton_Del_clicked();

    void on_pushButton_upd_clicked();

    void on_pushButtonAdd_clicked();

    void on_lineEdit_god_editingFinished();

    void on_pushButton_Edit_clicked();

private:
    Ui::PlanUMR *ui;
    QSqlQueryModel *PlanUMRModels;
    QString god;
    Dal_prepodcontrol *dal_prepodcontrol;
    AddOrEditPlanUmr *AddOrEditPlanUmrform;
};

#endif // PLANUMR_H
