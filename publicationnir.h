#ifndef PUBLICATIONNIR_H
#define PUBLICATIONNIR_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include "add_edit_forms/addoreditpublicationnir.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class PublicationNir;
}

class PublicationNir : public QDialog
{
    Q_OBJECT

public:
    explicit PublicationNir(QWidget *parent = 0);
    ~PublicationNir();
     bool static isOpen;

private slots:
     void on_pushButton_search_clicked();

     void on_pushButton_find_clicked();

     void on_pushButton_clear_clicked();

     void on_pushButton_del_clicked();

     void on_actionOpenFail_triggered();

     void on_pushButton_update_clicked();

     void on_pushButton_add_clicked();

     void on_pushButtonEdit_clicked();

     void on_actionEdit_triggered();

     void on_actionDelete_triggered();

private:
    Ui::PublicationNir *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QSqlQueryModel *PublicationNirModel;
    int pub_nir_id;
    QString naideno;
    QString NomerProekta;
    QString naimenovanieFaila;
    QString files;

public slots:
    void refreshData();
};

#endif // PUBLICATIONNIR_H
