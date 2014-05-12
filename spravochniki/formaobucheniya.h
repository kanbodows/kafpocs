#ifndef FORMAOBUCHENIYA_H
#define FORMAOBUCHENIYA_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class FormaObucheniya;
}

class FormaObucheniya : public QDialog
{
    Q_OBJECT

public:
    explicit FormaObucheniya(QWidget *parent = 0);
    ~FormaObucheniya();
    bool static isOpen;
    bool checkRelation(QString id_formObuch);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_dell_clicked();

private:
    Ui::FormaObucheniya *ui;
    QSqlTableModel *formaObucheniyaModel;
};

#endif // FORMAOBUCHENIYA_H
