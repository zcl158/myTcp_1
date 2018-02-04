#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QNetworkInterface>
#include <QtNetwork>
#include <QMessageBox>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::Server *ui;

    QTcpServer *tcpServer;
    QTcpSocket *clientSocket;
    //QList<QTcpSocket *> Socketlist;
\
    void flushLocalIP();

    void serverSend(QByteArray &sendData);

private  slots:

    void serverRecv();

    void displayErrorS(QAbstractSocket::SocketError); //显示服务端错误
    void NewConnected();  //新的连接进来创建socket
    void ClientDisconn();   //断开连接

    void on_sCleanpushButton_clicked();
    void on_listen_Button_clicked();

    void on_send_Button_clicked();
};

#endif // SERVER_H
