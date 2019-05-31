#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <string>
#include "database.h"
#include"sqlbase.h"
#include "crypto.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject * parent = nullptr);
    ~Server();

 public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();
 private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;
    int server_status;
    QMap <int,QTcpSocket*> sclients;
    int currentusers = 0;
 public:
    string parsing(vector<dataa>);
    vector<sqldata> parsingMessage(string,vector<sqldata>,sqldata);
    string parsingVector(vector<dataa>);
};

#endif // SERVER_H
