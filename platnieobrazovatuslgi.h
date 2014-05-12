#ifndef PLATNIEOBRAZOVATUSLGI_H
#define PLATNIEOBRAZOVATUSLGI_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_prepodcontrol.h"
#include"add_edit_forms/add_or_edit_platnieobrazovatkursi.h"
#include <QString>
namespace Ui {
class PlatnieObrazovatUslgi;
}

class PlatnieObrazovatUslgi : public QDialog
{
    Q_OBJECT

public:
    explicit PlatnieObrazovatUslgi(QWidget *parent = 0);
    ~PlatnieObrazovatUslgi();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButton_find_clicked();

    void on_comboBoxVidKursa_activated(const QString &arg1);

    void on_pushButton_clear_clicked();

    void on_pushButton_upd_clicked();

    void on_pushButton_4_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButton_Edit_clicked();

    void on_actionDelete_triggered();

    void on_actionEdit_triggered();

    void on_tableViewPlObrazUslugi_doubleClicked(const QModelIndex &index);

    void on_tableViewPlObrazUslugi_clicked(const QModelIndex &index);

private:
    Ui::PlatnieObrazovatUslgi *ui;
    QSqlQueryModel *ObrazovatKursModels;
    Dal_prepodcontrol *dal_prepodcontrol;
    add_or_edit_PlatnieObrazovatKursi *add_or_edit_PlatnieObrazovatKursiform;
    bool vidim;
    int count;
    QString NazvanieKursa;
    QString vidKursa;
    QString naideno;
};

#endif // PLATNIEOBRAZOVATUSLGI_H
