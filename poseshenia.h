#ifndef POSESHENIA_H
#define POSESHENIA_H

#include <QDialog>

namespace Ui {
class Poseshenia;
}

class Poseshenia : public QDialog
{
    Q_OBJECT

public:
    explicit Poseshenia(QWidget *parent = 0);
    ~Poseshenia();

private:
    Ui::Poseshenia *ui;
};

#endif // POSESHENIA_H
