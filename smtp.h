#ifndef SMTP_H
#define SMTP_H
//#include <QtNetwork/qtcpsocket.h>
#include <QTcpSocket>
#include <QTextStream>

class smtp : public QObject
{
    Q_OBJECT
    
public: 
    smtp(QString host, quint16 port);
    ~smtp();
    
    void set_Login(QString login); //Задаем логин
    void set_Password_for_login(QString password);//Задем пароль
    void set_Sender_mail(QString email);//Задаем отправителя
    void set_Recipient_mail(QString email);//Задаем получателя
    void set_Body_text(QString text);//Задаем текст тела сообщения
    void set_Subject(QString subject);//Задаем тему сообщения
    void send_Mail();//Отправка сообщенич
    
private:
    
    QString p_host;
    quint32 p_port;
    QString p_login;
    QString p_password;
    QString p_mail_From;
    QString p_mail_to;
    QString p_body;
    QString p_subject;
    
    QString message;
    
    int state;
    QString response;
    
    QTcpSocket *tcp;
    QTextStream *t;
    QString encodeBase64( QString xml );
    QString decodeBase64( QString xml );
    
  
    
    enum State {
            Init,  //Инициализация
            Start,
            Login,//Логин
            Pass, //Пароль
            Mail, //Отправитель
            Rcpt, //Получатель
            Data, 
            Body, //Тело
            Quit,
            Close
        };

    
private slots:
    void on_connected();
    void on_read();

signals:
    void error_code(QString error_text);  
    void status( const QString & );
};

#endif // SMTP_H
