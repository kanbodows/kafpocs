#ifndef DOLJNOST_H
#define DOLJNOST_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Doljnost;
}

class Doljnost : public QDialog
{
    Q_OBJECT

public:
    explicit Doljnost(QWidget *parent = 0);
    ~Doljnost();
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
    Ui::Doljnost *ui;
    QSqlTableModel *doljnostiModel;
};

#endif // DOLJNOST_H
