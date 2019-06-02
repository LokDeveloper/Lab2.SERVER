#ifndef SERVER_H
#define SERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include<string>
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
};

#endif // SERVER_H
