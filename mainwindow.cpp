#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSystemTrayIcon>
#include <QSplashScreen>
#include <QPixmap>
#include "groups.h"
#include "qdebug.h"
#include <delegates/qlightboxwidget.h>
#include <QLabel>
#include <QMovie>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QDebug>

QSystemTrayIcon *icon;
QEventLoop loop;
QTimer *checker;
DAL_main *dal_main;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dal_main = new DAL_main(this);
    QCoreApplication::setApplicationName("pocskaf2");
    QCoreApplication::setOrganizationName("team2");
    QSettings prevSettings("team2", "pocskaf2");
    if (prevSettings.value("databaseName").isNull() || prevSettings.value("port").isNull() || prevSettings.value("username").isNull() || prevSettings.value("password").isNull() || prevSettings.value("host").isNull())
    {
        accessdb *access = new accessdb(this);
        access->marker = 1;
        access->exec();
    }
    else
    {
        if( dal_main->setConnection(prevSettings.value("databaseName").toString(),prevSettings.value("port").toInt(),prevSettings.value("host").toString(),prevSettings.value("username").toString(),prevSettings.value("password").toString()))
        {
            QMessageBox::information(this, tr("Соединение успешно"), tr("Соединение с базой данных установлено"));
        }
        else
        {
            QMessageBox::critical(this, tr("Ошибка подключения"), tr("Соединение с базой данных невозможно"));
            accessdb *access = new accessdb(this);
            access->marker = 1;
            access->exec();
        }
    }
    Authorization *w=new Authorization(this) ;
    w->setWindowFlags(Qt::Dialog | Qt::Desktop);
    w->exec();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    showTime();

    QPixmap pixmap(":/img/img/pocs2.png");
    QSplashScreen *splash = new QSplashScreen(pixmap);
    QFont splashFont;
    splashFont.setPixelSize(17);
    splash->setFont(splashFont);
    splash->show();
    QTime time;
    time.start();
    for (int i = 0; i < 100; )
    {
        if (time.elapsed() > 5)
        {
            time.start();
            ++i;
        }
        splash->showMessage(tr("     Загрузка системы: ") + QString::number(i) + "%", Qt::AlignLeft | Qt::AlignBottom , Qt::black);
        QCoreApplication::processEvents();
    }

    QSqlQuery * empNameQuery = new QSqlQuery(dal_main->db);
    empNameQuery->prepare("SELECT st_fio, st_photo FROM is_sotrudniki where id_sotr = " + QString::number(dal_main->getCurrentEmployee()));
    empNameQuery->exec();
    empNameQuery->first();
    if(empNameQuery->isValid())
        this->setWindowTitle("SMT v. 1.0 (текущий пользователь - " + empNameQuery->value(0).toString() + ")");


    checkNaznPoruch = new QSqlQuery(dal_main->db);
    checkNaznPoruch->prepare("SELECT count(id_poruchenie) FROM is_porucheniya where  checkPoruch = 0 and poruchitel = " + QString::number(dal_main->getCurrentEmployee()));
    checkIspolPoruch = new QSqlQuery(dal_main->db);
    checkIspolPoruch->prepare("SELECT count(id_poruchenie) FROM is_porucheniya where  checkIspoln = 0 and ispolnitel = " + QString::number(dal_main->getCurrentEmployee()));
    checker = new QTimer;
    //        checkDatePoruch = new QSqlQuery(dal_main->db);
    //        checkDatePoruch->prepare("SELECT count(id_poruchenie) FROM is_porucheniya where check_date  and checkIspoln = 1 and ispolnitel = " + QString::number(dal_main->getCurrentEmployee()));
    connect(checker,SIGNAL(timeout()),this,SLOT(on_check()));
    checker->setInterval(10000);
    checker->start();
    icon = new QSystemTrayIcon(this);
    icon->setIcon(QIcon(":/img/img/institution_icon.png"));
    icon->show();
    //icon->setContextMenu(ui->menu_file);
    connect(icon,SIGNAL(messageClicked()),this,SLOT(on_actionChecks())); //Open form Request

    ui->pushButton_show->setVisible(false);
    ui->pushButton_show->setFlat(true);
    ui->pushButton_hide->setFlat(true);
    ui->groupBox_main->setContentsMargins(0,0,0,0);
    ui->mdiArea->setVisible(false);
    QGraphicsDropShadowEffect * ef1 =  new QGraphicsDropShadowEffect;
    ef1->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef2 =  new QGraphicsDropShadowEffect;
    ef2->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef3 =  new QGraphicsDropShadowEffect;
    ef3->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef4 =  new QGraphicsDropShadowEffect;
    ef4->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef5 =  new QGraphicsDropShadowEffect;
    ef5->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef6 =  new QGraphicsDropShadowEffect;
    ef6->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef7 =  new QGraphicsDropShadowEffect;
    ef7->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef8 =  new QGraphicsDropShadowEffect;
    ef8->setBlurRadius(15);
    QGraphicsDropShadowEffect * ef9 =  new QGraphicsDropShadowEffect;
    ef9->setBlurRadius(15);
    ui->pushButton1->setGraphicsEffect(ef1);
    ui->pushButton2->setGraphicsEffect(ef2);
    ui->pushButton3->setGraphicsEffect(ef3);
    ui->pushButton4->setGraphicsEffect(ef4);
    ui->pushButton5->setGraphicsEffect(ef5);
    ui->pushButton6->setGraphicsEffect(ef6);
    ui->pushButton7->setGraphicsEffect(ef7);
    ui->pushButton8->setGraphicsEffect(ef8);
    ui->pushButton9->setGraphicsEffect(ef9);
    //    this->setCentralWidget(ui->mdiArea);

    // Настраиваем виджет перекрытия
    QLightBoxWidget* lightBox = new QLightBoxWidget(this);
    QLabel* lbTitle = new QLabel(empNameQuery->value(0).toString());
    lbTitle->setStyleSheet("font-size: 28px; font-weight: bold; color: white");
    QLabel* lbProgress = new QLabel;
    QMovie* progressMovie = new QMovie(empNameQuery->value(1).toString());
    progressMovie->setScaledSize(QSize(250,200));
    lbProgress->setMovie(progressMovie);
    progressMovie->start();
    QLabel* lbDescription = new QLabel(tr("Для входа в систему нажмите Войти"));
    lbDescription->setStyleSheet("color: white");
    QPushButton* lbClose = new QPushButton(tr("Войти"));
    QGridLayout* lbLayout = new QGridLayout;
    lbLayout->setRowStretch(0, 1);
    lbLayout->setColumnStretch(0, 1);
    lbLayout->addWidget(lbTitle, 1, 1);
    lbLayout->addWidget(lbProgress, 1, 2, Qt::AlignRight);
    lbLayout->setColumnStretch(3, 1);
    lbLayout->addWidget(lbDescription, 2, 1, 1, 2);
    lbLayout->addWidget(lbClose, 3, 2);
    lbLayout->setRowStretch(4, 1);
    connect(ui->action_Oteshel, SIGNAL(triggered()), lightBox, SLOT(show()));
    connect(lbClose, SIGNAL(clicked()), lightBox, SLOT(hide()));
    lightBox->setLayout(lbLayout);
//    ui->menuBar->setVisible(false);
    ui->mainToolBar->setVisible(false);
    ui->groupBox->setVisible(false);
    splash->finish(this);

    /*QGridLayout *userl = new QGridLayout(ui->userWidget);
    ui->userWidget->setLayout(userl);
    QLabel* lbProgress1 = new QLabel;
    QMovie* progressMovie1 = new QMovie(empNameQuery->value(1).toString());
    progressMovie1->setScaledSize(QSize(250,200));
    lbProgress1->setMovie(progressMovie1);
    progressMovie1->start();
    userl->addWidget(lbProgress1);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
       QString text = time.toString("hh:mm");
   if ((time.second() % 2) == 0)
       text[2] = ' ';
    ui->lcdNumber->display(text);

}

void MainWindow::openPorucheniya()
{
    Poruchenia *Porucheniaform;
    try
    {
        Porucheniaform = new Poruchenia(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Porucheniaform);
    Porucheniaform->showMaximized();
    on_pushButton_hide_clicked();
}


void MainWindow::openProsmotr()
{
    prosmotrPoruchStatus *form;
    try
    {
        form = new prosmotrPoruchStatus(this);
        form->exec();
    }
    catch(...)
    {
        return;
    }
}


/*void MainWindow::on_action_triggered()//форма курсы
{
    Course *CourseForm;
    try
    {
        CourseForm = new Course(this);
    }
    catch (...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(CourseForm);
    CourseForm->showMaximized();
    on_pushButton_hide_clicked();

}*/

void MainWindow::on_action_podkl_triggered()
{
    accessdb *accessdbForm;
    try
    {
        accessdbForm = new accessdb(this);
    }
    catch(...)
    {
        return;
    }
    accessdbForm->show();
}



void MainWindow::on_action_semestr_triggered()
{
    Semestr *semestrForm;
    try
    {
        semestrForm = new Semestr(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(semestrForm);
    semestrForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_discipline_triggered()
{
    Discipline *disciplineForm;
    try
    {
        disciplineForm=new Discipline(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(disciplineForm);
    disciplineForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_exit_triggered()
{
    exit(1);
}

void MainWindow::on_action_group_triggered()
{
    groups *form;
    try
    {
        form = new groups(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();

}

/*void MainWindow::on_action_form_obuch_triggered()
{
    FormaObucheniya *FormaObucheniyaForm;
    try
    {
        FormaObucheniyaForm=new FormaObucheniya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(FormaObucheniyaForm);
    FormaObucheniyaForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_form_opl_triggered()
{
    FormOplaty *FormaOplatiForm;
    try
    {
        FormaOplatiForm = new FormOplaty(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(FormaOplatiForm);
    FormaOplatiForm->showMaximized();
    on_pushButton_hide_clicked();
}
*/
void MainWindow::on_action_tema_diplomnoi_triggered()
{
    ThemeDiplomProekta *ThemeDiplomProektaForm;
    try
    {
        ThemeDiplomProektaForm = new ThemeDiplomProekta(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(ThemeDiplomProektaForm);
    ThemeDiplomProektaForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_doljnosti_triggered()
{

    Doljnost *doljnostForm;
    try
    {
        doljnostForm = new Doljnost(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(doljnostForm);
    doljnostForm->showMaximized();
    on_pushButton_hide_clicked();

}

void MainWindow::on_action_konf_sem_triggered()
{
    Konf_sem *form;
    try
    {
        form = new Konf_sem(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_kur_rab_triggered()
{
    Kur_rab *form;
    try
    {
        form = new Kur_rab(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

/*void MainWindow::on_action_lic_doc_types_triggered()
{
    Lic_doc_types *form;
    try
    {
        form = new Lic_doc_types(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_lic_types_triggered()
{
    Lic_types *form;
    try
    {
        form = new Lic_types(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

void MainWindow::on_action_poruch_status_triggered()
{
    Poruch_status *form;
    try
    {
        form = new Poruch_status(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_zvaniya_triggered()
{
    Zvaniya *form;
    try
    {
        form = new Zvaniya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

/*void MainWindow::on_action_voenkomat_triggered()
{
    Voenkomat *form;
    try
    {
        form = new Voenkomat(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

/*void MainWindow::on_action_tip_voprosa_triggered()
{
    Tip_voprosa *form;
    try
    {
        form = new Tip_voprosa(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

/*void MainWindow::on_action_prize_types_triggered()
{
    Prize_types *form;
    try
    {
        form = new Prize_types(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

/*void MainWindow::on_action_stip_type_triggered()
{
    Stip_type *form;
    try
    {
        form = new Stip_type(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

void MainWindow::on_action_prac_type_triggered()
{
    Prac_type *form;
    try
    {
        form = new Prac_type(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_lic_docs_triggered()
{
    Lic_docs *form;
    try
    {
        form = new Lic_docs(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_sport_triggered()
{
    IsSport *issportForm;
    try
    {
        issportForm = new IsSport(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(issportForm);
    issportForm->showMaximized();
    on_pushButton_hide_clicked();;
}
/*
void MainWindow::on_action_main_journal_triggered()
{
    Main_journal *form;
    try
    {
        form = new Main_journal(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_met_types_triggered()
{
    Met_types *form;
    try
    {
        form = new Met_types(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/

void MainWindow::on_action_metodichki_triggered()
{
    Metodichki *form;
    try
    {
        form = new Metodichki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_studenti_triggered()
{
    Student *form;
    try
    {
        form = new Student(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}
/*
void MainWindow::on_action_vipuskniki_triggered()
{
    Vipuskniki *Vipusknikiform;
    try
    {
        Vipusknikiform = new Vipuskniki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Vipusknikiform);
    Vipusknikiform->showMaximized();
    on_pushButton_hide_clicked();
}
*/
void MainWindow::on_action_stipend_triggered()
{
    Stipendiya *Stipendiyaform;
    try
    {
        Stipendiyaform = new Stipendiya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Stipendiyaform);
    Stipendiyaform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_statia_triggered()
{
    Statiya *Statiyaform;
    try
    {
        Statiyaform = new Statiya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Statiyaform);
    Statiyaform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_porucheniya_triggered()
{
    Poruchenia *Porucheniaform;
    try
    {
        Porucheniaform = new Poruchenia(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Porucheniaform);
    Porucheniaform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_olimp_triggered()
{
    Olimpiada *Olimpiadaform;
    try
    {
        Olimpiadaform = new Olimpiada(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Olimpiadaform);
    Olimpiadaform->showMaximized();
    on_pushButton_hide_clicked();
}
/*
void MainWindow::on_action_posesheniya_triggered()
{

}
*/

void MainWindow::on_action_obchejitie_triggered()
{
    Obchejitie *Obchejitieform;
    try
    {
        Obchejitieform = new Obchejitie(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Obchejitieform);
    Obchejitieform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_partners_triggered()
{
    Partners *Partnersform;
    try
    {
        Partnersform = new Partners(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Partnersform);
    Partnersform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_Vakansi_triggered()
{
    Vakansii *Vakansiiform;
    try
    {
        Vakansiiform = new Vakansii(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Vakansiiform);
    Vakansiiform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_otpr_soobsh_triggered()
{
    OtpravkaSoobshVipusk *OtpravkaSoobshVipuskform;
    try
    {
        OtpravkaSoobshVipuskform = new OtpravkaSoobshVipusk(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(OtpravkaSoobshVipuskform);
    OtpravkaSoobshVipuskform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_actVnedr_triggered()
{
    ActVnedrenia *ActVnedreniaform;
    try
    {
        ActVnedreniaform = new ActVnedrenia(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(ActVnedreniaform);
    ActVnedreniaform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_practika_triggered()
{
    PracWork *PracWorkform;
    try
    {
        PracWorkform = new PracWork(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(PracWorkform);
    PracWorkform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_sotrudniki_triggered()
{
    Sotrudniki *Sotrudnikiform;
    try
    {
        Sotrudnikiform = new Sotrudniki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Sotrudnikiform);
    Sotrudnikiform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionKnigi_triggered()
{
    Knigi *knigiForm;
    try
    {
        knigiForm = new Knigi(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(knigiForm);
    knigiForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_nir_sotr_triggered()
{
    NirSotrudnika *NirSotrudnikaForm;
    try
    {
        NirSotrudnikaForm = new NirSotrudnika(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(NirSotrudnikaForm);
    NirSotrudnikaForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionPublicatNir_triggered()
{
    PublicationNir *PublicationNirForm;
    try
    {
        PublicationNirForm = new PublicationNir(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(PublicationNirForm);
    PublicationNirForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionBolezniSotr_triggered()
{
    BolezniSotrudnikov *BolezniSotrudnikovForm;
    try
    {
        BolezniSotrudnikovForm = new BolezniSotrudnikov(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(BolezniSotrudnikovForm);
    BolezniSotrudnikovForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionSponsors_triggered()
{
    Sponsors *SponsorsForm;
    try
    {
        SponsorsForm = new Sponsors(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(SponsorsForm);
    SponsorsForm->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionPovyshKval_triggered()
{
    PovyshKval *form;
    try
    {
        form = new PovyshKval(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionRaiting_triggered()
{
    Raiting *form;
    try
    {
        form = new Raiting(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionNagradi_triggered()
{
    Nagradi *form;
    try
    {
        form = new Nagradi(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionChecks()
{
    prosmotrPoruchStatus *form;
    try
    {
        form = new prosmotrPoruchStatus(this);
        form->exec();
    }
    catch(...)
    {
        return;
    }
}

void MainWindow::on_check()
{
    if( dal_main->checkConnection())
    {
        //        QString msg;
        checkNaznPoruch->exec();
        checkNaznPoruch->first();
        //        if(checkNaznPoruch->value(0).toInt()>0)
        //            msg.append(tr("Всего поручений, где вы поручитель, требующие вашего просмотра: ") + checkNaznPoruch->value(0).toString());
        checkIspolPoruch->exec();
        checkIspolPoruch->first();
        //        if(checkIspolPoruch->value(0).toInt()>0)
        //            msg.append(tr("\nВсего поручений, где вы испонитель, требующие вашего просмотра: ") + checkIspolPoruch->value(0).toString());
        //        if(!msg.isEmpty()) icon->showMessage(tr("Внимание!"),msg);
        //        checkDatePoruch->exec();
        //        checkDatePoruch->first();
        //        if(checkNaznPoruch->value(0).toInt() > 0 || checkIspolPoruch->value(0).toInt() > 0 || checkDatePoruch->value(0).toInt() > 0)
        if(checkNaznPoruch->value(0).toInt() > 0 || checkIspolPoruch->value(0).toInt() > 0)
        {
            uvedomlenie *uved = new uvedomlenie(this);
            uved->setWindowFlags(Qt::Dialog | Qt::Desktop);
            uved->exec();
        }
    }
}

void MainWindow::on_action_komGAK_triggered()
{
    SostavKomissii *SostavKomissiiform;
    try
    {
        SostavKomissiiform = new SostavKomissii(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(SostavKomissiiform);
    SostavKomissiiform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_gak_triggered()
{
    GAK *GAKform;
    try
    {
        GAKform = new GAK(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(GAKform);
    GAKform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_pushButton_show_clicked()
{
    ui->groupBox_menu->setVisible(true);
    ui->pushButton_hide->setVisible(true);
    ui->pushButton_show->setVisible(false);
    ui->mdiArea->setVisible(false);
}

void MainWindow::on_pushButton_hide_clicked()
{
    ui->groupBox_menu->setVisible(false);
    ui->pushButton_hide->setVisible(false);
    ui->pushButton_show->setVisible(true);
    ui->mdiArea->setVisible(true);

}

void MainWindow::on_pushButton1_clicked()
{
    on_action_porucheniya_triggered();
}

void MainWindow::on_pushButton2_clicked()
{
    createMenus(1);// сотрудники
}

void MainWindow::on_pushButton3_clicked()
{
    createMenus(4);// рассылка сообщений
}

void MainWindow::on_pushButton4_clicked()
{
    createMenus(2);// студенты
}

void MainWindow::on_pushButton5_clicked()
{
    //    on_action_partners_triggered();
    createMenus(3);// контрагенты
}

void MainWindow::on_pushButton6_clicked()
{
    on_action_lic_docs_triggered();
}

void MainWindow::on_pushButton7_clicked()
{
    createMenus(5);// отчёты
}

void MainWindow::on_pushButton8_clicked()
{
    createMenus(6);// отчёты
}

void MainWindow::on_pushButton9_clicked()
{
    on_action_podkl_triggered();
}

void MainWindow::createMenus(int parentMenu)
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
    QSqlQuery *query = new QSqlQuery("SELECT * FROM is_menus WHERE menu_parent = " + QString::number(parentMenu), dal_main->db);
    query->exec();
    query->first();
    //    qDebug()<<query->lastError()<<query->size();
    ui->groupBox->setVisible(true);
    ui->scrollArea->setMinimumSize(900, 400);
    //    ui->scrollArea->setMaximumSize(800, 500);
    //    ui->groupBox_menu->setMinimumSize(800, 1000);
    QGridLayout *menuLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(menuLayout);
    int row = 0;
    int column = 0;
    //    QList<QCommandLinkButton*> list;
    //    QSequentialAnimationGroup *anim = new QSequentialAnimationGroup;
    for (int i = 0; i < query->size(); ++i)
    {
        QCommandLinkButton* mbtn = new QCommandLinkButton(query->value(1).toString());
        //        QPushButton* mbtn = new QPushButton(query->value(1).toString());
        mbtn->setText(query->value(1).toString());
        mbtn->setMinimumSize(265,70);
        mbtn->setMaximumSize(265,70);
        mbtn->setIcon(QIcon(":/img/img/" + query->value(2).toString()));
        mbtn->setIconSize(QSize(30,60));
        mbtn->setToolTip(query->value(5).toString());
        QGraphicsDropShadowEffect * ef =  new QGraphicsDropShadowEffect; // эффект тени
        ef->setBlurRadius(20);
        mbtn->setGraphicsEffect(ef);
        connect(mbtn,SIGNAL(clicked()),SLOT(menuClick()));
        //        QGraphicsOpacityEffect* fade_effect = new QGraphicsOpacityEffect(mbtn);
        //        mbtn->setGraphicsEffect(fade_effect);
        //        QPropertyAnimation *animation = new QPropertyAnimation(fade_effect, "opacity");
        //        animation->setEasingCurve(QEasingCurve::InOutQuad);
        //        animation->setDuration(100);
        //        animation->setStartValue(0.01);
        //        animation->setEndValue(1.0);
        //        anim->addAnimation(animation);
        menuLayout->addWidget(mbtn,row,column);
        if(column<2)
            column++;
        else
        {
            column = 0;
            row++;
        }
        query->next();
    }
    //    anim->start();
    //    *555-050-*
    //        *909-0-*
    ui->frame->setVisible(false);
}
void MainWindow::on_action_restart_triggered()
{
    if(QMessageBox::warning(this, tr("Внимание"), tr("Сменить учётную запись?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void MainWindow::on_pushButton_back_clicked()
{
    ui->groupBox->setVisible(false);
    ui->frame->setVisible(true);
}

void MainWindow::menuClick()
{
    QString form = ((QPushButton*)sender())->toolTip();
    if(form=="")
        return;
    else if(form=="Sotrudniki")
        on_action_sotrudniki_triggered();
    else if(form=="Poruchenia")
        on_action_porucheniya_triggered();
    else if(form=="Kur_rab")
        on_action_kur_rab_triggered();
    else if(form=="Lic_docs")
        on_action_lic_docs_triggered();
    else if(form=="Metodichki")
        on_action_metodichki_triggered();
    else if(form=="Statiya")
        on_action_statia_triggered();
    else if(form=="NirSotrudnika")
        on_action_nir_sotr_triggered();
    else if(form=="PublicationNir")
        on_actionPublicatNir_triggered();
    else if(form=="BolezniSotrudnikov")
        on_actionBolezniSotr_triggered();
    else if(form=="PovyshKval")
        on_actionPovyshKval_triggered();
    else if(form=="Nagradi")
        on_actionNagradi_triggered();
    else if(form=="SostavKomissii")
        on_action_komGAK_triggered();
    else if(form=="Raspisanie")
        on_action_raspisanie_triggered();
    else if(form=="Student")
        on_action_studenti_triggered();
    else if(form=="IsSport")
        on_action_sport_triggered();
    else if(form=="Diploms")
        on_action_Diplom_triggered();
    else if(form=="Konf_sem")
        on_action_konf_sem_triggered();
    else if(form=="Stipendiya")
        on_action_stipend_triggered();
    else if(form=="Olimpiada")
        on_action_olimp_triggered();
    else if(form=="Obchejitie")
        on_action_obchejitie_triggered();
    else if(form=="ActVnedrenia")
        on_action_actVnedr_triggered();
    else if(form=="PracWork")
        on_action_practika_triggered();
    else if(form=="GAK")
        on_action_gak_triggered();
    else if(form=="Partners")
        on_action_partners_triggered();
    else if(form=="Vakansii")
        on_action_Vakansi_triggered();
    else if(form=="Sponsors")
        on_actionSponsors_triggered();
    else if(form=="OtpravkaSoobshVipusk")
        on_action_otpr_soobsh_triggered();
    else if(form=="JurnalVzaimoposesheniya")
        on_action_JurnalVzaimopos_triggered();
    else if(form=="Course_work")
        on_action_course_work_triggered();
    else if(form=="Srezy")
        on_action_srezy_triggered();
    else if(form=="OtpravkaSoobsheniaSotrudnikam")
        on_actionSendMessSotr_triggered();
    else if(form=="OtpravkaSoobshStudentam")
        on_action_RassilkaStudent_triggered();
    else if(form=="TrudoustroistvoVipusknikov")
        on_action_trudoVipusk_triggered();
    else if(form=="BolezniStudentov")
        on_action_BolezniStud_triggered();
    else if(form=="Raiting")
        on_actionRaiting_triggered();
    else if(form=="OtchetStarostaGruppi")
        on_action_reportStarostaGruppi_triggered();
    else if(form=="Vipusk_ki")
        on_action_Vipusk_triggered();
    else if(form=="TrudvVipusk")
        on_action_RepTrudoVip_triggered();
    else if(form=="Otchetsrezbally")
        on_action_srez_bally_triggered();
    else if(form=="Semestry")
        on_action_semestr_triggered();
    else if(form=="Disciplines")
        on_action_discipline_triggered();
    else if(form=="Gruppy")
        on_action_group_triggered();
    else if(form=="DiplomThemes")
        on_action_tema_diplomnoi_triggered();
    else if(form=="Doljnosti")
        on_action_doljnosti_triggered();
    else if(form=="Zvaniya")
        on_action_zvaniya_triggered();
    else if(form=="VidyPraktik")
        on_action_prac_type_triggered();
    else if(form=="Knigi")
        on_actionKnigi_triggered();
    else if(form=="OtchetFormaControlya")
        on_action_forma_controlya_triggered();
    else if(form=="OtchetPraktiki")
        on_action_praktiki_triggered();
    else if(form=="OtchetStudenty")
        on_action_otchet_studenty_triggered();
    else if(form=="OtchetSotr")
        on_action_otchet_sotrudniki_triggered();
    else if(form=="OtchetPovyshKval")
        on_action_otchet_povyshKval_triggered();
    else if(form=="OtchetSpravkaPochas")
        on_action_otchet_spravki_pochas_triggered();
    else if(form=="SvodOtchetPraktik")
        on_action_svod_otchet_praktik_triggered();
    else if(form=="GrafikVzaimopos")
        on_action_CreateGrafPosesh_triggered();
    else if(form=="PlanyZased")
        on_action_planZasedKaf_triggered();
    else if(form=="Proekty")
        on_action_proekty_triggered();
    else if(form=="SpravkiPochas")
        on_action_spravki_pochas_triggered();
    else if(form=="PlatnyeKursy")
        on_action_ObuchKursi_triggered();
    else if(form=="VedomostiKurs")
        on_action_vedomosti_kursovye_triggered();
    else if(form=="Специальности")
        on_action_specialnosti_triggered();
    else if(form=="Кафедры")
        on_action_kafedry_triggered();
}

void MainWindow::on_action_raspisanie_triggered()
{
    Raspisanie *form;
    try
    {
        form = new Raspisanie(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_JurnalVzaimopos_triggered()
{
    JurnalVzaimoposesheniya *JurnalVzaimoposesheniyaform;
    try
    {
        JurnalVzaimoposesheniyaform = new JurnalVzaimoposesheniya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(JurnalVzaimoposesheniyaform);
    JurnalVzaimoposesheniyaform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_reportStarostaGruppi_triggered()
{
    OtchetStarostaGruppi *OtchetStarostaGruppiform;
    try
    {
        OtchetStarostaGruppiform = new OtchetStarostaGruppi(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(OtchetStarostaGruppiform);
    OtchetStarostaGruppiform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_course_work_triggered()
{
    Course_work *form;
    try
    {
        form = new Course_work(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_Diplom_triggered()
{
    Diploms *Diplomsform;
    try
    {
        Diplomsform = new Diploms(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Diplomsform);
    Diplomsform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_Vipusk_triggered()
{
    Vipusk_ki *Vipusk_kiform;
    try
    {
        Vipusk_kiform = new Vipusk_ki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(Vipusk_kiform);
    Vipusk_kiform->showMaximized();
    on_pushButton_hide_clicked();
}


void MainWindow::on_action_srezy_triggered()
{
    Srezy *form;
    try
    {
        form = new Srezy(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_trudoVipusk_triggered()
{
    TrudoustroistvoVipusknikov *TrudoustroistvoVipusknikovform;
    try
    {
        TrudoustroistvoVipusknikovform = new TrudoustroistvoVipusknikov(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(TrudoustroistvoVipusknikovform);
    TrudoustroistvoVipusknikovform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_RepTrudoVip_triggered()
{
    OtchetTrudoUstrVip *OtchetTrudoUstrVipform;
    try
    {
        OtchetTrudoUstrVipform = new OtchetTrudoUstrVip(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(OtchetTrudoUstrVipform);
    OtchetTrudoUstrVipform->showMaximized();
    on_pushButton_hide_clicked();
}


/*void MainWindow::on_action_temp_triggered()
{
    Report *form;
    try
    {
        form = new Report(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}*/


void MainWindow::on_action_BolezniStud_triggered()
{
    BolezniStudentov *BolezniStudentovform;
    try
    {
        BolezniStudentovform = new BolezniStudentov(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(BolezniStudentovform);
    BolezniStudentovform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_actionSendMessSotr_triggered()
{
    OtpravkaSoobsheniaSotrudnikam *OtpravkaSoobsheniaSotrudnikamform;
    try
    {
        OtpravkaSoobsheniaSotrudnikamform = new OtpravkaSoobsheniaSotrudnikam(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(OtpravkaSoobsheniaSotrudnikamform);
    OtpravkaSoobsheniaSotrudnikamform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_srez_bally_triggered()
{
    Otchetsrezbally *form;
    try
    {
        form = new Otchetsrezbally(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_RassilkaStudent_triggered()
{
    OtpravkaSoobshStudentam *OtpravkaSoobshStudentamform;
    try
    {
        OtpravkaSoobshStudentamform = new OtpravkaSoobshStudentam(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(OtpravkaSoobshStudentamform);
    OtpravkaSoobshStudentamform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_planZasedKaf_triggered()
{
    GodaSozdPlanaKaf *GodaSozdPlanaKafform;
    try
    {
        GodaSozdPlanaKafform = new GodaSozdPlanaKaf(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(GodaSozdPlanaKafform);
    GodaSozdPlanaKafform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_forma_controlya_triggered()
{
    OtchetFormaControlya *form;
    try
    {
        form = new OtchetFormaControlya(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}


void MainWindow::on_action_praktiki_triggered()
{
    OtchetPraktiki *form;
    try
    {
        form = new OtchetPraktiki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_otchet_studenty_triggered()
{
    OtchetStudenty *form;
    try
    {
        form = new OtchetStudenty(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_CreateGrafPosesh_triggered()
{
    GodaSozdaniaGrafikaVzaiomopos *GodaSozdaniaGrafikaVzaiomoposform;
    try
    {
        GodaSozdaniaGrafikaVzaiomoposform = new GodaSozdaniaGrafikaVzaiomopos(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(GodaSozdaniaGrafikaVzaiomoposform);
    GodaSozdaniaGrafikaVzaiomoposform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_svod_otchet_praktik_triggered()
{
    SvodOtchetPraktiki *form;
    try
    {
        form = new SvodOtchetPraktiki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_proekty_triggered()
{
    Proekty *form;
    try
    {
        form = new Proekty(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_otchet_povyshKval_triggered()
{
    OtchetPovyshKval *form;
    try
    {
        form = new OtchetPovyshKval(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_vedomosti_kursovye_triggered()
{
    VedomostiKursovye *form;
    try
    {
        form = new VedomostiKursovye(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_kafedry_triggered()
{
    Kafedry *form;
    try
    {
        form = new Kafedry(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_specialnosti_triggered()
{
    Specialnosti *form;
    try
    {
        form = new Specialnosti(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_otchet_sotrudniki_triggered()
{
    OtchetSotrudniki *form;
    try
    {
        form = new OtchetSotrudniki(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_spravki_pochas_triggered()
{
    SpravkiPochas *form;
    try
    {
        form = new SpravkiPochas(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_ObuchKursi_triggered()
{
    PlatnieObrazovatUslgi *PlatnieObrazovatUslgiform;
    try
    {
        PlatnieObrazovatUslgiform = new PlatnieObrazovatUslgi(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(PlatnieObrazovatUslgiform);
    PlatnieObrazovatUslgiform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_otchet_spravki_pochas_triggered()
{
    OtchetSpravkiPochas *form;
    try
    {
        form = new OtchetSpravkiPochas(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(form);
    form->showMaximized();
    on_pushButton_hide_clicked();
}


void MainWindow::on_action_PlanUmr_triggered()
{
    PlanUMR *PlanUMRform;
    try
    {
        PlanUMRform = new PlanUMR(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(PlanUMRform);
    PlanUMRform->showMaximized();
    on_pushButton_hide_clicked();
}

void MainWindow::on_action_procentovka_triggered()
{
    procentovka *procentovkaform;
    try
    {
        procentovkaform = new procentovka(this);
    }
    catch(...)
    {
        return;
    }
    ui->mdiArea->addSubWindow(procentovkaform);
    procentovkaform->showMaximized();
    on_pushButton_hide_clicked();
}
