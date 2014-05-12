#ifndef ADDOREDITBOLEZNISTUD_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITBOLEZNISTUD_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditBolezniStud;
}

class AddOrEditBolezniStud : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditBolezniStud(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditBolezniStud();
    bool isEdit;
private slots:
    void on_pushButtonOK_clicked();

    void on_comboBoxGroups_activated(int index);

    void on_comboBox_Stud_activated(int index);

private:
    Ui::AddOrEditBolezniStud *ui;
    Dal_studentsControl *dal_studentsControl;
    int stud_id;
    int group_id;
    int rec_id;
};

#endif // ADDOREDITBOLEZNISTUD_H
