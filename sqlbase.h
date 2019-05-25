#ifndef SQLBASE_H
#define SQLBASE_H
#include<QtSql>
#include<QString>
#include<QCryptographicHash>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>

using namespace std;

struct sqldata
{
    string id;
    string fio;
    string tel;
    string date;
};

class sqlbase{
public:
    QString changePerson( vector<sqldata> );// изменить данные о пользователе
    QString addPerson( vector<sqldata> ); //добавить пользователя
    QString delPerson( QString ); // удалить пользователя
    QString findPerson( QString ); // найти пользователя
    QString showPersonAll(); // показать всю базу данных
    QString baseLogPass(); // показать все логины/пароль
    void baseOpen();
    void baseClose();
    string autho( string ); // авторизация для клиента
    QSqlError proof( QSqlQuery ); // проверка на ошибки
    QString changePassword( QString ); // сменить пароль(для админа)
    QString addLogin( QString); // добавить логин
    QString delLogin( QString); // удалить логин
    string authoServ( string ); // авторизация для сервера
    string checkFio( string ); // проверка фамилии
    string checkTel( string ); // проверка телефона
    string checkDate( string ); // проверка даты
    string checkDatePos( string ); // проверка даты посещения

    QString changeForUser( QString ); // сменить пароль(для пользователя)
    QString addDate( string ); // отметить посещение
    QString abonement( string ); // абонемент
     QSqlDatabase mydb; //

private:
};

#endif // SQLBASE_H
