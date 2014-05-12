#ifndef SMENAPARROLYA_H
#define SMENAPARROLYA_H

#include <QDialog>
#include "dals/dal_prepodcontrol.h"
#include "styles.h"

namespace Ui {
class SmenaParrolya;
}

class SmenaParrolya : public QDialog
{
    Q_OBJECT

public:
    explicit SmenaParrolya(QWidget *parent = 0, int rec=-1);
    ~SmenaParrolya();

private slots:
    void on_pushButton_ok_clicked();

    void on_lineEdit_StarParol_editingFinished();

    void on_lineEdit_VvodNovogoParol_editingFinished();

    void on_lineEdit_PodtverParpl_editingFinished();

    void on_pushButton_cancel_clicked();

private:
    Ui::SmenaParrolya *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    int record_id;
    Styles *css;
};

#endif // SMENAPARROLYA_H
