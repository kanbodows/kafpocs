//Моё
#ifndef DAL_MAIN_H
#define DAL_MAIN_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtSql>
#include <QTemporaryFile>
#include <QThread>
#include <QApplication>
//
class DAL_main : public QObject
{
    Q_OBJECT
public:
    QTemporaryFile *conf;
    explicit DAL_main(QObject *parent = 0);
    ~DAL_main();

    bool setConnection(QString dbname, int port, QString host, QString username, QString password);
    bool checkConnection();
    void setCurrentEmployee(int employee_id);
    QSqlDatabase *getCurrentConnection();
    void closeConnection();
   void setCurrentRole(int dolj_id);

   int getCurrentEmployee();
   int getCurrentRole();

    QSqlDatabase db;
    /**
     * @brief checkKey ключ синглота
     */
    bool static next; bool static checkKey;
signals:
    
public slots:
    void checked(bool);
private:


    
};

#endif // DAL_MAIN_H
