#ifndef ACCESSDB_H
#define ACCESSDB_H

#include <QDialog>
#include "dals/dal_main.h"
#include <QMessageBox>
#include <QSettings>
#include <styles.h>
extern DAL_main *dal_main;
namespace Ui {
class accessdb;
}

class accessdb : public QDialog
{
    Q_OBJECT
    
public:
    explicit accessdb(QWidget *parent = 0);
    ~accessdb();
    int marker;
    int pointerForClose;
    
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_lineEdit_dbName_cursorPositionChanged();

    void on_lineEdit_port_cursorPositionChanged();

    void on_lineEdit_host_cursorPositionChanged();

    void on_lineEdit_userName_cursorPositionChanged();

    void on_lineEdit_password_cursorPositionChanged();

private:
    Ui::accessdb *ui;
    Styles *css;
protected:
    void closeEvent(QCloseEvent *e);
};

#endif // ACCESSDB_H
