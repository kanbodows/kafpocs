#ifndef PROCENTOVKA_H
#define PROCENTOVKA_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include"dals/dal_prepodcontrol.h"
#include"add_edit_forms/add_or_edit_procentovka.h"
#include <QString>
#include "prosmotr_redaktirovanie_procentovok.h"

namespace Ui {
class procentovka;
}

class procentovka : public QDialog
{
    Q_OBJECT

public:
    explicit procentovka(QWidget *parent = 0);
    ~procentovka();
    bool static isOpen;
    void refreshData();

private slots:
    void on_comboBox_rukovod_activated(int index);

    void on_pushButton_add_clicked();

    void on_pushButton_clicked();

    void on_pushButton_edit_clicked();

private:
    Ui::procentovka *ui;
    QSqlQueryModel *ProcentkaModels;
    Dal_studentsControl *dal_studentsControl;
    Dal_prepodcontrol *dal_prepodcontrol;
    int rukovod;
    add_or_edit_procentovka *add_or_edit_procentovkaform;
    Prosmotr_redaktirovanie_procentovok *Prosmotr_redaktirovanie_procentovokForm;
};

#endif // PROCENTOVKA_H
