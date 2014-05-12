#include <smtp.h>
#include <QMessageBox>
smtp::smtp(QString host, quint16 port)
{

    p_host = host;
    p_port = port;
    tcp = new QTcpSocket();//Создаем экземляр объекта сокета
    t = new QTextStream(tcp);
    

    connect(tcp, SIGNAL(connected()),this, SLOT(on_connected()));
    connect(tcp, SIGNAL(readyRead()), this, SLOT(on_read()));
    connect(this, SIGNAL(status(QString)), this, SLOT(message_sended(QString)));
}

smtp::~smtp()
{

}


void smtp::send_Mail()
{
    if (p_login.isEmpty() || p_password.isEmpty()) 
        {
            emit error_code(QString::fromLocal8Bit("Логин или пароль не установлены"));
            return;
        }
    if (p_mail_From.isEmpty() || p_mail_to.isEmpty())
        {
            emit error_code(QString::fromLocal8Bit("Получатель или отправитель не задан"));
            return;
        }
    if (p_body.isEmpty())
        {
            emit error_code(QString::fromLocal8Bit("Тело сообщения не задано"));
            return;
        }
    if (p_subject.isEmpty())
        {
            emit error_code(QString::fromLocal8Bit("Тема сообщения не задана"));
            return;
        }
    
    tcp->abort();
    tcp->connectToHost(p_host, p_port);
    state = Init;
}

void smtp::on_connected()
{
    emit status("Connected to SMTP server");
}

void smtp::on_read()
{
    if( !tcp->canReadLine() )
        return;
    
    QString responseLine;
    
    do {
            responseLine = tcp->readLine();
            response += responseLine;
        } while( tcp->canReadLine() && responseLine[3] != ' ' );
    qDebug() << responseLine;
    responseLine.truncate( 3 );
    
    if ( state == Init && responseLine[0] == '2' )
        {
            *t << "HELO there\r\n";
            t->flush();
            state = Start;
        }
    else if (state == Start && responseLine[0] == '2')
        {
            *t << "AUTH LOGIN\r\n";
            t->flush();
            state = Login;
        }
    else if (state == Login && responseLine[0] == '3')
        {
            *t << encodeBase64(p_login) << "\r\n";
            t->flush();
            state = Pass;
        }
    else if (state == Pass && responseLine[0] == '3')
        {
            *t << encodeBase64(p_password) << "\r\n";
            t->flush();
            state = Mail;
        }
    else if ( state == Mail && responseLine[0] == '2' )
        {
            
            *t << "MAIL FROM: <" << p_mail_From << ">\r\n";
            t->flush();
            state = Rcpt;
        }
    else if ( state == Rcpt && responseLine[0] == '2' )
        {
            
            *t << "RCPT TO: <" << p_mail_to << ">\r\n";
            t->flush();
            state = Data;
        }
    else if ( state == Data && responseLine[0] == '2' )
        {
            *t << "DATA\r\n";
            t->flush();
            state = Body;
        }
    else if ( state == Body && responseLine[0] == '3' )
        {
            QString body;
            body = p_body;
            QString subject;
            subject = p_subject;
            message = QString::fromLatin1( "From: " ) + p_mail_From
                      +QString::fromLatin1( "\nTo: " ) + p_mail_to +QString::fromLatin1( "\nSubject: "
                                                                                                       ) + subject +QString::fromLatin1( "\n\n" ) + body +"\n";
            message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1(
                    "\r\n" ) );
            message.replace( QString::fromLatin1( "\r\n.\r\n" ),
                             QString::fromLatin1( "\r\n..\r\n" ) );
            
            *t << message << "\n" << ".\r\n";
            t->flush();
            state = Quit;
        }
    else if ( state == Quit && responseLine[0] == '2' )
        {
            *t << "QUIT\r\n";
            state = Close;
            //t->flush();
            emit status( tr( "Message sent" ) );
        }
    else if ( state == Close )
        {
            
        }
    else
        {
            emit status(tr("Unknown answer from SMTP server"));
            state = Close;
        }
    response = "";   
}

void smtp::set_Login(QString login)
{
    p_login = login;
}

void smtp::set_Password_for_login(QString password)
{
    p_password = password;
}

void smtp::set_Sender_mail(QString email)
{
    p_mail_From = email;
}

void smtp::set_Recipient_mail(QString email)
{
    p_mail_to = email;
}

void smtp::set_Body_text(QString text)
{
    p_body = text;
}

void smtp::set_Subject(QString subject)
{
    p_subject = subject;
}

QString smtp::encodeBase64( QString xml )
{
    QByteArray text;
    text.append(xml);
    return text.toBase64();
}

QString smtp::decodeBase64( QString xml )
{
    QByteArray xcode("");;
    xcode.append(xml);
    QByteArray precode(QByteArray::fromBase64(xcode));
    QString notetxt = precode.data();
    return notetxt;
}
