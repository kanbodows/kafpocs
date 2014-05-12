#ifndef PORUCH_STATUS_H
#define PORUCH_STATUS_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Poruch_status;
}

class Poruch_status : public QDialog
{
    Q_OBJECT

public:
    explicit Poruch_status(QWidget *parent = 0);
    ~Poruch_status();
    bool static isOpen;
    bool checkRelation(QString id_dolj);

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Poruch_status *ui;
    QSqlTableModel *poruch_statusModel;
};

#endif // Poruch_status_H
