#ifndef FORMOPLATY_H
#define FORMOPLATY_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class FormOplaty;
}

class FormOplaty : public QDialog
{
    Q_OBJECT

public:
    explicit FormOplaty(QWidget *parent = 0);
    ~FormOplaty();
    bool static isOpen;
    bool checkRelation(QString id_formOpl);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::FormOplaty *ui;
    QSqlTableModel *formaOplatiModel;
};

#endif // FORMOPLATY_H
