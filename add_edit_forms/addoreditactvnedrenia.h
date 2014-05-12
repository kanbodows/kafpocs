#ifndef ADDOREDITACTVNEDRENIA_H
#define ADDOREDITACTVNEDRENIA_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditActVnedrenia;
}

class AddOrEditActVnedrenia : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditActVnedrenia(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditActVnedrenia();
        bool isEdit;

private slots:
        void on_comboBox_grupa_activated(int index);

        void on_radioButton_tema_kg_clicked();

        void on_radioButton_tema_rus_clicked();

        void on_pushButton_ok_clicked();

        void on_pushButton_cancel_clicked();

        void on_comboBox_tema_activated(int index);

private:
    Ui::AddOrEditActVnedrenia *ui;
    Dal_studentsControl *dal_studentsControl;
    int stud_id;
    int group_id;
    int tema_id;
    int rec_id;
    bool sel;
    int ids_tema;
    int ids_tema2;
};

#endif // ADDOREDITACTVNEDRENIA_H
