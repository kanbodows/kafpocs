#include "raiting.h"
#include "ui_raiting.h"

bool Raiting::isOpen = false;
Raiting::Raiting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::raiting)
{
    ui->setupUi(this);
    if (! isOpen)
    {
        isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();

    }
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        this->isOpen = false;
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->dateEdit_endReport->setDate(QDate::currentDate());
    ui->dateEdit_startReport->setDisplayFormat("MMMM yyyy");
    ui->dateEdit_endReport->setDisplayFormat("MMMM yyyy");
    ui->comboBox_doljnost->setModel(dal_prepodcontrol->getDoljnost());
    ui->comboBox_doljnost->setModelColumn(1);
    ui->comboBox_doljnost->setCurrentIndex(-1);
}

Raiting::~Raiting()
{
    isOpen = false;
    delete ui;
}

void Raiting::printDocument(QPrinter *printer)
{
    body->print(printer);
}

void Raiting::on_pushButton_showReport_clicked()
{
    if(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt()==0)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Выберите должность"));
        return;
    }
    MyPrint myPrint;
    RowList list;
    list[1] = tr("Сотрудник");
    list[2] = tr("Награды");
    list[3] = tr("Кур. раб.");
    list[4] = tr("Методички");
    list[5] = tr("Повыш. квал.");
    list[6] = tr("Публикации");
    list[7] = tr("Книги");
    list[8] = tr("Проекты");
    list[9] = tr("Итого");
    body = myPrint.print(dal_prepodcontrol->getRaiting(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt(),
                                                       ui->dateEdit_startReport->date(),
                                                       ui->dateEdit_endReport->date()),
                         list, this->windowTitle(),"", 0);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Widget);
    QList<QPrintPreviewWidget*> list2 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list2.isEmpty()) // paranoiac safety check
        list2.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    QLayoutItem* item;
    while ((item = ui->gridLayout_4->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    ui->gridLayout_4->addWidget(preview);
    preview->exec();
}



void Raiting::on_pushButton_clicked()
{
    if(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt()==0)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Выберите должность"));
        return;
    }
    QLayoutItem* item;
    while ((item = ui->gridLayout_4->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    /*QDialog* diag = new QDialog(this);
    diag->setWindowFlags(Qt::Window);
    diag->setWindowTitle("Рейтинг");
    QGridLayout* diagL = new QGridLayout();
    diag->setLayout(diagL);
    QScrollArea* scroll = new QScrollArea();
    diagL->addWidget(scroll);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QGridLayout* scrollL = new QGridLayout();
    scroll->setLayout(scrollL);
    QWidget* cont = new QWidget();
    scrollL->addWidget(cont);
    QGridLayout* mainL = new QGridLayout();
    cont->setLayout(mainL);*/
    QSqlQueryModel* model = dal_prepodcontrol->getRaiting(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt(),
                                                          ui->dateEdit_startReport->date(),
                                                          ui->dateEdit_endReport->date());
    int katCount = 7;
//    QSequentialAnimationGroup *anim = new QSequentialAnimationGroup;
//    anim->setLoopCount(3);
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QGroupBox* prepodBox = new QGroupBox();
        prepodBox->setMaximumSize(800,200);
        prepodBox->setMinimumSize(800,200);
        if(row==0)
            prepodBox->setStyleSheet("QGroupBox{background-color:red;}");
        else if(row == 1)
            prepodBox->setStyleSheet("QGroupBox{background-color:orange;}");
        else if(row == 2)
            prepodBox->setStyleSheet("QGroupBox{background-color:yellow;}");
        else
            prepodBox->setStyleSheet("QGroupBox{background-color:#fff;}");
        QGraphicsDropShadowEffect * effect =  new QGraphicsDropShadowEffect; // эффект тени
        effect->setBlurRadius(20);
        prepodBox->setGraphicsEffect(effect);
//        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(prepodBox);
//        prepodBox->setGraphicsEffect(fade_effect);
//        QPropertyAnimation *animation = new QPropertyAnimation(prepodBox, "opacity");
//        animation->setEasingCurve(QEasingCurve::InOutQuad);
//        animation->setDuration(100);
//        animation->setStartValue(0.01);
//        animation->setEndValue(1.0);
//        anim->addAnimation(animation);
//        QPropertyAnimation *animation = new QPropertyAnimation(prepodBox, "geometry");
//        animation->setEasingCurve(QEasingCurve::InOutQuad);
//        animation->setDuration(400);
//        animation->setStartValue(QRect(0, 208*(row)+8, 20, 20));
//        int left = ui->scrollAreaWidgetContents->geometry().width() - 600;
//        animation->setEndValue(QRect(left, 208*(row)+8, 600, 200));
//        anim->addAnimation(animation);
        QGridLayout* boxLayout = new QGridLayout(prepodBox);
        prepodBox->setLayout(boxLayout);
        QLabel *labelPrepod = new QLabel(model->record(row).value(1).toString());
        labelPrepod->setStyleSheet("font-size:25px; color:black;");
        labelPrepod->setAlignment(Qt::AlignCenter);
        QGroupBox* pointBox = new QGroupBox();
        pointBox->setStyleSheet("QGroupBox{background-color:transparent; border:4px solid black;border-radius:8px; max-width:140px;min-width:140px;margin-right:15px;margin-left:45px;}");
        QGridLayout* pointLayout = new QGridLayout(pointBox);
        pointBox->setLayout(pointLayout);
        QLabel *labelPoint = new QLabel(model->record(row).value(9).toString());
        labelPoint->setStyleSheet("font-size:100px; color:green;");
        pointLayout->addWidget(labelPoint,0,0,0,0,Qt::AlignCenter);
        //        labelPoint->setAlignment(Qt::AlignCenter);
        QLabel *label1 = new QLabel("Награды:");
        label1->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label2 = new QLabel("Кур. раб.:");
        label2->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label3 = new QLabel("Методички:");
        label3->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label4 = new QLabel("Повыш. квал.:");
        label4->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label5 = new QLabel("Публикации:");
        label5->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label6 = new QLabel("Книги:");
        label6->setStyleSheet("font-size:20px; color:#8470FF;");
        QLabel *label7 = new QLabel("Проекты:");
        label7->setStyleSheet("font-size:20px; color:#8470FF;");
        boxLayout->addWidget(labelPrepod, 0, 0);
        boxLayout->addWidget(pointBox, 1, 0, katCount-1, 1);
        boxLayout->addWidget(label1, 0, 1);
        boxLayout->addWidget(label2, 1, 1);
        boxLayout->addWidget(label3, 2, 1);
        boxLayout->addWidget(label4, 3, 1);
        boxLayout->addWidget(label5, 4, 1);
        boxLayout->addWidget(label6, 5, 1);
        boxLayout->addWidget(label7, 6, 1);

        for (int p = 0; p < katCount; ++p)
        {
            QLabel* labelCount = new QLabel(model->record(row).value(p + 2).toString());
            labelCount->setStyleSheet("font-size:25px; color:#fff;");
            boxLayout->addWidget(labelCount, p, 2);
            for (int i = 0; i < model->record(row).value(p + 2).toInt(); ++i)
            {
                QLabel* lbStar= new QLabel;
                QMovie* starMovie = new QMovie(":/img/img/star (2).png");
                starMovie->setScaledSize(QSize(20,20));
                lbStar->setMovie(starMovie);
                starMovie->start();
                boxLayout->addWidget(lbStar, p, 3 + i);
            }
        }
        QLabel *labelNum;

        if(row==0)
        {
            labelNum = new QLabel;
            QMovie* cupMovie = new QMovie(":/img/img/cup1.png");
            cupMovie->setScaledSize(QSize(180,180));
            labelNum->setMovie(cupMovie);
            cupMovie->start();
        }
        else if(row == 1)
        {
            labelNum = new QLabel;
            QMovie* cupMovie = new QMovie(":/img/img/cup2.png");
            cupMovie->setScaledSize(QSize(180,180));
            labelNum->setMovie(cupMovie);
            cupMovie->start();
        }
        else if(row == 2)
        {
            labelNum = new QLabel;
            QMovie* cupMovie = new QMovie(":/img/img/cup3.png");
            cupMovie->setScaledSize(QSize(180,180));
            labelNum->setMovie(cupMovie);
            cupMovie->start();

        }
        else
            labelNum = new QLabel(QString::number(row+1));
        labelNum->setStyleSheet("font-size:70px; color:#8470FF;");
        QLabel* lbAva= new QLabel;
        QMovie* progressMovie = new QMovie(":/img/img/avatar.png");
        progressMovie->setScaledSize(QSize(180,180));
        lbAva->setMovie(progressMovie);
        progressMovie->start();
        ui->gridLayout_4->addWidget(labelNum, row, 0, 1, 1, Qt::AlignCenter);
        ui->gridLayout_4->addWidget(lbAva, row, 1, 1, 1, Qt::AlignCenter);
        ui->gridLayout_4->addWidget(prepodBox, row, 2, 1, 1, Qt::AlignCenter);
        //    mainL->addWidget(labelNum, row, 0, 1, 1, Qt::AlignCenter);
        //    mainL->addWidget(lbAva, row, 1, 1, 1, Qt::AlignCenter);
        //    mainL->addWidget(prepodBox, row, 2, 1, 1, Qt::AlignCenter);
    }
    //    anim->start();
    //ndiag->showMaximized();
}
