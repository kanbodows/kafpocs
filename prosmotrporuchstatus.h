#ifndef PROSMOTRPORUCHSTATUS_H
#define PROSMOTRPORUCHSTATUS_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QGroupBox>
#include <QMessageBox>
#include "add_edit_forms/edit_poruchstatus.h"
#include "poruchenia.h"

extern DAL_main  *dal_main;
namespace Ui {
class prosmotrPoruchStatus;
}

class prosmotrPoruchStatus : public QDialog
{
    Q_OBJECT

public:
    explicit prosmotrPoruchStatus(QWidget *parent = 0);
    ~prosmotrPoruchStatus();
     bool static isOpen;
private slots:
    void on_messageReadBtn_clicked();
    void showMessages();
    void on_messageDetailBtn_clicked();
    void on_pushButton_close_clicked();

    void on_pushButton_poruch_clicked();

private:
    Ui::prosmotrPoruchStatus *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    QSqlQuery *queryNaznPoruch;
    QSqlQuery *queryIspolPoruch;

};

#endif // PROSMOTRPORUCHSTATUS_H
