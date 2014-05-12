#ifndef PROSTAVITOCENKI_H
#define PROSTAVITOCENKI_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include"dals/dal_studentscontrol.h"
extern DAL_main  *dal_main;
namespace Ui {
class ProstavitOcenki;
}

class ProstavitOcenki : public QDialog
{
    Q_OBJECT

public:
    explicit ProstavitOcenki(QWidget *parent = 0, int id = -1, int rec_idDipl=-1);
    ~ProstavitOcenki();
     bool static isOpen;

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::ProstavitOcenki *ui;
    int rec;
    int stud_id;
    QSqlRelationalTableModel *DiplomsOcenkiModel;
    QSqlTableModel *comboGroupModel;
    Dal_studentsControl *dal_studentsControl;
};

#endif // PROSTAVITOCENKI_H
