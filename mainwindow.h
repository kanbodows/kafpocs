#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QBitmap>
#include <QList>
#include <QCommandLinkButton>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include "dals/dal_main.h"
#include "accessdb.h"
//#include "spravochniki/coursework.h"
#include "spravochniki/semestr.h"
#include "spravochniki/discipline.h"
//#include "spravochniki/formaobucheniya.h"
//#include "spravochniki/formoplaty.h"
#include "themediplomproekta.h"
#include "spravochniki/doljnost.h"
#include "konf_sem.h"
#include "kur_rab.h"
#include "spravochniki/lic_doc_types.h"
//#include "spravochniki/lic_types.h"
#include "spravochniki/poruch_status.h"
#include "spravochniki/zvaniya.h"
//#include "spravochniki/voenkomat.h"
//#include "spravochniki/tip_voprosa.h"
//#include "spravochniki/prize_types.h"
#include "spravochniki/stip_type.h"
#include "spravochniki/prac_type.h"
#include "lic_docs.h"
#include "issport.h"
//#include "main_journal.h"
//#include "spravochniki/met_types.h"
#include "metodichki.h"
#include "student.h"
#include "stipendiya.h"
#include "statiya.h"
#include "poruchenia.h"
#include "olimpiada.h"
#include "pracwork.h"
#include "actvnedrenia.h"
#include "obchejitie.h"
#include "partners.h"
#include "vakansii.h"
#include "otpravkasoobshvipusk.h"
#include "authorization.h"
#include "sotrudniki.h"
#include "knigi.h"
#include "nirsotrudnika.h"
#include "publicationnir.h"
#include "boleznisotrudnikov.h"
#include "sponsors.h"
#include "povyshkval.h"
#include "reports/raiting.h"
#include "nagradi.h"
#include "prosmotrporuchstatus.h"
#include "sostavkomissii.h"
#include "gak.h"
#include "raspisanie.h"
#include "jurnalvzaimoposesheniya.h"
#include "reports/otchetstarostagruppi.h"
#include "course_work.h"
#include "diploms.h"
#include "reports/vipusk_ki.h"
#include "srezy.h"
#include "trudoustroistvovipusknikov.h"
#include "reports/otchettrudoustrvip.h"
#include "reports/otchetsrezbally.h"
#include "boleznistudentov.h"
#include "otpravkasoobsheniasotrudnikam.h"
#include "otpravkasoobshstudentam.h"
#include "godasozdplanakaf.h"
#include "reports/otchetformacontrolya.h"
#include "reports/otchetpraktiki.h"
#include "reports/otchetstudenty.h"
#include "reports/svodotchetpraktiki.h"
#include "godasozdaniagrafikavzaiomopos.h"
#include "proekty.h"
#include "reports/otchetpovyshkval.h"
#include "vedomostikursovye.h"
#include "spravochniki/kafedry.h"
#include "spravochniki/specialnosti.h"
#include "reports/otchetsotrudniki.h"
#include "spravkipochas.h"
#include "platnieobrazovatuslgi.h"
#include "reports/otchetspravkipochas.h"
#include "uvedomlenie.h"
#include "planumr.h"
#include "reports/svodotchetgakbally.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void openPorucheniya();
    void openProsmotr();

public slots:
  void showTime();
private slots:
    //   void on_action_triggered();

    void on_action_podkl_triggered();

    void on_action_semestr_triggered();

    void on_action_exit_triggered();

    void on_action_discipline_triggered();

    void on_action_group_triggered();

    // void on_action_form_obuch_triggered();

    // void on_action_form_opl_triggered();

    void on_action_tema_diplomnoi_triggered();

    void on_action_doljnosti_triggered();

    void on_action_konf_sem_triggered();

    void on_action_kur_rab_triggered();

    //  void on_action_lic_doc_types_triggered();

    //  void on_action_lic_types_triggered();

    void on_action_poruch_status_triggered();

    void on_action_zvaniya_triggered();

    //void on_action_voenkomat_triggered();

    // void on_action_tip_voprosa_triggered();

    // void on_action_prize_types_triggered();

    //  void on_action_stip_type_triggered();

    void on_action_prac_type_triggered();

    void on_action_lic_docs_triggered();

    void on_action_sport_triggered();

    //void on_action_main_journal_triggered();

    // void on_action_met_types_triggered();

    void on_action_metodichki_triggered();

    void on_action_studenti_triggered();

    //void on_action_vipuskniki_triggered();

    void on_action_stipend_triggered();

    void on_action_statia_triggered();

    void on_action_porucheniya_triggered();

    void on_action_olimp_triggered();

    //void on_action_posesheniya_triggered();

    void on_action_obchejitie_triggered();

    void on_action_partners_triggered();

    void on_action_Vakansi_triggered();

    void on_action_otpr_soobsh_triggered();

    void on_action_actVnedr_triggered();

    void on_action_practika_triggered();

    void on_action_sotrudniki_triggered();

    void on_actionKnigi_triggered();

    void on_action_nir_sotr_triggered();

    void on_actionPublicatNir_triggered();

    void on_actionBolezniSotr_triggered();

    void on_actionSponsors_triggered();

    void on_actionPovyshKval_triggered();

    void on_actionRaiting_triggered();

    void on_actionNagradi_triggered();

    void on_actionChecks();

    void on_check();

    void on_action_komGAK_triggered();

    void on_action_gak_triggered();

    void on_pushButton_show_clicked();

    void on_pushButton1_clicked();

    void on_pushButton_hide_clicked();

    void on_pushButton2_clicked();

    void on_pushButton3_clicked();

    void on_pushButton4_clicked();

    void on_pushButton5_clicked();

    void on_pushButton6_clicked();

    void on_pushButton7_clicked();

    void on_pushButton8_clicked();

    void on_pushButton9_clicked();

    void on_action_restart_triggered();

    void on_pushButton_back_clicked();

    void createMenus(int parentMenu);

    void menuClick();

    void on_action_raspisanie_triggered();

    void on_action_JurnalVzaimopos_triggered();

    void on_action_reportStarostaGruppi_triggered();

    void on_action_course_work_triggered();

    void on_action_Diplom_triggered();

    void on_action_Vipusk_triggered();

    void on_action_srezy_triggered();

    void  on_action_trudoVipusk_triggered();


    void on_action_RepTrudoVip_triggered();

//    void on_action_temp_triggered();
    void on_action_BolezniStud_triggered();

    void on_actionSendMessSotr_triggered();

    void on_action_srez_bally_triggered();

    void on_action_RassilkaStudent_triggered();

    void on_action_planZasedKaf_triggered();

    void on_action_forma_controlya_triggered();

    void on_action_praktiki_triggered();

    void on_action_otchet_studenty_triggered();

    void on_action_CreateGrafPosesh_triggered();

    void on_action_svod_otchet_praktik_triggered();

    void on_action_proekty_triggered();

    void on_action_otchet_povyshKval_triggered();

    void on_action_vedomosti_kursovye_triggered();

    void on_action_kafedry_triggered();

    void on_action_specialnosti_triggered();

    void on_action_otchet_sotrudniki_triggered();

    void on_action_spravki_pochas_triggered();

    void on_action_ObuchKursi_triggered();

    void on_action_otchet_spravki_pochas_triggered();

    void on_action_PlanUmr_triggered();

    void on_action_svodGak_triggered();

private:
    Ui::MainWindow *ui;
    QSqlQuery *checkNaznPoruch;
    QSqlQuery *checkIspolPoruch;
    QSqlQuery *checkDatePoruch;
    QSystemTrayIcon *icon;
    QEventLoop loop;
    QTimer *checker;
};

#endif // MAINWINDOW_H


