#include "poseshenia.h"
#include "ui_poseshenia.h"

Poseshenia::Poseshenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Poseshenia)
{
    ui->setupUi(this);
}

Poseshenia::~Poseshenia()
{
    delete ui;
}
