#ifndef STIP_TYPE_H
#define STIP_TYPE_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Stip_type;
}

class Stip_type : public QDialog
{
    Q_OBJECT

public:
    explicit Stip_type(QWidget *parent = 0);
    ~Stip_type();
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
    Ui::Stip_type *ui;
    QSqlTableModel *stip_typeModel;
};

#endif // Stip_type_H
