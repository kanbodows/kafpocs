#ifndef CHECKER_H
#define CHECKER_H

#include <QObject>
#include "dals/dal_main.h"
extern DAL_main  *dal_main;
class checker :
        public DAL_main
{
    Q_OBJECT
public:
    explicit checker(QObject *parent = 0);

signals:
    void checked(bool);
    void stop();
public slots:
    void startCheck();

};

#endif // CHECKER_H
