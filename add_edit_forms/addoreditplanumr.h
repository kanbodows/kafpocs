#ifndef ADDOREDITPLANUMR_H
#define ADDOREDITPLANUMR_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
extern DAL_main *dal_main;

namespace Ui {
class AddOrEditPlanUmr;
}

class AddOrEditPlanUmr : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditPlanUmr(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditPlanUmr();
    bool isEdit;

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_cancel_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::AddOrEditPlanUmr *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QString ispolnit;
    int rec_id;
};

#endif // ADDOREDITPLANUMR_H
