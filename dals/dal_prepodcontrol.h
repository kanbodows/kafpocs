#ifndef DAL_PREPODCONTROL
#define DAL_PREPODCONTROL

#include "dal_main.h"

extern DAL_main *dal_main;
class Dal_prepodcontrol : public DAL_main
{
    Q_OBJECT
public:
    explicit Dal_prepodcontrol(QObject *parent = 0);
    QSqlQueryModel *getStatia(QString avtor, QString soavtor, QString tema);
    bool deleteStatia (int id_statia);
    QSqlQueryModel *getComboAvtor();
    QSqlQueryModel *getcomboPrepod();
    QSqlQueryModel *getcomboPredm();
    QSqlQueryModel *getcomboSemmestr();
    QSqlQueryModel *getcomboKafedry();
    QSqlQueryModel *getPlanUmr(QString goda);
    QSqlQueryModel *getComboSoAvtor(int id_avt);
    bool getStatiaAdd(int avtors_id, int soAvtors_id, QString temas, QDate dates);
    bool addPlanUMR(QString ispoln, QString UMR, QString anotac, double kolvoStr, int tiraj, QString srokIspoln, QString gods);
    bool PublicationNirAdd(int pub_id, QString nameF, QString Urlf, QString MejStandartNomer, QString NazvaniePublikacii, QString NazvanieJurnala, QString NomerVipuskaJurnala, QDate GodVipuska);
    bool sostavKomAdd(int pred, int secr, int ch1, int ch2, int ch3, int ch4, int ch5, QDate dates, int nomerRaporta);
    bool SotrudnikAdd(QString FIO, QString telephon, QString mails, QString obrazovanie, int staj_obsh, int stajVKGTU, int ped_staj, QString uchZvan, int dolj, double stavka, QString logins, QString passw, QDate dateRojd, QDate dateReg, int status, QString MestoRojd, QString mestoRaboti, QString nacion, QString SeriaPasport, QString addres, QString semPoloj, QString socZash, int OnlyGak, QString sostav);
    bool getSubWorkAdd(int prepod, int predm, int semes, QDate dates);
    QSqlQuery *getCurrentStatia(int id_stati);
    QSqlQuery *getCurrentSubWork(int id_sub_work);
    QSqlQuery *getCurrentSotrudnik(int rec_id);
    QSqlQuery *getCurrentDiscipline(int id_discipline);
    QSqlQuery *getCurrentPublication(int id_publ);
    QSqlQuery *getCurrentplanUmr(int zap_id);
    bool editStat (int id_stat, int statAvtor_id, int statSoAvtor_id, QString stat_tema, QDate stat_date);
    bool EditPlanUMR(int id_zap,QString ispoln, QString UMR, QString anotac, double kolvoStr, int tiraj, QString srokIspoln, QString gods);
    bool editSubWork (int subwork_id,int predm,int semes, QDate dates, int prepod);
    bool editParolSotr (int recor_id,QString newParol);
    QSqlQueryModel *getSubjWork(QString FIO, QString discipl);
    bool deleteSubWork (int subWork_id);
    bool deleteSotr(int sotr_id);
    bool deleteZapPlanUmr(int id_zap);
    QSqlQueryModel *getMetodichki(int id_discipline);
    QSqlQueryModel *getMetodichka(int discipl, int type_met);
    QSqlQueryModel *getKnigi(int id_discipline);
    QSqlQueryModel *getMetTypes();
    QSqlQueryModel *getDoljnost();
    QSqlQueryModel *getComboPubNir();
    QSqlQueryModel *getKniga(QString namess, int discip);
    QSqlQueryModel *getDisciplines();
    QSqlQueryModel *getDisciplines(QString name, int semestr_id);
    QSqlQueryModel *getPublication(QString numberNir, QString naimFaila);
    int addDiscipline(QString nazv, int semestr_id, int tochki, QString varianty, int mashin, int blanoch, int ust, int pism, int sotr_id, int kafedra_id, int specialnost_id, QString tip);
    bool editDiscipline(int discipline_id, QString nazv, int semestr_id, int tochki, QString varianty, int mashin, int blanoch, int ust, int pism, int sotr_id, int kafedra_id, int specialnost_id, QString tip);
    bool deleteDiscipline(int id_discipline);
    bool addPlanZasKaf(QString Vopros, QString Ispolnit,QString SrokiIspoln, QString GodaS, QString GodaPo);
    bool addSozdPlanKaf(QString gods1, QString godsS, QString godsPo);
    bool addSozdGrafKaf(QString gods1, QString godsS, QString godsPo);
    bool addGrafikVzaimopoa(int FioPrepod, QString datapos, int FIOProv, QString NachGod, QString KonecGoda);
    QSqlQuery *getCurrentMetodichka(int id_metodichka);
    QSqlQuery *getCurrentplanzasKaf(int zap);
    QSqlQuery *updateGalochkaInProsmotrplana(int ids_prosmPlana);
    bool addMetodichka(int discipline_id, QString nazv, QString avtor, QString file, QDate god, int metType_id, int sotr_id, QString URLfiles);
    bool editMetodichka(int id_metodichka, int discipline_id, QString nazv, QString avtor, QString file, QDate god, int metType_id, int sotr_id, QString URLfiles);
    bool editSotrudnik(int id_sotr, QString FIO, QString telephon, QString mails, QString obrazovanie, int staj_obsh, int stajVKGTU, int ped_staj, QString uchZvan, int dolj, double stavka, QString logins, QString passw, QDate dateRojd, QDate dateReg, int status, QString MestoRojd, QString mestoRaboti, QString nacion, QString SeriaPasport, QString addres, QString semPoloj, QString socZash, int OnlyGak, QString sostav);
    bool deleteMetodichka(int id_metodichka);
    bool ProverkaGoda(QString godS, QString GodPo);
    QSqlQuery *getCurrentKniga(int id_knigi);
    bool addKniga(int discipline_id, QString nazv, QString avtor, QString file, QDate god, QString izdatel, int sotr);
    bool editKniga(int id_knigi, int discipline_id, QString nazv, QString avtor, QString file, QDate god, QString izdatel, int sotr);
    bool editPublNir(int id_publ, int pub_nimer_proecta, QString namefil, QString UrlFil, QString MejStandartNomer, QString NazvaniePublikacii, QString NazvanieJurnala, QString NomerVipuskaJurnala, QDate GodVipuska);
    bool deleteKniga(int id_knigi);
    bool ProverkaPrepod(int prepod_id, QDateTime Dataprov);
    bool ProverProveraush(int prov_id, QDateTime Dataprov);
    bool deletePublicationNir(int id_pub);
    bool deleteObrazKursi(int id_obrazKurs);
    bool deleteSostavKom(int id_sost_kom);
    QSqlQueryModel *getSotrModels(QString sotr, int doljnost, QString sostav);
    QSqlQueryModel *getSotrReportModels(int doljnost, QString sostav);
    QSqlQueryModel *getUchastnikiKom(int pred, int secr, int ch1, int ch2, int ch3, int ch4, int ch5);
    QSqlQueryModel *getPlatnieObrazUslugi(QString nazvanKur, QString VidKur);
    bool addPrepodDiscipline(int sotrudnik_id, int group_id, int  discipline_id, int lk, int pr, int lb);
    QSqlQueryModel *getPredmetPrepod(int id_discipline);
    QSqlQueryModel *getGroups(int id_discipline);
    bool existLektor(int  discipline_id);
    bool existPraktik(int group_id, int  discipline_id);
    bool existLabor(int group_id, int  discipline_id);    
    QSqlQueryModel *getPovyshKval(QString sotr, QString tema, QString kurs);
    QSqlQueryModel *getPovyshKvalReport(int doljnost, QDate beginDate, QDate endDate);
    QSqlQuery *getCurrentPovyskKval(int id_povyskKval);
    bool addPovyskKval(int sotr_id, QDate date, QString tema, QString kurs, QString mesto, QString sert, QString sertFile, double ocenka, QString URLfile);
    bool editPovyskKval(int id_povyskKval, int sotr_id,  QDate date, QString tema, QString kurs, QString mesto, QString sert, QString sertFile, double ocenka, QString URLfile);
    bool editGrafkik(int zap_id, int prepod, int prov, QDateTime dataprov, QString godS, QString godPo);
    bool deletePovyskKval(int id_povyskKval);
    QSqlQueryModel* getRaiting(int doljnost, QDate beginDate, QDate endDate);
    QSqlQueryModel* getSostavKom(int nomRaport);
    QSqlQueryModel *getNagradi(QString sotr);
    QSqlQuery *getCurrentNagradi(int id_nagradi);
    bool addNagradi(int sotr_id, QString nagrada, QDate date, QString file);
    bool editNagradi(int id_nagradi, int sotr_id, QString nagrada, QDate date, QString file);
    bool deleteNagradi(int id_nagradi);
    QSqlQueryModel *getPorucheniya(int status_id, QString title);
    QSqlQuery *getCurrentPoruchenie(int id_poruch);
    QSqlQuery *getCurrentSostavGAK(int id_gaks);
    bool addPoruchenie(QString title, QString text, QString files, int ispolnitel, QDate date);
    bool PlatnieObrazovatslugiAdd(int id_sotrud, QString nazvanKursa, QString kolvoChasov, QString vidZanyatia, QString periodObuch, QString oplata, QString tipKursa);
    bool editPoruchenie(int id_poruch, QString title, QString text, QString files, int ispolnitel, QDate date);
    bool PlatnieObrazovatslugiEdit(int zap, int id_sotrud, QString nazvanKursa, QString kolvoChasov, QString vidZanyatia, QString periodObuch, QString oplata, QString tipKursa);
    bool editPlanZasKaf(int zap, QString GODS, QString GODSPO, QString SrokVipol, QString VoprosZas, QString ispolnit);
    bool sostavKomEdit(int zap, int preds, int secr, int ch1, int ch2, int ch3, int ch4, int ch5, QDate dates, int chislo);
    bool deletePoruchenie(int id_poruch);
    bool setStatusPorucheniya(int id_poruch, int status_id,  bool checkPoruch, bool checkIspoln);
    bool ProverNaIzmenenia(int proveryaush, QDateTime datapos);
    bool ProverNaIzmForPrepod(int prepod, QDateTime datapos);
    QSqlQuery *getCurrentRaspisanie(int id_sotr);
    QSqlQuery *getCurrentPlatnuyObrazovatUslugu(int id_platnieobrazUslugi);
    QSqlQuery *getCurrentGrafVzaimoPos(int id_graf);
    QSqlQuery *deletePlanKaf(QString Gods, QString GodPo);
    QSqlQuery *deleteProsmotrPlanKaf(QString Gods, QString GodPo);
    QSqlQuery *getCurrentSpravkaPochas(int id_sparvki_pochas);
    QSqlQueryModel *getSpravkiPochas(int sotr_id, QDate dateBegin, QDate dateEnd);
    QSqlQueryModel *getSpravkiPochasReport(int sotr_id, QDate date);
    int addSpravkaPochas(int nomer_spravki, QString facultet, int kafedra_id, int sotr_id, int nomer_protocola, QDate data_za, double vsego_lek, double vsego_labor, double vsego_prak, double vsego_on_line, double vsego_off_line, double vsego_kontr_rab, double vsego_ekzamen, double vsego_kurs_rab, QString yazyk, double vsego);
    bool editSpravkaPochas(int id_sparvki_pochas, int nomer_spravki, QString facultet, int kafedra_id, int sotr_id, int nomer_protocola, QDate data_za, double vsego_lek, double vsego_labor, double vsego_prak, double vsego_on_line, double vsego_off_line, double vsego_kontr_rab, double vsego_ekzamen, double vsego_kurs_rab, QString yazyk, double vsego);
    bool addSpravkiDetails(QString daty, int gruppa_id, int discipline_id, double lekcii, double labor, double prak, double on_line, double off_line, double kontr_rab, double ekzamen, double kurs_rab, int sparvki_pochas_id);
    bool deleteSpravkiDetails(int id_sparvki_pochas);
private:
    QSqlQueryModel static *StatiaModelView;
    QSqlQueryModel static *comboAvtorModel;
    QSqlQueryModel static *comboSoAvtorModel;
    
};

#endif // Dal_prepodcontrol_H
