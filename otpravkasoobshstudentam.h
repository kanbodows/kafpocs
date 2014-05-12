#ifndef OTPRAVKASOOBSHSTUDENTAM_H
#define OTPRAVKASOOBSHSTUDENTAM_H
#include "dals/dal_main.h"
#include <QSqlRelationalTableModel>
#include"dals/dal_studentscontrol.h"
#include <QString>
#include <QDialog>
#include "smtp.h"
namespace Ui {
class OtpravkaSoobshStudentam;
}

class OtpravkaSoobshStudentam : public QDialog
{
    Q_OBJECT

public:
    explicit OtpravkaSoobshStudentam(QWidget *parent = 0);
    ~OtpravkaSoobshStudentam();
     bool static isOpen;
private slots:
    void on_pushButton_sendSoobsh_clicked();

    void on_comboBox_group_activated(const QString &arg1);

    void on_comboBox_group_activated(int index);

private:
    Ui::OtpravkaSoobshStudentam *ui;
    QSqlQueryModel *StudentModels;
    Dal_studentsControl *dal_studentsControl;
    int Group;

    QString mess;
    QString logc;
    QString parol;
    QString mais;
};

#endif // OTPRAVKASOOBSHSTUDENTAM_H
