#include "authorization.h"
#include "ui_authorization.h"

/**
 * @brief Authorization::Authorization
 * Конструктор объекта
 * @param parent - родитель объекта.
 */
Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_acl=new DAL_ACL(this);
}
/**
 * @brief Authorization::~Authorization
 * Деструктор объекта
 */
Authorization::~Authorization()
{
    delete ui;
}
/**
 * @brief Authorization::on_pushButtonOk_clicked
 * Слот объекта, который срабатывает при нажатии на кнопку "OK".
 *Проверяет аутентификацию пользователя по логину и паролю.
 */
void Authorization::on_pushButtonOk_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    int dolj_id = dal_acl->authorization(ui->lineEditLogin->text(), ui->lineEditPassword->text(),this);
    if (dolj_id > 0)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        dal_main->setCurrentRole(dolj_id);
        close();
    }
}
void Authorization::on_pushButtonCancel_clicked()
{
    exit(0);
}
