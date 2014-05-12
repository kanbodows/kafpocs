#ifndef PRIZE_TYPES_H
#define PRIZE_TYPES_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Prize_types;
}

class Prize_types : public QDialog
{
    Q_OBJECT

public:
    explicit Prize_types(QWidget *parent = 0);
    ~Prize_types();
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
    Ui::Prize_types *ui;
    QSqlTableModel *prize_typesModel;
};

#endif // Prize_types_H
