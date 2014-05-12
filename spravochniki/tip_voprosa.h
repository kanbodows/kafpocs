#ifndef TIP_VOPROSA_H
#define TIP_VOPROSA_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Tip_voprosa;
}

class Tip_voprosa : public QDialog
{
    Q_OBJECT

public:
    explicit Tip_voprosa(QWidget *parent = 0);
    ~Tip_voprosa();
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
    Ui::Tip_voprosa *ui;
    QSqlTableModel *tip_voprosaModel;
};

#endif // Tip_voprosa_H
