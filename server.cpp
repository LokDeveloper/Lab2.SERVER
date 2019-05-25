#include "server.h"
#include <QDebug>
#include <QCoreApplication>
#include "autorize.h"
#include <QString>
#include <iostream>
#include<database.h>

Server::Server(QObject *parent ) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer,&QTcpServer::newConnection,this,&Server::slotNewConnection);
    if(!mTcpServer->listen(QHostAddress::Any,33333))
    {
        qDebug() << "server is not started";
    }
    else {
        server_status = 1;
        qDebug() << "server is started";
    }
}
Server::~Server()
{
    foreach(int id, sclients.keys())
    {
        QTextStream os(sclients[id]);
        sclients[id]->close();
        sclients.remove(id);
    }
    server_status = 0;
    mTcpServer->close();
}

void Server::slotNewConnection()
{
    if(server_status == 1)
    {
        mTcpSocket = mTcpServer->nextPendingConnection();
        int id=(int)mTcpSocket->socketDescriptor();
        sclients[id]=mTcpSocket;
        connect(sclients[id], &QTcpSocket::readyRead, this, &Server::slotServerRead);
        connect(sclients[id], &QTcpSocket::disconnected, this, &Server::slotClientDisconnected);
        currentusers+=1;

        qDebug() << "User join" ;
        qDebug() << "Current Users :" << currentusers ;

        sclients[id]->write("Пожалуйста,скажите кто вы:\r\n");
        sclients[id]->write("Команда авторизации-auth\r\n");
        sclients[id]->waitForBytesWritten(200);
        sclients[id]->readAll();
    }

}

void Server::slotClientDisconnected()
{
    if(server_status==1){
        QTcpSocket* clientSocket = (QTcpSocket*)sender();
        int id=(int)clientSocket->socketDescriptor();
        sclients[id]->close();
        currentusers-=1;
        qDebug() << "User leave" ;
        qDebug() << "Current users:" << currentusers ;
    }
}
void Server::slotServerRead()
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    int id =(int)clientSocket->socketDescriptor();


    if(clientSocket->bytesAvailable() > 0)
    {
        sqlbase *obj;
        string message,temp;
        QByteArray array = clientSocket->readAll();

        message = array.toStdString();

        int k = message.find("&");
        string crypt = message.substr(0,k);
        message.erase(0,k+1);

        if(crypt=="shifr")
        {
            array.remove(0,k+1);
            QString deshifr = crypto.decryptToString(array);
            message = deshifr.toStdString();
        }
        else{
            message = array.toStdString();
        }

        k = message.find("&");
        temp = message.substr(0,k);
        message.erase(0,k+1);



        if(temp == "autho")
        {
            string log,pass;
            string rezult = obj->autho(message);
            if(rezult == "admin")
                clientSocket->setObjectName("admin");
            if(rezult == "manager")
                clientSocket->setObjectName("manager");
            if(rezult == "user")
                clientSocket->setObjectName("user");
            QByteArray rez = crypto.encryptToByteArray(QString::fromStdString(rezult));

            clientSocket->write(rez);
            clientSocket->waitForBytesWritten(200);
            obj->baseClose();
        }
        else if(temp == "auth")
        {
            string log,pass;
            string rezult = obj->authoServ(message);
            if(rezult == "admin")
                clientSocket->setObjectName("admin");
            if(rezult == "manager")
                clientSocket->setObjectName("manager");
            if(rezult == "user")
                clientSocket->setObjectName("user");

            clientSocket->write(QByteArray::fromStdString(rezult));
            clientSocket->waitForBytesWritten();
            obj->baseClose();
        }
        else if(temp =="show" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager" || clientSocket->objectName()=="user"))
        {
            QString rezult;
            sqlbase *obj;

            rezult=obj->showPersonAll();

            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(100);
            obj->baseClose();
        }

        else if(temp =="add" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager"))
        {
            //DataBase *obj;
            vector<sqldata> db;
            vector<sqldata> temp_db;

            sqldata obj2;


            db = parsingMessage(message,temp_db,obj2);

            QString fail = obj->addPerson(db);
            clientSocket->write(QByteArray::fromStdString(fail.toStdString()));
            clientSocket->waitForBytesWritten(200);
            obj->baseClose();
        }

        else if(temp=="del" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager"))
        {
            sqlbase *obj;
            vector<dataa> db;


            QString fail = obj->delPerson(QString::fromStdString(message));

            clientSocket->write(QByteArray::fromStdString(fail.toStdString()));
            clientSocket->waitForBytesWritten(100);

            obj->baseClose();

        }

        else if(temp=="change" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager" ))
        {
            sqlbase *obj;
            vector<sqldata> db;
            sqldata temp;

            db = parsingMessage(message,db,temp);

            QString fail = obj->changePerson(db);

            clientSocket->write(QByteArray::fromStdString(fail.toStdString()));
            clientSocket->waitForBytesWritten(100);

            obj->baseClose();
        }
        else if(temp == "changeForUser" && clientSocket->objectName()=="user")
        {
            sqlbase obj;

            QString rezult = obj.changeForUser(QString::fromStdString(message));

            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(200);
        }
        else if(temp=="find" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager" ))
        {
            sqlbase obj;

            QString rezult = obj.findPerson(QString::fromStdString(message));


            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(100);
            obj.baseClose();
        }
        else if(temp =="logBase" && clientSocket->objectName()=="admin")
        {
            sqlbase obj;
            QString rezult = obj.baseLogPass();
            QByteArray shifr = crypto.encryptToByteArray(rezult);

            clientSocket->write(shifr);
            clientSocket->waitForBytesWritten(1000);
        }
        else if(temp == "changeLog" && clientSocket->objectName()=="admin")
        {
            sqlbase obj;

            QString rezult = obj.changePassword(QString::fromStdString(message));

            QByteArray rez = QByteArray::fromStdString(rezult.toStdString());

            clientSocket->write(rez);
            clientSocket->waitForBytesWritten(1000);
        }
        else if(temp == "addLog" && clientSocket->objectName()=="admin")
        {
            sqlbase obj;
            QString rezult = obj.addLogin(QString::fromStdString(message));

            QByteArray rez = QByteArray::fromStdString(rezult.toStdString());

            clientSocket->write(rez);
            clientSocket->waitForBytesWritten(1000);
        }
        else if(temp == "delLog" && clientSocket->objectName()=="admin")
        {
            sqlbase obj;
            QString rezult = obj.delLogin(QString::fromStdString(message));

            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(200);
        }
        else if(temp == "Adddate" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager" ))
        {
            sqlbase obj;
            QString rezult = obj.addDate(message);

            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(200);
        }
        else if(temp == "abonement" && (clientSocket->objectName()=="admin" || clientSocket->objectName()=="manager" ))
        {
            sqlbase obj;
            QString rezult;

            rezult = obj.abonement(message);
            clientSocket->write(QByteArray::fromStdString(rezult.toStdString()));
            clientSocket->waitForBytesWritten(200);
        }

        else {
            clientSocket->write("Неизвестная команда или неверный ключ\r\n");
            obj->baseClose();
        }

    }
}
vector<sqldata> Server::parsingMessage(string temp,vector<sqldata> db,sqldata obj)
{

    int n;
    n = temp.find("&");
    string ID = temp.substr(0,n);
    obj.id = ID;
    temp.erase(0,n+1);

    n = temp.find("&");
    obj.fio = temp.substr(0,n);
    temp.erase(0,n+1);

    n = temp.find("&");
    obj.tel = temp.substr(0,n);
    temp.erase(0,n+1);

    n = temp.find("&");
    obj.date = temp.substr(0,n);
    temp.erase(0,n+1);

    db.push_back(obj);

    return db;
}

string Server::parsing(vector<dataa> db)
{
    string temp;
    string id;
    string fio;
    string tel;
    string date;

    for(vector<dataa>::iterator it = db.begin();it!=db.end();it++)
    {
        id = it->id;
        fio = it->fio;
        tel = it->tel;
        date = it->date;
        temp = id + "&" + fio + "&" + tel +"&" + date + "&";
    }

    return temp;
}

string Server::parsingVector(vector<dataa>db)
{
    string temp;

    for(vector<dataa>::iterator it = db.begin();it!=db.end();it++)
    {

        temp += it->id + "&" + it->fio + "&" + it->tel +"&" + it->date +"&";
    }
    return temp;
}

