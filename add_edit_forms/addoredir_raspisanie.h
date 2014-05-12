#ifndef ADDOREDIR_RASPISANIE_H
#define ADDOREDIR_RASPISANIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QList>
#include <QStringList>
#include "myprint.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>

namespace Ui {
class addoredir_raspisanie;
}

class addoredir_raspisanie : public QDialog
{
    Q_OBJECT

public:
    explicit addoredir_raspisanie(QWidget *parent = 0, int id_sotr = 0);
    ~addoredir_raspisanie();
    bool isEdit;
    int id_sotr;
private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_print_clicked();

    void on_comboBox_sotr_currentIndexChanged(int index);

    void printDocument(QPrinter *printer);
private:
    Ui::addoredir_raspisanie *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    QStringList days;
    QString body;
};

#endif // ADDOREDIR_RASPISANIE_H
