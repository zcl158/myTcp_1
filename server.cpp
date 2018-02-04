#include "server.h"
#include "ui_server.h"
#include "zcltools.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    this->flushLocalIP();
    ui->serverIP_comboBox->installEventFilter(this);

    ui->input_lineEdit->setValidator((new QRegExpValidator(QRegExp("^[0-9a-fA-F\u0020]+$"),this)));
}

Server::~Server()
{
    delete ui;
}

bool Server::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
       if (obj == ui->serverIP_comboBox)    //插入本地IP地址
       {
            this->flushLocalIP();
       }
    }

    return QWidget::eventFilter(obj, event);
}

void Server::flushLocalIP()
{
     ui->serverIP_comboBox->clear();
     QList<QHostAddress> IPlist = QNetworkInterface::allAddresses();
     foreach (QHostAddress IP, IPlist)
     {
        if(IP.protocol()==QAbstractSocket::IPv4Protocol)
            ui->serverIP_comboBox->addItem (IP.toString());
     }
}



//服务器端读取信息
void Server::serverRecv()
{
    QByteArray recvData;
    recvData.append(clientSocket->readAll());
    //先缓存数据，再调用定时器，设置一定时间后，再处理缓存中的数据
    //qDebug()<<"receive uart data cache: " <<QString(*receiveDat)<<endl;
    //receiveHandleTimer->start(20); //毫秒

    ui->recv_Browser->insertPlainText("\n");
    for(int i = 0 ; i < recvData.count(); i++)
    {
        uchar outChar = recvData.at(i);
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));  //十六进制的转换
        ui->recv_Browser->insertPlainText(str.toUpper());//大写
        ui->recv_Browser->insertPlainText(" ");//每发送两个字符后添加一个空格
    }
}

//服务器端发送信息
void Server::serverSend(QByteArray &sendData)
{    
    clientSocket->write(sendData);

    ui->send_Browser->insertPlainText("\n");
    for(int i = 0 ; i < sendData.count(); i++)
    {
        uchar outChar = sendData.at(i);
        QString str = QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));  //十六进制的转换
        ui->send_Browser->insertPlainText(str.toUpper());//大写
        ui->send_Browser->insertPlainText(" ");//每发送两个字符后添加一个空格
    }
}

//侦听按钮槽函数
void Server::on_listen_Button_clicked()
{
    static char flag = 0;  //用于标志按钮的状态
    if( 0 == flag)
    {
        QHostAddress serverIP(ui->serverIP_comboBox->currentText());
        ushort serverPort = ui->serverPortlineEdit->text().toUShort();

        tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(serverIP, serverPort)) {
            QMessageBox::warning (this, tr("Warnning"), tcpServer->errorString());
            close ();
        }
        else {
            connect (tcpServer, SIGNAL(newConnection()), this, SLOT(NewConnected())); //有新的连接到来，则开始创建套接字

            flag = 1;
        }
    }
    else
    {
        tcpServer->close();
        delete tcpServer;

        flag = 0;
    }

    if(0 == flag) //UI相关
    {
        ui->listen_Button->setText("侦听");
        ui->serverIP_comboBox->setEnabled(true);
        ui->serverPortlineEdit->setEnabled(true);
    }
    else //侦听中
    {
        ui->listen_Button->setText("断开");
        ui->serverIP_comboBox->setEnabled(false);
        ui->serverPortlineEdit->setEnabled(false);
    }

}



//服务器端创建套接字
void Server::NewConnected()
{

    clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, SIGNAL(disconnected()), this, SLOT(ClientDisconn()));
    connect(clientSocket, SIGNAL(readyRead()), this, SLOT(serverRecv()));

    ui->send_Browser->insertPlainText("\n");
    ui->send_Browser->insertPlainText(QString::asprintf("%s:%d 连接",
                        clientSocket->peerAddress().toString().toStdString().c_str(),
                        clientSocket->peerPort()));
}

void Server::ClientDisconn()
{
    clientSocket->close();

    ui->send_Browser->insertPlainText("\n");

    ui->send_Browser->insertPlainText(QString::asprintf("%s:%d 断开",
                        clientSocket->peerAddress().toString().toStdString().c_str(),
                        clientSocket->peerPort()));
}

 //服务器端错误提示
void Server::displayErrorS(QAbstractSocket::SocketError)
{
  //  QMessageBox::warning (this, tr("Warnning"), clientConnection->errorString ());
    ui->send_Browser->insertPlainText(clientSocket->errorString());
//    connect (clientSocket, SIGNAL(disconnected()), clientSocket, SLOT(deleteLater()));
    clientSocket->disconnectFromHost();
}


//清空按钮槽函数
void Server::on_sCleanpushButton_clicked()
{
    ui->send_Browser->clear();
    ui->recv_Browser->clear();
}


void Server::on_send_Button_clicked()
{
 //   QByteArray txbuff =QByteArray::fromHex( ui->input_lineEdit->text().toLatin1());
    QByteArray txbuff;
    QString temp = ui->input_lineEdit->text();
    txbuff.clear();
    zclTools::strToHex(temp.toStdString().c_str(), txbuff);
    this->serverSend( txbuff);
    ui->input_lineEdit->clear();
}






