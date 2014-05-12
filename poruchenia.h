#ifndef PORUCHENIA_H
#define PORUCHENIA_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QString>
#include "add_edit_forms/addoredit_poruchenie.h"
#include "add_edit_forms/edit_poruchstatus.h"

//extern DAL_main  *dal_main;
namespace Ui {
class Poruchenia;
}

class Poruchenia : public QDialog
{
    Q_OBJECT

public:
    explicit Poruchenia(QWidget *parent = 0);
    ~Poruchenia();
     bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_comboBox_status_activated(int index);

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_poruchenia_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_poruchenia_clicked(const QModelIndex &index);

private:
    Ui::Poruchenia *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QSqlTableModel *comboStatus;
    int status_id;
public slots:
    void refreshData();
};

#endif // PORUCHENIA_H
