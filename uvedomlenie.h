#ifndef UVEDOMLENIE_H
#define UVEDOMLENIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "mainwindow.h"
extern DAL_main  *dal_main;
namespace Ui {
class uvedomlenie;
}

class uvedomlenie : public QDialog
{
    Q_OBJECT

public:
    explicit uvedomlenie(QWidget *parent = 0);
    ~uvedomlenie();
    bool static isOpen;
private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::uvedomlenie *ui;
};

#endif // UVEDOMLENIE_H
