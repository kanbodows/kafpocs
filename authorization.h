#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include "mainwindow.h"
#include "dals/dal_acl.h"
namespace Ui {
class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT
    
public:
    explicit Authorization(QWidget *parent = 0);
    ~Authorization();
    
private slots:
    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();


private:
    Ui::Authorization *ui;
    DAL_ACL *dal_acl;
protected:

};

#endif // AUTHORIZATION_H
