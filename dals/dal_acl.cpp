#include "dal_acl.h"
#include "authorization.h"
#include <QMessageBox>
#include <QDebug>
#include <QCryptographicHash>


DAL_ACL::DAL_ACL(QObject *parent = 0)
{

}

int DAL_ACL::authorization(QString login, QString password, QWidget *parent)
{
   // password = QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Sha1);
    QSqlQuery *queryaut = new QSqlQuery;
    queryaut->prepare("select st_doljnost, id_sotr, st_passwd  from is_sotrudniki where st_login = :login and st_passwd=:password");
    queryaut->bindValue(":login", login);
    queryaut->bindValue(":password", password);
    queryaut->exec();
    queryaut->first();
    if(!queryaut->isValid())
    {
    QMessageBox::warning(parent, tr("Ошибка Авторизации"), tr("Введен неверный ЛОГИН или ПАРОЛЬ"));
    return -1;
    }
    else
    {
    int sotr_id, dolj_id;
    sotr_id=queryaut->value(1).toInt();
    dolj_id=queryaut->value(0).toInt();
    dal_main->setCurrentEmployee(sotr_id);
    dal_main->setCurrentRole(dolj_id);
    return dolj_id;
    }

}
QSqlQueryModel *DAL_ACL::getEmployees()
{
    getEmployeesModel = new QSqlQueryModel(this);
    getEmployeesModel->setQuery("select * from is_sotrudniki");
    return getEmployeesModel;
}

QSqlQuery *DAL_ACL::getCurrentEmployee(int id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select id_sotr, st_login, st_passwd, st_doljnoat from is_sotrudniki where id_sotr = :id");
    query->bindValue(":id", id);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQueryModel *DAL_ACL::getDolj()
{
   QSqlQueryModel static *getDoljModel = new QSqlQueryModel(this);
    getDoljModel->setQuery("select * from is_doljnost");
    return getDoljModel;
}

QSqlQueryModel *DAL_ACL::getPassw()
{
   QSqlQueryModel static *getPasswdModel = new QSqlQueryModel(this);
    getPasswdModel->setQuery("select * from is_sotrudniki");
    return getPasswdModel;
}

/*
QSqlQueryModel *DAL_ACL::getLogins(QString name, QString login, int role_id, int isActive, int point_id)
{
    QSqlQueryModel static *getLoginsModel = new QSqlQueryModel(this);
    QString query = "", condition = "";
    query = "select * from logins_view where logins_view.`Активный` = " + QString::number(isActive);
    if (name != "")
    {
        condition.append(" and logins_view.`ФИО` like '%" + name + "%'");
    }
    if (login != "")
    {
        condition.append(" and logins_view.`Логин` like '%" + login + "%'");
    }
    if (role_id != 0)
    {
        condition.append(" and logins_view.role_id  = " + QString::number(role_id));
    }
    if (point_id != 0)
    {
        condition.append(" and logins_view.point_id = " + QString::number(point_id));
    }
    query = query + condition;
    getLoginsModel->setQuery(query);
    return getLoginsModel;

}


QSqlQuery *DAL_ACL::getAcl(int roleId)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from acl where role_id = :role");
    query->bindValue(":role",roleId);
    query->exec();
    return query;
}
*/




