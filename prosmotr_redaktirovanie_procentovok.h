#ifndef PROSMOTR_REDAKTIROVANIE_PROCENTOVOK_H
#define PROSMOTR_REDAKTIROVANIE_PROCENTOVOK_H
#define EDIT 2
#include <QDialog>
#include "dals/dal_studentscontrol.h"

namespace Ui {
class Prosmotr_redaktirovanie_procentovok;
}

class Prosmotr_redaktirovanie_procentovok : public QDialog
{
    Q_OBJECT

public:
    explicit Prosmotr_redaktirovanie_procentovok(QWidget *parent = 0, int id = -1, bool prosmtr = false);
    ~Prosmotr_redaktirovanie_procentovok();

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::Prosmotr_redaktirovanie_procentovok *ui;
     Dal_studentsControl* dal_studentsControl;
     QSqlQueryModel *ProcentkaListModels;
     int stud_ids;
};

#endif // PROSMOTR_REDAKTIROVANIE_PROCENTOVOK_H
