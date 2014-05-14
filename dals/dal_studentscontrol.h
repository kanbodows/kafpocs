#ifndef DAL_STUDENTSCONTROL_H
#define DAL_STUDENTSCONTROL_H

#include "dal_main.h"
extern DAL_main *dal_main;
class Dal_studentsControl : public DAL_main
{
    Q_OBJECT
public:
    explicit Dal_studentsControl(QObject *parent = 0);
    QSqlQueryModel *getKonf_sem(int gruppa, QString student, QString theme);
    QSqlQueryModel *getGAK(int nomPrik, int gak_id);
    QSqlQuery *getCurrentKonf_sem(int id_konf);
    QSqlQuery *getCurrentPracWork(int id_PracWork);
    QSqlQueryModel *getObchejitie(int gruppa, QString student);
    QSqlQuery *getCurrentObchejitie(int id_obch);
    QSqlQuery *getCurrentVedomostiKursovye(int id_vedomosti);
    QSqlQuery *getCurrentGAK(int gaks_id);
    QSqlQuery *getCurrentStudent(int id_student);
    QSqlQuery *CurrentBoleznStud(int id_bolezStud);   
    QSqlQueryModel *getOlimpiada(QString stud, QString pred, int ZanMesto);
    QSqlQueryModel *getVedomostiKursovye(int group_id, int discipline_id, QDate dateBegin, QDate dateEnd); 
    int addVedomostiKursovye(int group_id, int discipline_id, QDate data_zashity, int kolvo_rabot, int kolvo_listov);
    bool addKursovyeVedomosti(int vedomostiKursovye_id, QString primechanie, int id_course_work);
    bool addKonfSem(int student_id, QString theme, QString comments, int mesto, QString sert, QDate date);
    bool editKonfSem(int id_konf_sem, int student_id, QString theme, QString comments, int mesto, QString sert, QDate dates);
    bool addStudent(QString fio, int group_id, QDate dateBirth, QDate dateReg, QString pasport, QString tel, QString rodTel,  QString email, QString rodEmail, QString adresProj, QString adresProp, QString shkola, int formaObuch_id, double skidka, int formaOplaty_id, bool otch, QString photo, bool starost, QDate dataPost);
    bool editStudent(int id_student, QString fio, int group_id, QDate dateBirth, QDate dateReg, QString pasport, QString tel, QString rodTel,  QString email, QString rodEmail, QString adresProj, QString adresProp, QString shkola, int formaObuch_id, double skidka, int formaOplaty_id, bool otch, QString photo, bool starost, QDate dataPost);
    bool DiplomOcenkaUpdate(int id_dip, QString ocenk, QDate dates);
    //    bool editKonfSem(int id_konf_sem, int student_id, QString theme, QString comments, int mesto, QString sert, QDate dates);
    bool addObchejitie(int student_id, QString adres, double oplata, QString otzivi);
    bool editObchejitie(int id_obch, int student_id, QString adres, double oplata, QString otzivi);
    QSqlQueryModel *getComboGroup(int id_spec);
    QSqlQueryModel*getSpec();
    QSqlQueryModel *getComboTypePrac();
    QSqlQueryModel *getStudentGroup(int group_id);
    QSqlQueryModel *getGakBally(int spec_id, int group_id, int gak_id);
    QSqlQueryModel *getDiplomsModel(QString stud, int grupa);
    QSqlQueryModel *getcomborucPrac();
    QSqlQueryModel *getcomboSem();
    QSqlQueryModel *getTemadiploma();
    QSqlQueryModel *getFormaObuch();
    QSqlQueryModel *getFormaOplaty();
    QSqlQueryModel *getJurnVzaimopos(QString prov, QDate datestart, QDate dateend);
//    QSqlQueryModel *getNomRapFromSostavGAK();
    QSqlQueryModel *getSotrKaf(int sotr_id);
    QSqlQueryModel *getDiscipl();
    QSqlQueryModel *getKafedry();
    QSqlQueryModel *getFormKom();
    QSqlQueryModel *getTemaRusKG(int id_theme);
    QSqlQueryModel *getComboRuk();
    QSqlQueryModel *getVipusk_ki();
    QSqlQueryModel *getBolezniStud(int group, QString bolez, QDate dateZbol, QDate dateVizdorav);
    QSqlQueryModel *getStudent(int groups);
    QSqlQueryModel *SendMessageStud(int groups);
    QSqlQueryModel *getStudents(int spec_id, int group_id, QString student);
    QSqlQuery *getStudentNeotch(int id_stud);

    //QSqlQueryModel *get(int id_stud);
    QSqlQueryModel *getTrudoVipusk(QString Stud, int groups);
    QSqlQueryModel *getVipGroup(int groupss);
    QSqlQueryModel *getRepTrudoVipusk();
    QSqlQueryModel *getCourseWorks(int group_id, int discipline_id, QString tema, int vedomost_id);
    bool addAktSpisaniyaKurs(int vedomostKurs_id, QDate data_spis, QString komissiya1, QString komissiya2, QString komissiya3, QString primechanie);
    QSqlQueryModel *getSrezy(int specialnost_id, int group_id, QDate dateBegin, QDate dateEnd);
    QSqlQuery *getSrezyQuery(int group_id, QDate dateBegin, QDate dateEnd);
    QSqlQuery *getSrezySrednie(int srez_id);
    bool addDiplom(int stud_id, int tema, int kom, int ruk);
    bool BolezniStudAdd(int stud, QString bolez, QDate datZabol, QDate datVizd);
    bool getOlimpAdd(int stud_id, QString pred, QDate dates, QString comm, int mest, QString mestProv,QString prveril);
    bool getJurnVzaimoposAdd(int sotr_id, int discp_id, int group_id, int prover_id, QDateTime dates, QString commentarii, QString vidDiscipl, QString Ocenka);
    bool GAKAdd(int discp1, int discp2, int discp3, int discp4, QDate dates, QString kab, int nomerPrikaza, int GodFormKom);
    bool getTrudoVipAdd(int vip_ids, QString addresRab, QString ZanDolj, int RabotZaGranic);
    bool getVipusknikAdd(int stud_id, QString nomer_diploma, QString addressFoto, QDate dates3, QString emails, QString telefon, QString mestRab, QString dolj);
    bool getActVnedreniaAdd(int stud_id, QString nomer_acta, QDate dates, QString predpriatie, int tema_id, QString dopolnenia);
    bool editOlimp(int id_ol, int students, QString predm, QDate dates, QString comm, int zanMest, QString mestoProv, QString prover);
    bool getPracWorkAdd(int typePr_id, QDate dates, int sem_id, int stud_id, int lid_kaf_id, QString lid_predpr, QString mestProh, QString ocenka, QString otziv, QString Nom_prik);
    QSqlQuery *getCurrentOlipm(int id_olim);
    QSqlQuery *getCurrentJurnalVzaimoPos(int rec);
    bool deleteOlimp (int id_olimp);
    bool deleteJurnVzaimPos(int id_jurnalVzaimopos);
    bool deleteDiploms(int id_diploms);
    bool deleteVipusk (int id_vip);
    bool deleteBolezniSotr(int id_bolStud);
    bool deletePracWork (int id_prWork);
    bool deleteKonfSem (int konf_sem);
    bool deleteObshejit (int id_obshejit);
    bool deleteGAK(int id_gak);
    bool deleteStudent(int id_stud);
    bool deleteTrudoVipusk(int id_TrudoVip);
    QSqlQueryModel *getVipusknik(QString stud, QString group, QDate dates, QDate datesEnd);
    QSqlQueryModel *SendMessageVip(QString group);
    QSqlQueryModel *SendMessageSotrKaf();
    QSqlQueryModel *getSostavKom(int komis_id);
    QSqlQuery *getCurrentVipusknik(int id_vip);
    QSqlQuery *getCurrentActVnedr(int id_act);
    bool editVipusknik(int id_vip, int stud_id, QString nomer_diploma, QString addressFoto, QDate dates3, QString emails, QString telefon, QString mestRab, QString dolj);
    bool editBoleznStud(int id_bol_stud, int stud_id, QString bolezn, QDate dateStart, QDate dateEnds);
    bool editPracWork(int id_prac, int prac_id, QDate dates, int semestr_id, int stud_id, int lid_kaf, QString lid_placePredp, QString Mestoprohoj, QString ocenka, QString otziv, QString nom_prikaza);
    bool editActVnedr(int id_actvndr,int stud_id, QString nomer_acta, QDate dates, QString predpriatie, int tema_id, QString dopolnenia);
    bool JurnVzaimoposEdit(int record, int sotr_id, int discp_id, int group_id, int prover_id, QDateTime dates, QString commentarii, QString tip_discp, QString Ocenka);
    bool GAKsEdit(int zap, int discp1, int discp2, int discp3, int discp4, QDate dates, QString kab, int nomerPrikaza, int GodFormKom);
    QSqlQueryModel *getActVnedr(QString stud, QString group, QDate dates, QDate datesEnd);
    QSqlQueryModel *getPracWork(QString stud, QString group, QDate dates, QDate datesEnd, int tip_prac);
    bool deleteActVned (int id_actvndr);
    QSqlQuery *getCurrentCourseWork(int id_course_work);
    QSqlQuery *getCurrentTrudoVipusknik(int id_trudoVip);
    bool addCourseWork(int discipline_id, int student_id, int sotrudnik_id, QString tema, QString course_work_fail, int ocenka, QDate data_sdachi, int kolvo_listov);
    bool editCourseWork(int id_course_work, int discipline_id, int student_id, int sotrudnik_id, QString tema, QString course_work_fail, int ocenka, QDate data_sdachi, int kolvo_listov);
    bool editTrudoVipusknik(int trudo_id, int vip_id, QString AdrRab, QString ZanDolj, int RabZaGran);
    bool deleteCourseWork(int id_course_work);
    QSqlQuery *getCurrentSrez(int id_srez);
    bool addSrez(int group_id, QDate data_sreza, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5);
    bool editSrez(int id_srez, int group_id, QDate data_sreza, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5);
    bool deleteSrez(int id_srez);
    bool addSrezBally(int student_id, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5, int srez_id);
    bool addGakBally(int student_id, int discipline_1, int discipline_2, int discipline_3, int discipline_4, bool nedopusk, int gak_id, int group_id);
    QSqlQueryModel *getPracWorkReport(int specialnost_id, int group_id, QDate dateStart, QDate dateEnd, int tip_prac);
    QSqlQueryModel *getStudentsReport(int specialnost_id, int group_id, QDate dateStart, QDate dateEnd, int formaObuch_id, int formaOplaty_id);
    QSqlQueryModel *getPractikiSvodReport(int specialnost_id, int tipPrac, QDate dateStart, QDate dateEnd);
    QSqlQueryModel *getGakSvodReport(int specialnost_id, int group_id, int gak_id);
private:
};

#endif // DAL_STUDENTSCONTROL_H
