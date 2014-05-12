#ifndef SPECIALNOSTI_H
#define SPECIALNOSTI_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Specialnosti;
}

class Specialnosti : public QDialog
{
    Q_OBJECT

public:
    explicit Specialnosti(QWidget *parent = 0);
    ~Specialnosti();
    bool static isOpen;

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Specialnosti *ui;
    QSqlTableModel *specialnostiiModel;
};

#endif // KAFEDRY_H
