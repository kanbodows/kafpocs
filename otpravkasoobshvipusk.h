#ifndef OTPRAVKASOOBSHVIPUSK_H
#define OTPRAVKASOOBSHVIPUSK_H
#include "dals/dal_main.h"
#include <QSqlRelationalTableModel>
#include"dals/dal_studentscontrol.h"
#include <QString>
#include <QDialog>
#include "smtp.h"

namespace Ui {
class OtpravkaSoobshVipusk;
}

class OtpravkaSoobshVipusk : public QDialog
{
    Q_OBJECT

public:
    explicit OtpravkaSoobshVipusk(QWidget *parent = 0);
    ~OtpravkaSoobshVipusk();
    bool static isOpen;
private slots:

    void on_pushButton_sendSoobsh_clicked();

    void on_comboBox_group_activated(const QString &arg1);

private:
    Ui::OtpravkaSoobshVipusk *ui;
    QSqlQueryModel *vipusknikModels;
    Dal_studentsControl *dal_studentsControl;
    QString Student;
    QString Group;
    QString naideno;
    QDate dateBeginning;
    QDate dateEnd;

    QString mess;
    QString logc;
    QString parol;
    QString mais;

    QSqlRelationalTableModel *OtprSoobModel;
};

#endif // OTPRAVKASOOBSHVIPUSK_H
