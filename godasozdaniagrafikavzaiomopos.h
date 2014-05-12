#ifndef GODASOZDANIAGRAFIKAVZAIOMOPOS_H
#define GODASOZDANIAGRAFIKAVZAIOMOPOS_H

#include <QDialog>
#include "dals/dal_main.h"
#include <QString>
#include "add_edit_forms/addgrafvzaimopos.h"
//#include "prosmotrplanazaskaf.h"
#include "dals/dal_prepodcontrol.h"
#include"prosmotrgrafvzaimopos.h"
extern DAL_main  *dal_main;
namespace Ui {
class GodaSozdaniaGrafikaVzaiomopos;
}

class GodaSozdaniaGrafikaVzaiomopos : public QDialog
{
    Q_OBJECT

public:
    explicit GodaSozdaniaGrafikaVzaiomopos(QWidget *parent = 0, QString GodSS="", QString GodPo="");
    ~GodaSozdaniaGrafikaVzaiomopos();
     bool static isOpen;

private slots:
     void on_pushButton_clicked();

     void on_pushButton_ProsmotraGrfika_clicked();

private:
    Ui::GodaSozdaniaGrafikaVzaiomopos *ui;
    QSqlTableModel *GodSozdGrafModel;
    AddGrafVzaimopos *AddGrafVzaimoposForm;
    ProsmotrGrafVzaimopos *ProsmotrGrafVzaimoposForm;
//    Gra *ProsmotrPlanaZasKafForm;
    Dal_prepodcontrol *dal_prepodcontrol;
};

#endif // GODASOZDANIAGRAFIKAVZAIOMOPOS_H
