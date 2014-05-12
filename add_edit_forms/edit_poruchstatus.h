 #ifndef EDIT_PORUCHSTATUS_H
#define EDIT_PORUCHSTATUS_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QDoubleValidator>

namespace Ui {
class Edit_PoruchStatus;
}

class Edit_PoruchStatus : public QDialog
{
    Q_OBJECT

public:
    explicit Edit_PoruchStatus(QWidget *parent = 0, bool isEdit  = false, int id_porucheniya = -1);
    ~Edit_PoruchStatus();
    bool isEdit;
    int id_porucheniya;

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_prinyat_clicked();

    void on_pushButton_otkl_clicked();

    void on_pushButton_openFile_clicked();

    void on_pushButton_wait_clicked();

private:
    Ui::Edit_PoruchStatus *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
};

#endif // EDIT_PORUCHSTATUS_H
