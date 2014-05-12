#ifndef OTPRAVKASOOBSHENIASOTRUDNIKAM_H
#define OTPRAVKASOOBSHENIASOTRUDNIKAM_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include <QString>
#include <QDialog>
#include "smtp.h"
namespace Ui {
class OtpravkaSoobsheniaSotrudnikam;
}

class OtpravkaSoobsheniaSotrudnikam : public QDialog
{
    Q_OBJECT

public:
    explicit OtpravkaSoobsheniaSotrudnikam(QWidget *parent = 0);
    ~OtpravkaSoobsheniaSotrudnikam();
    bool static isOpen;

private slots:
    void on_pushButton_sendSoobsh_clicked();

private:
    Ui::OtpravkaSoobsheniaSotrudnikam *ui;
    Dal_studentsControl *dal_studentsControl;
    QSqlQueryModel *SotrKafModels;

    QString mess;
    QString logc;
    QString parol;
    QString mais;
};

#endif // OTPRAVKASOOBSHENIASOTRUDNIKAM_H
