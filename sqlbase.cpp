#include "sqlbase.h"
string sqlbase::checkFio(string fio)//Используемые символы - только буквы; Если регистр не тот, меняем на правильный.
{
    for(int i = 0; i < fio.size();i++)
    {
        if(fio[i] < 65 || fio[i] > 122 ||  !isalpha(fio[i]) ) // не буква
        {
            fio.erase(i,1);
            i--;
            continue;
        }
        if(fio[0] >= 97 && fio[0] <= 122) // маленькие буквы
        {
            fio[0]-=32;
            continue;
        }
        if(fio[i] >= 65 && fio[i] <= 90) // большие буквы
        {
            fio[i]+=32;
        }
        if(fio[i] >= 97 && fio[i] < 122 ) // маленькие буквы
        {
            continue;
        }


    }
    return fio;
}
string sqlbase::checkTel(string tel)//длина строки 10 символов, все из которых цифры;
{
    int k = 0;
    for(int i = 0;i < tel.size();i++)
    {
        if(isdigit(tel[i]))
        {
            k++;
        }
        else {
            return "null";
        }
    }
    if(k == 10)
    {
        return tel;
    }
    else {
        return "null";
    }
}
string sqlbase::checkDate(string date_temp)
{
    QString date = QString::fromStdString(date_temp);

    QDate current = QDate::currentDate();
    QDate now = QDate::fromString(date,"dd.MM.yyyy");

    if(now.isValid() && (now >= current))
    {
        return date_temp;
    }
    else{
        return "Неверная дата.\nДата не должна быть меньше сегодняшнего дня и иметь правильный формат.";
    }

}
string sqlbase::checkDatePos(string date_temp)
{
    QString date = QString::fromStdString(date_temp);

    QDate current = QDate::currentDate();
    QDate now = QDate::fromString(date,"dd.MM.yyyy");

    if(now.isValid() && (now <= current))
    {
        return date_temp;
    }
    else{
        return "Неверная дата.\nДата не должна быть больше сегодняшнего дня и иметь правильный формат.";
    }
}
QString sqlbase::changePerson(vector<sqldata> db)
{
    vector<sqldata>::iterator it = db.begin();

    QString id = QString::fromStdString(it->id);
    QString name = QString::fromStdString(checkFio(it->fio));
    QString tel = QString::fromStdString(checkTel(it->tel));
    QString date = QString::fromStdString(it->date);

    baseOpen();
    QSqlQuery qry;

    qry.prepare("update  DataBasePerson set NAME ='"+id+"',FIO ='"+name+"',TEL = '"+tel+"' where ID = '"+id+"'");

    if(qry.exec())
    {
        baseClose();
        return "Пользователь изменен";
    }
    else {
        baseClose();
        return proof(qry).text();
    }
}
QString sqlbase::changeForUser(QString message)
{
    baseOpen();
    string mes = message.toStdString();

    int k = mes.find("&");
    string log = mes.substr(0,k);
    QString login = QString::fromStdString(log);
    mes.erase(0,k+1);

    k = mes.find("&");
    string pass = mes.substr(0,k);
    mes.erase(0,k+1);
    QString password = QString::fromStdString(pass);
    string reppass = mes;
    QString REPPASS = QString::fromStdString(reppass);

    QSqlQuery qry("SELECT * FROM UserName where login = '"+login+"'");
    if(qry.next())
    {
        QString prov = qry.value(1).toString();
        if(prov == password)
        {
            QSqlQuery qry2("UPDATE UserName set password = '"+REPPASS+"' WHERE login = '"+login+"'");
        }
    }
    return "Successfully";
}

QString sqlbase::addPerson(vector<sqldata> db)
{
    vector<sqldata>::iterator it = db.begin();

    QString id = QString::fromStdString(it->id);
    QString name = QString::fromStdString(checkFio(it->fio));
    QString tel = QString::fromStdString(checkTel(it->tel));
    QString date = QString::fromStdString(checkDate(it->date));

    if(it->id=="" || it->fio=="" || it->tel=="" || it->date=="")
    {
        return "Необходимо заполнить все поля";
    }

    if(tel=="null")
    {
        return "Неправильный номер.\nНомер должен состоять из 10 цифр";
    }

    if(date=="Неверная дата.\nДата не должна быть меньше сегодняшнего дня и иметь правильный формат.")
    {
        return date;
    }

    if(id=="" || name=="" || tel=="" || date=="")
    {
        return "Все поля должны заполнены";
    }
    int pos_t = 0;
    QString pos = QString::number(pos_t);
    baseOpen();
    QSqlQuery qry;
    qry.prepare("SELECT * FROM DataBasePerson ");
    qry.exec();
    while(qry.next())
    {
        QString tel_t = qry.value(2).toString();
        if(tel==tel_t)
        {
            return "Номер уже есть в базе";
        }
    }
    qry.prepare("insert into DataBasePerson (NAME,FIO,Tel,ABONEMENT,VISIT) values ('"+id+"','"+name+"','"+tel+"','"+date+"','"+pos+"')");

    if(qry.exec())
    {
        baseClose();
        return "Пользователь добавлен";
    }
    else {
        baseClose();
        return proof(qry).text();
    }


}
QString sqlbase::delPerson(QString message)
{
    baseOpen();
    QSqlQuery qry;
    string temp = message.toStdString();

    int k = temp.find("&");
    string ID = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string FIO = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string TEL = temp.substr(0,k);

    QString id = QString::fromStdString(ID);
    QString fio = QString::fromStdString(FIO);
    QString tel = QString::fromStdString(checkTel(TEL));

    if(ID=="" || FIO=="" || TEL=="")
    {
        return"Недостаточно информации для удаления";
    }
    if(tel=="null")
    {
        return "Неправильный формат номера";
    }

    QSqlQuery qry2;

    qry2.prepare("SELECT * FROM DataBasePerson where NAME = '"+id+"' and FIO='"+fio+"' and Tel='"+tel+"'");
    if(qry2.exec())
    {
        qry.prepare("Delete from  DataBasePerson where NAME = '"+id+"' and FIO='"+fio+"' and Tel='"+tel+"'");

        if(qry.exec())
        {
            baseClose();
            return "Пользователь удален";
        }
        else {
            baseClose();
            return proof(qry).text();
        }
    }
    else{
        return "Такого пользователя нет";
    }

}

QString sqlbase::findPerson(QString message)
{
    string temp = message.toStdString();

    int k = temp.find("&");
    string ID = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string FIO = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string TEL = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string ABONEMENT = temp.substr(0,k);
    temp.erase(0,k+1);

    k = temp.find("&");
    string VISIT = temp.substr(0,k);
    temp.erase(0,k+1);

    if(message=="&&&&&")
    {
        return "Все поля пусты.\nПоиск невозможен";
    }

    QString id = QString::fromStdString(ID);
    QString fio = QString::fromStdString(FIO);
    QString tel = QString::fromStdString(checkTel(TEL));
    QString abon = QString::fromStdString(checkDate(ABONEMENT));
    QString visit = QString::fromStdString(checkDatePos(VISIT));

    if(tel=="null" && TEL!="")
    {
        return"Неправильный номер";
    }

    if(abon=="Неверная дата.\nДата не должна быть меньше сегодняшнего дня и иметь правильный формат." && ABONEMENT!="")
    {
        return"Неверная дата.\nДата не должна быть меньше сегодняшнего дня и иметь правильный формат.";
    }
    if(visit=="Неверная дата.\nДата не должна быть больше сегодняшнего дня и иметь правильный формат." && VISIT!="")
    {
        return "Неверная дата.\nДата не должна быть больше сегодняшнего дня и иметь правильный формат.";
    }

    baseOpen();

    QSqlQuery qry;

    if(ID!="" && FIO=="" && TEL=="" && ABONEMENT=="" && VISIT=="") // поиск по имени
    {
        qry.prepare("Select * from DataBasePerson where NAME = '"+id+"'");
    }
    else if(ID!="" && FIO!="" && TEL=="" && ABONEMENT=="" && VISIT=="") // имя и фамилия
    {
        qry.prepare("Select * from DataBasePerson where NAME = '"+id+"' and FIO = '"+fio+"'");
    }
    else if(ID=="" && FIO=="" && TEL!="" && ABONEMENT=="" && VISIT=="") // телефон
    {
        qry.prepare("Select * from DataBasePerson where Tel='"+tel+"'");
    }
    else if(ID=="" && FIO!="" && TEL=="" && ABONEMENT=="" && VISIT=="") // фамилия
    {
        qry.prepare("Select * from DataBasePerson where FIO = '"+fio+"'");
    }
    else if(ID!="" && FIO!="" && TEL!="" && ABONEMENT=="" && VISIT=="") // имя фамилия телефон
    {
        qry.prepare("Select * from DataBasePerson where NAME='"+id+"' and FIO = '"+fio+"' and Tel='"+tel+"'");
    }
    else if(ID=="" && FIO=="" && TEL=="" && ABONEMENT!="" && VISIT=="") // абонемент
    {
        qry.prepare("Select * from DataBasePerson where Abonement='"+abon+"'");
    }
    else if(ID=="" && FIO=="" && TEL=="" && ABONEMENT=="" && VISIT!="") // дата посещения
    {
        qry.prepare("Select * from DataBasePerson where DATA='"+visit+"'");
    }
    else if(ID=="" && FIO=="" && TEL=="" && ABONEMENT!="" && VISIT!="") // дата посещения и абонемент
    {
        qry.prepare("Select * from DataBasePerson where Abonement ='"+abon+"' and DATA='"+visit+"'");
    }
    else if(ID!="" && FIO!="" && TEL!="" && ABONEMENT!="" && VISIT!="") // все поля
    {
        qry.prepare("Select * from DataBasePerson where NAME ='"+id+"' and FIO='"+fio+"' and tel='"+tel+"' and Abonement ='"+abon+"' and DATA='"+visit+"'");
    }
    else if(ID!="" && FIO!="" && TEL!="" && ABONEMENT=="" && VISIT!="") // все поля кроме абонемента
    {
        qry.prepare("Select * from DataBasePerson where NAME ='"+id+"' and FIO='"+fio+"' and tel='"+tel+"' and DATA='"+visit+"'");
    }
    else{
        return "Неправильный формат поиска";
    }
    qry.exec();
    if(qry.next())
    {
        qry.previous();
        QString rezult;

        while(qry.next())
        {
            QString id = qry.value(0).toString();
            QString name = qry.value(1).toString();
            QString tel = qry.value(2).toString();
            QString date = qry.value(3).toString();
            rezult += id + "&" + name + "&" + tel +"&" + date +"&";
        }
        baseClose();
        return "Пользователь найден&" + rezult;
    }
    else {
        baseClose();
        return "Пользователь не найден";
    }

}
QString sqlbase::showPersonAll()
{
    baseOpen();
    QSqlQuery qry("SELECT * FROM DataBasePerson ORDER BY FIO");

    QString rezult;
    if(qry.exec())
    {
        while(qry.next())
        {
            QString id = qry.value(0).toString();
            QString name = qry.value(1).toString();
            QString tel = qry.value(2).toString();
            QString date = qry.value(3).toString();
            QString abonement = qry.value(4).toString();
            QString pos = qry.value(5).toString();
            rezult += id + "&" + name + "&" + tel +"&" + date +"&" + abonement + "&" + pos + "&";
        }
        baseClose();
        return rezult;
    }
    else
    {
        baseClose();
        return proof(qry).text();
    }
}

void sqlbase::baseOpen()
{
    sqlbase obj;
    obj.mydb = QSqlDatabase::addDatabase("QSQLITE");
    obj.mydb.setDatabaseName("C:\\repos\\Server\\mydatabase.sqlite");

    if(!obj.mydb.open())
    {
        qDebug() << "File is not open";
    }
    else {
        qDebug() << "File open";
    }
}

void sqlbase::baseClose()
{
    sqlbase obj;
    obj.mydb.close();
    obj.mydb.removeDatabase(QSqlDatabase::defaultConnection);
}

string sqlbase::autho(string message)
{
    string log,pass;

    baseOpen();

    int pos = message.find("&");
    log = message.substr(0,pos);
    message.erase(0,pos+1);
    pos = message.find("&");
    pass = message.substr(0,pos);
    message.erase(0,pos+1);

    QString tlog,tpass;
    tlog = QString::fromStdString(log);
    tpass = QString::fromStdString(pass);
    QSqlQuery qry;
    string stata;

    if(qry.exec("select * from UserName where login='"+tlog +"' and password='"+tpass+"'"))
    {
        int count = 0;
        while(qry.next())
        {
            count++;
            QString stat = qry.value(2).toString();
            stata = stat.toStdString();
        }
        if(count==1)
        {
            baseClose();
            return stata;
        }
        if(count>1)
        {
            baseClose();
            return "Есть повторы";
        }
        if(count < 1)
        {
            baseClose();
            return "Неверный логин или пароль";
        }

    }
}
QSqlError sqlbase::proof(QSqlQuery qry)
{
    return qry.lastError();
}
QString sqlbase::baseLogPass()
{
    baseOpen();
    QSqlQuery qry("SELECT * FROM UserName");
    QString rezult;
    if(qry.exec())
    {
        while(qry.next())
        {
            QString log = qry.value(0).toString();
            QString pass = qry.value(1).toString();
            QString stat = qry.value(2).toString();
            rezult += log + "&" + pass + "&" + stat +"&";
        }
        baseClose();
        return rezult;
    }
    else
    {
        baseClose();
        return proof(qry).text();
    }
}
QString sqlbase::changePassword(QString message)
{
    baseOpen();
    string rez = message.toStdString();
    int k = rez.find("&");
    string log = rez.substr(0,k);
    rez.erase(0,k+1);
    k = rez.find("&");
    string pass_temp = rez.substr(0,k);
    rez.erase(0,k+1);
    string stat = rez;

    QString login = QString::fromStdString(log);
    QString pass = QString::fromStdString(pass_temp);
    QString status = QString::fromStdString(stat);

    QSqlQuery qry("SELECT * FROM UserName WHERE login = '"+login+"'");
    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if(count == 1 && pass=="" && status=="") // ++
        {
            return "No change";
        }
        if(count == 1 && pass == "" && status!= "") // ++
        {
            QSqlQuery qry2("UPDATE UserName set Status = '"+status+"' where login = '"+login+"'" );
            return "Succesfull";
        }
        if(count == 1 && pass!= "" && status == "" ) // ++
        {
            QSqlQuery qry2("UPDATE UserName set password = '"+pass+"' where login = '"+login+"'" );
            return "Succesfull";
        }
        if(count == 1 && pass!= "" && status!= "")
        {
            QSqlQuery qry2("UPDATE UserName set password = '"+pass+"',Status = '"+status+"' where login = '"+login+"'" );
            return "Succesfull";
        }
        if(count < 1){
            return "Error";
        }
    }

    else{
        return proof(qry).text();
    }
}

QString sqlbase::addLogin(QString message)
{
    baseOpen();
    string rez = message.toStdString();
    int k = rez.find("&");
    string log = rez.substr(0,k);
    rez.erase(0,k+1);
    k = rez.find("&");
    string pass_temp = rez.substr(0,k);
    rez.erase(0,k+1);
    string stat = rez;


    QString login = QString::fromStdString(log);
    QString pass = QString::fromStdString(pass_temp);
    QString status = QString::fromStdString(stat);

    QSqlQuery qry("insert into UserName (Login,Password,Status) VALUES ('"+login+"','"+pass+"','"+status+"')" );
    if(qry.exec())
    {
        int count = 0;
        while(qry.next())
        {
            count++;
        }
        if(count == 1)
        {
            return "Succesfully";
        }
        if(count < 1)
        {
            return "Error";
        }
        baseClose();
    }
    else {
        baseClose();
        return proof(qry).text();
    }
}
QString sqlbase::delLogin(QString message)
{
    QString log = message;
    baseOpen();

    QSqlQuery qry("DELETE FROM UserName where login = '"+log+"'");

    baseClose();

    return "Successfully";
}
string sqlbase::authoServ(string message)
{
    string log,pass;

    baseOpen();
    message.pop_back();
    message.pop_back();
    int pos = message.find("&");
    log = message.substr(0,pos);
    message.erase(0,pos+1);
    pos = message.find("&");
    pass = message.substr(0,pos);
    message.erase(0,pos+1);

    QString tlog,tpass;
    tlog = QString::fromStdString(log);
    tpass = QString::fromStdString(pass);
    QString tpass2 =tpass + tpass;
    QByteArray str = tpass2.toUtf8();
    QString password = QCryptographicHash::hash(str,QCryptographicHash::Sha256).toHex();
    QSqlQuery qry;
    string stata;

    if(qry.exec("select * from UserName where login='"+tlog+"' and password='"+password+"'"))
    {
        int count = 0;
        while(qry.next())
        {
            count++;
            QString stat = qry.value(2).toString();
            stata = stat.toStdString();
        }
        if(count==1)
        {
            baseClose();
            return stata;
        }
        if(count>1)
        {
            baseClose();
            return "Есть повторы";
        }
        if(count < 1)
        {
            baseClose();
            return "Неверный логин или пароль";
        }

    }
}

QString sqlbase::addDate(string message)
{
    QString abon;
    QString date_visit;
    baseOpen();

    int k = message.find("&");
    string ID = message.substr(0,k);
    message.erase(0,k+1);

    k = message.find("&");
    string FIO = message.substr(0,k);
    message.erase(0,k+1);

    k = message.find("&");
    string TEL = message.substr(0,k);
    message.erase(0,k+1);

    k = message.find("&");
    string DATE = message.substr(0,k);
    message.erase(0,k+1);

    QString id = QString::fromStdString(ID);
    QString fio = QString::fromStdString(FIO);
    QString tel = QString::fromStdString(TEL);
    QString date = QString::fromStdString(DATE);

    QSqlQuery qry;
    qry.prepare("SELECT * FROM DataBasePerson WHERE  NAME = '"+id+"' and FIO='"+fio+"' and TEL='"+tel+"'");

    if(qry.exec())
    {
        qry.last();
        date_visit = qry.value(4).toString();
        abon = qry.value(3).toString();
        QDate current = QDate::currentDate();
        QDate now = QDate::fromString(abon,"dd.MM.yyyy");

        if(now < current)
        {
            return"Абонемент закончился.\nОбратитесь к менеджеру за помощью";
        }

        if(date_visit=="")
        {
            QSqlQuery qry3;
            qry.exec("DELETE FROM DataBasePerson WHERE  NAME = '"+id+"' and FIO='"+fio+"' and Tel='"+tel+"'");
        }
    }
    else{
        return proof(qry).text();
    }

    qry.last();
    int ras = qry.value(5).toInt();
    char buff[256];
    ras+=1;
    _itoa_s(ras,buff,256,10);

    if(ras==20)
    {
        return"Абонемент закончился.\nОбратитесь к менеджеру за помощью";
    }

    QString pos = QString::number(ras);

    QSqlQuery qry2;
    qry2.prepare("INSERT INTO DataBasePerson (NAME,FIO,Tel,Abonement,DATA,Visit) VALUES('"+id+"','"+fio+"','"+tel+"','"+abon+"','"+date+"','"+pos+"')");

    if(qry2.exec())
    {
        QString rezult = "Successful";
        return rezult;
    }
    else{
        return "Ошибка";
    }


}
QString sqlbase::abonement(string message)
{
    QString name;
    QString FIO;
    QString abon;
    QString date_t;
    QString visit;
    baseOpen();
    QString str = QString::fromStdString(message);

    int k = message.find("&");
    string TEL = message.substr(0,k);
    message.erase(0,k+1);

    k = message.find("&");
    string DATE = message.substr(0,k);
    message.erase(0,k+1);

    QString tel = QString::fromStdString(checkTel(TEL));
    if(tel=="null")
    {
        return "неправильный номер";
    }

    QString date = QString::fromStdString(checkDate(DATE));

    if(date=="Неверная дата./nДата не должна быть меньше сегодняшнего дня и иметь правильный формат.")
    {
        return"Неверная дата./nДата не должна быть меньше сегодняшнего дня и иметь правильный формат.";
    }

    int pos = 0;
    QString vis = QString::number(pos);

    QSqlQuery qry2;
    qry2.prepare("SELECT * FROM DataBasePerson where Tel='"+tel+"'");
    if(qry2.exec())
    {
         qry2.last();
         name = qry2.value(0).toString();
         FIO = qry2.value(1).toString();
         abon = qry2.value(3).toString();
         date_t = qry2.value(4).toString();
         visit = qry2.value(5).toString();
    }
    int pos_t = 0;
    QString neumber = QString::number(pos_t);
    qry2.prepare("DELETE FROM DataBasePerson where Tel='"+tel+"'");
    qry2.exec();

    QSqlQuery qry;
    qry.prepare("INSERT INTO DataBasePerson(NAME,FIO,Tel,Abonement,Visit) VALUES ('"+name+"','"+FIO+"','"+tel+"','"+date+"','"+neumber+"')");

    if(qry.exec())
    {
        baseClose();
        return "Successful";
    }
    else {
        baseClose();
        return "Ошибка";
    }
}
