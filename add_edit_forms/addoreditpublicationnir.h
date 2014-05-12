#ifndef ADDOREDITPUBLICATIONNIR_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITPUBLICATIONNIR_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>

namespace Ui {
class AddOrEditPublicationNir;
}

class AddOrEditPublicationNir : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditPublicationNir(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditPublicationNir();
    bool isEdit;

private slots:
    void on_pushButton_clicked();

    void on_toolButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddOrEditPublicationNir *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    int pub_nirs;
    int rec_id;
    QString NameFails;
    QString UrlFail;
};

#endif // ADDOREDITPUBLICATIONNIR_H
