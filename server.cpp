#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server),
    myIp("140.116.116.60")
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);

    if(!tcpServer->listen(myIp,55555)){
        QMessageBox::critical(this, "point server", "cannot listen!");
    }
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connected()));


}

Server::~Server()
{
    delete ui;
}

void Server::connected()
{
    qDebug()<<"connected";
    ui->label->setText("connected");
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

void Server::readMessage()
{
    qDebug()<<"message in";
    //read Data from client "name: score"
    QDataStream in(clientConnection);
    in.startTransaction();
    QString name, score;
    in>>name>>score;
    list.push_back({name,score});


    //-------------------open file and read Data and sort Data---------------------------//
    QFile file("./rank.file");
    QDataStream stream(&file);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"1cannot open rank.file";
        return ;
    }

    QString pl, sc;
    while(!stream.atEnd()){
        stream>>pl>>sc;
        list.push_back({pl,sc});
    }
    std::sort(list.begin(),list.end());
    file.close();

    QFile file2("./rank.file");
    if(!file2.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"2cannot open rank.file2";
        return ;
    }
    QDataStream stream2(&file2);
    foreach(Player i, list){

        qDebug()<<i.name<<" "<<i.score;
        stream2<<i.name<<i.score;
    }
    file2.close();
    //---------------------------------------------------------------------------------//

    //send Result
    QByteArray byte;
    QDataStream out(&byte, QIODevice::WriteOnly);

    foreach(Player i, list){
        out<<i.name<<i.score;
    }
    qDebug()<<clientConnection->write(byte);

    disconnect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    disconnect(clientConnection, SIGNAL(readyRead()), this, SLOT(readMessage()));
    clientConnection->disconnectFromHost();
    list.clear();
    byte.clear();
}


