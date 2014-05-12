#ifndef READBUTTON_H
#define READBUTTON_H

#include <QPushButton>

class ReadButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ReadButton(QWidget *parent = 0);
    bool isPoruchitel;
    int id_porucheniya;
    int status_id;
//    void setButtonIcon(int frame);
signals:

public slots:

};

#endif // READBUTTON_H
