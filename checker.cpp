#include "checker.h"
//#include <accessdb.h>
QSqlQuery *tester;
checker::checker(QObject *parent) :  DAL_main(parent)
{
    tester=new QSqlQuery(dal_main->db);
}

void checker::startCheck()
{
    if (dal_main->db.isOpen())
    {
        if (tester->exec("SELECT id_course FROM is_course limit 1"))
        emit checked(true);
        else {dal_main->db.close(); emit checked(false);}
    }
    else
    {
        if(dal_main->db.open()) emit checked(true);
        else emit checked(false);
    }
    emit stop();
}
