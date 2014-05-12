//Мое
#include "dal_main.h"
#include "accessdb.h"
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <checker.h>
#include <QLabel>
#include <QMovie>
#include <QDesktopWidget>
#include <QDialog>
QSqlQuery *test;
QMovie *load;
static QLabel *waiter;
bool checkRes;
bool DAL_main::checkKey = false;
bool DAL_main::next = false;
QDialog *prog;
DAL_main::DAL_main(QObject *parent) :
    QObject(parent)
{
    conf = new QTemporaryFile;
    load=new QMovie(":/img/img/loader.gif");
    prog=new QDialog;
    prog->setFixedSize(200,200);
    prog->setModal(true);
    prog->setWindowModality(Qt::ApplicationModal);
        //prog->setWindowTitle(tr("Exporting..."));
    prog->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
    waiter = new QLabel(prog);
    waiter->setMovie(load);
    waiter->setFixedSize(200,200);

}
DAL_main::~DAL_main()
{
    delete conf;
}

// установление соединения с бд
bool DAL_main::setConnection(QString dbname, int port, QString host, QString username, QString password)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(dbname);
    db.setPort(port);
    db.setHostName(host);
    db.setUserName(username);
    db.setPassword(password);

    if (db.open())
    {
        test=new QSqlQuery(db);
        return true;
    }
    else
    {

        return false;
    }
}

// проверка соединения с базой
bool DAL_main::checkConnection()
{
    if (!checkKey){
    checkKey=true;
    checker * chk= new checker;
    QThread *thread=new QThread;
    chk->moveToThread(thread);
    connect(thread,SIGNAL(started()),chk,SLOT(startCheck()));
    connect(chk,SIGNAL(stop()),thread,SLOT(quit()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    connect (chk,SIGNAL(checked(bool)),this,SLOT(checked(bool)));
    thread->start();
    load->start();
    }
    //chk->startCheck();
    int i=0; next=false;
    while (!next)
    {
        if (i>10000 && !prog->isVisible() && !checkKey)
            prog->show();
        i++;
        QApplication::processEvents();
        //this->thread()->msleep(100);
    }
    return checkRes;
}

void DAL_main::setCurrentEmployee(int sotr_id)
{
    if(!conf->isOpen())
    if (!conf->open())
    {
        QMessageBox::warning(NULL,tr("Ошибка!"),tr("Невозможно создать временный файл!"));
    }
    QSettings qs(conf->fileName(), QSettings::IniFormat);
    qs.setValue("lid",sotr_id);
}

int DAL_main::getCurrentEmployee()
{
    if(!conf->isOpen())
    if (!conf->open())
    {
        QMessageBox::warning(NULL,tr("Ошибка!"),tr("Невозможно открыть временный файл!"));
        return -1;
    }
    QSettings qs(conf->fileName(), QSettings::IniFormat);
    return qs.value("lid").toInt();
}

QSqlDatabase *DAL_main::getCurrentConnection()
{
    if (db.isOpen())
    {
        return &db;
    }
    else
    {
        return NULL;
    }
}

void DAL_main::closeConnection()
{
    db.close();
}

void DAL_main::setCurrentRole(int dolj_id)
{
    if(!conf->isOpen())
    if (!conf->open())
    {
        QMessageBox::warning(NULL,tr("Ошибка!"),tr("Невозможно создать временный файл!"));
    }
    QSettings qs(conf->fileName(), QSettings::IniFormat);
    qs.setValue("rid",dolj_id);
}

int DAL_main::getCurrentRole()
{
    if(!conf->isOpen())
    if (!conf->open())
    {
        QMessageBox::warning(NULL,tr("Ошибка!"),tr("Невозможно открыть временный файл!"));
        return -1;
    }
    QSettings qs(conf->fileName(), QSettings::IniFormat);
    return qs.value("rid").toInt();
}


void DAL_main::checked(bool i)
{
    qDebug()<<i;
    checkRes=i;
  prog->hide(); prog->close(); load->stop();//}   if(i)
    next=true; checkKey=false;
}

