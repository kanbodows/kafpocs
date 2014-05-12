#include "uvedomlenie.h"
#include "ui_uvedomlenie.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QSqlQuery>
bool uvedomlenie::isOpen = false;
uvedomlenie::uvedomlenie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::uvedomlenie)
{
    ui->setupUi(this);
    showNormal();
    QDesktopWidget widget;
    QRect mainScreenSize = widget.availableGeometry(widget.primaryScreen());
    move(mainScreenSize.width()-this->width()-18,mainScreenSize.height()-this->height()-38);
    if (!isOpen)
    {
        isOpen = true;
    }
    else
    {
        close();
    }
//    QString msg;
//    QSqlQuery* checkNaznPoruch = new QSqlQuery(dal_main->db);
//    checkNaznPoruch->prepare("SELECT count(id_poruchenie) FROM is_porucheniya where  checkPoruch = 0 and poruchitel = " + QString::number(dal_main->getCurrentEmployee()));
//    QSqlQuery *checkIspolPoruch = new QSqlQuery(dal_main->db);
//    checkIspolPoruch->prepare("SELECT count(id_poruchenie) FROM is_porucheniya where  checkIspoln = 0 and ispolnitel = " + QString::number(dal_main->getCurrentEmployee()));
//    checkNaznPoruch->exec();
//    checkNaznPoruch->first();
//    if(checkNaznPoruch->value(0).toInt()>0)
//        msg.append(tr("Всего поручений, где вы менежжер, требующие вашего просмотра: ") + checkNaznPoruch->value(0).toString());
//    checkIspolPoruch->exec();
//    checkIspolPoruch->first();
//    if(checkIspolPoruch->value(0).toInt()>0)
//        msg.append(tr("\nВсего поручений, где вы испонитель, требующие вашего просмотра: ") + checkIspolPoruch->value(0).toString());
//    ui->pushButton_ok->setText(msg);

}

uvedomlenie::~uvedomlenie()
{
    isOpen = false;
    delete ui;
}

void uvedomlenie::on_pushButton_ok_clicked()
{
    MainWindow* mainWindow = static_cast<MainWindow*>(this->parent());
    mainWindow->openProsmotr();
    close();
}
