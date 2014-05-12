#ifndef KAFEDRY_H
#define KAFEDRY_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Kafedry;
}

class Kafedry : public QDialog
{
    Q_OBJECT

public:
    explicit Kafedry(QWidget *parent = 0);
    ~Kafedry();
    bool static isOpen;

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Kafedry *ui;
    QSqlTableModel *kafedryiModel;
};

#endif // KAFEDRY_H
