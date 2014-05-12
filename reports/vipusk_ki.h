#ifndef VIPUSK_KI_H
#define VIPUSK_KI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "myprint.h"

namespace Ui {
class Vipusk_ki;
}

class Vipusk_ki : public QDialog
{
    Q_OBJECT

public:
    explicit Vipusk_ki(QWidget *parent = 0);
    ~Vipusk_ki();
    bool static isOpen;

private slots:

    void on_pushButton_showReport_clicked();

    void printDocument(QPrinter *printer);

private:
    Ui::Vipusk_ki *ui;
    Dal_studentsControl *dal_studentsControl;
    QTextDocument* body;
};

#endif // VIPUSK_KI_H
