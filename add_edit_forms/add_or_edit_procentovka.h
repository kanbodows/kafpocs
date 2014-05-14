#ifndef ADD_OR_EDIT_PROCENTOVKA_H
#define ADD_OR_EDIT_PROCENTOVKA_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_studentscontrol.h"
namespace Ui {
class add_or_edit_procentovka;
}

class add_or_edit_procentovka : public QDialog
{
    Q_OBJECT

public:
    explicit add_or_edit_procentovka(QWidget *parent = 0, int select = 0, int id = -1);
    ~add_or_edit_procentovka();

private slots:
    void on_pushButtonSformGrafProc_clicked();

    void on_pushButton_addToDatabase_clicked();

private:
    Ui::add_or_edit_procentovka *ui;
    void addRMToTableWidget(QDate dates, QString FioStud,int vved, int analTreb,int konstr,int docum,int eksperement,int ekonomich,int zakl,int rabotaPP,int prezentation,int id_students);
     Dal_studentsControl* dal_studentsControl;
     int id_studenta;
     QString FIOStudenta;
     QDate datas;
     int days;
};

#endif // ADD_OR_EDIT_PROCENTOVKA_H
