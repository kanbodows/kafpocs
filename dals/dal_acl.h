#ifndef DAL_ACL_H
#define DAL_ACL_H
#include "dal_main.h"
extern DAL_main  *dal_main;
class DAL_ACL : public DAL_main
{
public:
    DAL_ACL(QObject *parent);
    int authorization(QString login, QString password, QWidget *parent);
    QSqlQueryModel *getEmployees();
    QSqlQuery *getCurrentEmployee(int id);
    QSqlQueryModel *getDolj();
    QSqlQueryModel *getPassw();
    int getCurrentRole();

    QSqlQueryModel *getLogins(QString name = "", QString login = "", int role_id = 0, int isActive = 1, int point_id = 0);
    bool addNewLogin(int employees_id, QString login, QString pass, int role_id, bool isActive);
    bool EditCurentLogin(int rec_id,int employees_id, QString login, QString pass, int role_id, bool isActive);
    QSqlQuery *getAcl(int roleId);

private:
    QSqlQueryModel *getEmployeesModel;
    QSqlQueryModel static *getDoljModel;
    QSqlQueryModel static *getLoginsModel;

};

#endif // DAL_ACL_H
