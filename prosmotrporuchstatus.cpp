#include "prosmotrporuchstatus.h"
#include "ui_prosmotrporuchstatus.h"
#include <QLabel>
#include "delegates/readbutton.h"

bool prosmotrPoruchStatus::isOpen = false;
prosmotrPoruchStatus::prosmotrPoruchStatus(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::prosmotrPoruchStatus)
{
    ui->setupUi(this);
    if (!isOpen)
    {
        isOpen = true;
    }
    else
    {
        close();
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    showMessages();
}

prosmotrPoruchStatus::~prosmotrPoruchStatus()
{
    isOpen = false;
    delete ui;
}

void prosmotrPoruchStatus::on_messageReadBtn_clicked()
{
    // смена статуса
    ReadButton *btn = (ReadButton*)sender();
    QSqlQuery *q = new QSqlQuery();

    if(btn->status_id == 3 || btn->status_id == 5 || btn->status_id == 2) // если нажал "просмотрено"
        q->prepare("UPDATE is_porucheniya SET checkPoruch = 1, checkIspoln = 1 WHERE id_poruchenie = " + QString::number(btn->id_porucheniya));
    else
    {
        if(btn->isPoruchitel) // если он назначал
        {
            if(btn->status_id == 4)// если поручитель нажал на "проверено"
                q->prepare("UPDATE is_porucheniya SET checkPoruch = 1, checkIspoln = 0, status_id = 5 WHERE id_poruchenie = " + QString::number(btn->id_porucheniya));
        }
        else
        {
            if(btn->status_id == 1) // при принятии поручения
            {
                if(btn->text()=="Принять") // если исполнитель нажал на "принято"
                    q->prepare("UPDATE is_porucheniya SET checkPoruch = 0, checkIspoln = 1, status_id = 2 WHERE id_poruchenie = " + QString::number(btn->id_porucheniya));
                else // если исполнитель нажал на "отклонить"
                    q->prepare("UPDATE is_porucheniya SET checkPoruch = 0, checkIspoln = 1, status_id = 3 WHERE id_poruchenie = " + QString::number(btn->id_porucheniya));
            }
        }
    }
    q->exec();
    qDebug()<<q->lastError()<<q->executedQuery();
    showMessages();
//    btn->setEnabled(false);
}

void prosmotrPoruchStatus::showMessages()
{
    if ( ui->scrollAreaWidgetContents->layout() != NULL ) // очистка кнопок
    {
        QLayoutItem* item;
        while ((item = ui->scrollAreaWidgetContents->layout()->takeAt(0) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete ui->scrollAreaWidgetContents->layout();
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    // генерация
    int oo = 0;
    QGridLayout *gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(gridLayout);
    queryNaznPoruch = new QSqlQuery();
    queryNaznPoruch->prepare("SELECT * FROM Poruchenias_view where checkPoruch = 0 and poruchitel = " + QString::number(dal_main->getCurrentEmployee()));
    queryNaznPoruch->exec();
    queryNaznPoruch->first();
    for (int row = 0; row < queryNaznPoruch->size(); row++) // проход по всем записям
    {
        QGroupBox *messageGroupBox = new QGroupBox();
        QGridLayout *messageLayout = new QGridLayout(messageGroupBox);
        messageGroupBox->setLayout(messageLayout);
        QGraphicsDropShadowEffect * ef =  new QGraphicsDropShadowEffect; // эффект тени
        ef->setBlurRadius(20);
        messageGroupBox->setGraphicsEffect(ef);
        messageGroupBox->setContentsMargins(3,3,3,3);
        QLabel *label = new QLabel("Назначенное поручение: " + queryNaznPoruch->value(5).toString()
                                   + "\nСменен статус на: " + queryNaznPoruch->value(10).toString()
                                   + "\nИсполнитель: " + queryNaznPoruch->value(9).toString());
        ReadButton* messageReadBtn = new ReadButton();
        messageReadBtn->id_porucheniya = queryNaznPoruch->value(0).toInt();
        messageReadBtn->isPoruchitel = true;
        messageReadBtn->status_id = queryNaznPoruch->value(4).toInt();
        if(messageReadBtn->status_id == 2 || messageReadBtn->status_id == 3)
            messageReadBtn->setText("Просмотрено");
        else if(messageReadBtn->status_id == 4)
            messageReadBtn->setText("Проверено");
        messageReadBtn->setIcon(QIcon(":/img/img/33291.png"));
        connect(messageReadBtn,SIGNAL(clicked()),SLOT(on_messageReadBtn_clicked()));
        messageLayout->addWidget(label,0,0);
        messageLayout->addWidget(messageReadBtn,0,1);
        gridLayout->addWidget(messageGroupBox,row,0);
        queryNaznPoruch->next();
        oo ++;
    }

    queryIspolPoruch = new QSqlQuery();
    queryIspolPoruch->prepare("SELECT * FROM Poruchenias_view where checkIspoln = 0 and ispolnitel = " + QString::number(dal_main->getCurrentEmployee()));
    queryIspolPoruch->exec();
    queryIspolPoruch->first();
    for (int row = 0; row < queryIspolPoruch->size(); row++) // проход по всем записям
    {
        QGroupBox *messageGroupBox = new QGroupBox();
        messageGroupBox->setStyleSheet("QGroupBox{background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.482955 rgba(39, 74, 157, 255), stop:1 rgba(126, 154, 255, 255));color:yellow;}QLabel{color:white;}");
        QGridLayout *messageLayout = new QGridLayout(messageGroupBox);
        messageGroupBox->setLayout(messageLayout);
        QGraphicsDropShadowEffect * ef =  new QGraphicsDropShadowEffect;
        ef->setBlurRadius(20);
        messageGroupBox->setGraphicsEffect(ef);
        messageLayout->setContentsMargins(8,20,8,8);
        QLabel *label = new QLabel("Вам назначено поручение: " + queryIspolPoruch->value(5).toString()
                                   + " \nДата сдачи: " + queryIspolPoruch->value(11).toDate().toString("dd-MM-yyyy")
                                   + " \nБригадир: " + queryIspolPoruch->value(8).toString());
        ReadButton* messageReadBtn = new ReadButton(); // кнопка принять или просморено
        messageReadBtn->id_porucheniya = queryIspolPoruch->value(0).toInt();
        messageReadBtn->isPoruchitel = false;
        messageReadBtn->status_id = queryIspolPoruch->value(4).toInt();
        if(messageReadBtn->status_id == 5)
            messageReadBtn->setText("Просмотрено");
        messageReadBtn->setIcon(QIcon(":/img/img/33291.png"));
        connect(messageReadBtn,SIGNAL(clicked()),SLOT(on_messageReadBtn_clicked()));
        messageLayout->addWidget(label,0,0);
        messageLayout->addWidget(messageReadBtn,0,1);
        if(messageReadBtn->status_id == 1)
        {
            messageGroupBox->setTitle("Новое поручение");
            messageReadBtn->setText("Принять");            
            messageReadBtn->setIcon(QIcon(":/img/img/535786.png"));
            // кнопка отклонить
            ReadButton* messageReadBtnNot = new ReadButton();
            messageReadBtnNot->id_porucheniya = queryIspolPoruch->value(0).toInt();
            messageReadBtnNot->isPoruchitel = false;
            messageReadBtnNot->status_id = messageReadBtn->status_id;
            messageReadBtnNot->setText("Отклонить");
            messageReadBtnNot->setIcon(QIcon(":/img/img/3996.png"));
            connect(messageReadBtnNot,SIGNAL(clicked()),SLOT(on_messageReadBtn_clicked()));
            messageLayout->addWidget(messageReadBtnNot,0,2);
            // кнопка Подробно
            ReadButton* messageDetailBtn = new ReadButton();
            messageDetailBtn->id_porucheniya = queryIspolPoruch->value(0).toInt();
            messageDetailBtn->isPoruchitel = false;
            messageDetailBtn->status_id = messageReadBtn->status_id;
            messageDetailBtn->setText("Детально");
            messageDetailBtn->setIcon(QIcon(":/img/img/43146.png"));
            connect(messageDetailBtn,SIGNAL(clicked()),SLOT(on_messageDetailBtn_clicked()));
            messageLayout->addWidget(messageDetailBtn,1,0);
        }
        gridLayout->addWidget(messageGroupBox,row + oo,0);
        queryIspolPoruch->next();
    }
    this->adjustSize();
    if(queryNaznPoruch->size()<=0 && queryIspolPoruch->size()<=0)
        close();
}

void prosmotrPoruchStatus::on_messageDetailBtn_clicked()
{
    ReadButton *btn = (ReadButton*)sender();
    try
    {
        Edit_PoruchStatus* form = new Edit_PoruchStatus(this,true,btn->id_porucheniya);
        form->exec();
        showMessages();
    }
    catch (...)
    {
        return;
    }
}

void prosmotrPoruchStatus::on_pushButton_close_clicked()
{
    this->close();
}

void prosmotrPoruchStatus::on_pushButton_poruch_clicked()
{
    try
    {
        Poruchenia* form = new Poruchenia();
        this->close();
        form->exec();
    }
    catch (...)
    {
        return;
    }
}
