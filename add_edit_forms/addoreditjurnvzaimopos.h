#ifndef ADDOREDITJURNVZAIMOPOS_H
#define ADDOREDITJURNVZAIMOPOS_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditJurnVzaimopos;
}

class AddOrEditJurnVzaimopos : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditJurnVzaimopos(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditJurnVzaimopos();
 bool isEdit;
private slots:
 void on_comboBoxPrepod_activated(int index);

 void on_pushButton_OK_clicked();

 void on_comboBox_discipl_activated(int index);

 void on_comboBox_gruppa_activated(int index);

 void on_comboBox_prover_activated(int index);

 void on_pushButton_cancel_clicked();

 void on_comboBox_activated(const QString &arg1);

 void on_lineEdit_Ocenka_textChanged(const QString &arg1);

private:
    Ui::AddOrEditJurnVzaimopos *ui;
    Dal_studentsControl *dal_studentsControl;
    int sotr_ids;
    int discipl_ids;
    int group_ids;
    int prover_ids;
    int rec_id;
    QString tip_discipl;
};

#endif // ADDOREDITJURNVZAIMOPOS_H
