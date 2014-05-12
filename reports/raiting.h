#ifndef RAITING_H
#define RAITING_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>

namespace Ui {
class raiting;
}

class Raiting : public QDialog
{
    Q_OBJECT
public:
    explicit Raiting(QWidget *parent = 0);
    ~Raiting();    
    bool static isOpen;
private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_clicked();

private:
    Ui::raiting *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QTextDocument* body;
};

#endif // RAITING_H
