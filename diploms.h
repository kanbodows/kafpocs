#ifndef DIPLOMS_H
#define DIPLOMS_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include"add_edit_forms/addoreditdiplom.h"
#include <QString>
#include"prostavitocenki.h"
namespace Ui {
class Diploms;
}

class Diploms : public QDialog
{
    Q_OBJECT

public:
    explicit Diploms(QWidget *parent = 0);
    ~Diploms();
    bool static isOpen;
    void refreshData();
private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_comboBox_gruppa_activated(int index);

    void on_pushButton_add_clicked();

    void on_pushButton_postavitOcenki_clicked();

    void on_actionOcenka_triggered();

private:
    Ui::Diploms *ui;
    QSqlQueryModel *DiplomsModels;
    QString Student;
    QString naideno;
    int group;
    Dal_studentsControl *dal_studentsControl;
    AddOrEditDiplom *AddOrEditDiplomform;
    ProstavitOcenki *ProstavitOcenkiForm;
    bool vidim;
    int count;
};

#endif // DIPLOMS_H
