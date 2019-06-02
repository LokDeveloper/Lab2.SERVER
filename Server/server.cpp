#include "server.h"
#include <QDebug>
#include <QCoreApplication>
#include "autorize.h"
#include <QString>
#include <iostream>

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
    server_status = 0;
}

void Server::slotNewConnection()
{
    if(server_status == 1)
    {
        mTcpSocket = mTcpServer->nextPendingConnection();
        //mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(mTcpSocket, &QTcpSocket::readyRead,this,&Server::slotServerRead);
        connect(mTcpSocket,&QTcpSocket::disconnected,this,&Server::slotClientDisconnected);
    }

}

void Server::slotClientDisconnected()
{
//    if(server_status == 1)
//    {
//        mTcpSocket->close();
//        mTcpServer->close();
//        qDebug() << QString::fromUtf8("Server is stopped!");
//        server_status = 0;
//    }
}
void Server::slotServerRead()
{
    QTcpSocket *clientSocket = (QTcpSocket*)sender();
    int id =(int)clientSocket->socketDescriptor();
    if(clientSocket->bytesAvailable() > 0)
    {
        QByteArray array =clientSocket->readAll();
        qDebug() << array;
        string log = "";
        string pass = "";
        string message; 
        message = array.toStdString();
        qDebug() << QString::fromStdString(message);
        int pos = message.find("&");
        message.erase(0,pos+1);

        pos = message.find("&");
        log = message.substr(0,pos);
        message.erase(0,pos+1);

        pos = message.find("&");
        pass = message.substr(0,pos);


        QString log2 =  QString::fromStdString(log);
        QString pass2 = QString::fromStdString(pass);
        string rezult=autorizes(log2,pass2);


        qDebug() << "login= " << log2 << "password= " << pass2;
        qDebug() << "rezult = " << QString::fromStdString(rezult);


        clientSocket->write(QByteArray::fromStdString(rezult));
        //clientSocket->write("\r\n");

    }
}
