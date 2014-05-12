#ifndef ADDGRAFVZAIMOPOS_H
#define INSERT 1
#define EDIT 2
#define ADDGRAFVZAIMOPOS_H

#include <QDialog>
#include "dals/dal_prepodcontrol.h"
#include "dals/dal_studentscontrol.h"
#include "styles.h"
extern DAL_main *dal_main;
namespace Ui {
class AddGrafVzaimopos;
}

class AddGrafVzaimopos : public QDialog
{
    Q_OBJECT

public:
    explicit AddGrafVzaimopos(QWidget *parent = 0, int select = 0, int id = -1, QString GodS1="", QString GodPo1="");
    ~AddGrafVzaimopos();
    bool isEdit;

private slots:

    void on_comboBox_Prepod_activated(int index);

    void on_pushButtonAddToTableWidjet_clicked();

    void on_pushButton_AddDataBase_clicked();

    void on_pushButtonSave_clicked();

private:
    Ui::AddGrafVzaimopos *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    Dal_studentsControl *dal_studentsControl;
    void addRMToTableWidget(QString FIOPrepod, QString FIOProv, QDateTime DataProv, int FIOPrepod_id, int FIOProv_id);
    int sotr_ids;
    int sel_ct;
    int rec_id;
};

#endif // ADDGRAFVZAIMOPOS_H
