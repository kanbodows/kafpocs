#ifndef RASPISANIE_H
#define RASPISANIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QGridLayout>
#include <QGraphicsDropShadowEffect>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QStringList>
#include <QPlainTextEdit>
#include "add_edit_forms/addoredir_raspisanie.h"
#include "myprint.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>

namespace Ui {
class Raspisanie;
}

class Raspisanie : public QDialog
{
    Q_OBJECT

public:
    explicit Raspisanie(QWidget *parent = 0);
    ~Raspisanie();
    bool static isOpen;
public slots:
    void on_sotr_clicked();

    void createRaspisanie();
private slots:
    void on_pushButton_print_clicked();

    void on_comboBox_den_currentIndexChanged(int index);

    void printDocument(QPrinter *);

private:
    Ui::Raspisanie *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QStringList days;
    QStringList vr;
    QString body;
};

#endif // RASPISANIE_H
