#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Server),
    myIp("140.116.116.60")
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);

    if(!tcpServer->listen(myIp,12345)){
        QMessageBox::critical(this, "point server", "cannot listen!");
    }
    qDebug()<<tcpServer->serverPort();
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(connected()));

    //-------------------open file and read Data and sort Data---------------------------//
    QFile file("./rank.file");
    QList<Player> list;
    list.push_back({"hi_buddy",19999});
    QTextStream stream(&file);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"1cannot open rank.file";
        file.open(QIODevice::WriteOnly);
        file.close();
        return ;
    }
    QString pl; int sc;
    while(!stream.atEnd()){
        stream>>pl>>sc;
        list.push_back({pl,sc});
    }
    std::sort(list.begin(),list.end());
    file.close();

    file.setFileName("./rank.file");
    if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"2cannot open rank.file";
        return ;
    }
    QTextStream stream2(&file);
    foreach(Player i, list){
        qDebug()<<i.name<<" "<<QString::number(i.score);
        stream2<<i.name<<" "<<QString::number(i.score)<<" ";
    }
    file.close();

    //---------------------------------------------------------------------------------//


}

Server::~Server()
{
    delete ui;
}

void Server::connected()
{
    ui->label->setText("connected");
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    QList<Player> list;
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    //client will send name: score
    //read Data from client
    QDataStream in;
    in.startTransaction();
    int score;
    QString name;
    while(!in.atEnd()){
        in>>name>>score;
        list.push_back({name,score});
    }

    list.push_back({"hi_buddy",1999});

    //-------------------open file and read Data and sort Data---------------------------//
    QFile file("./rank.file");
    QTextStream stream(&file);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"1cannot open rank.file";
        return ;
    }
    QString pl; int sc;
    while(!stream.atEnd()){
        stream>>pl>>sc;
        list.push_back({pl,sc});
    }
    std::sort(list.begin(),list.end());
    file.close();

    file.setFileName("./rank.file");
    if(!file.open(QIODevice::Truncate|QIODevice::WriteOnly)){
        qDebug()<<"2cannot open rank.file";
        return ;
    }
    QTextStream stream2(&file);
    foreach(Player i, list){
        qDebug()<<i.name<<" "<<QString::number(i.score);
        stream2<<i.name<<" "<<QString::number(i.score)<<" ";
    }
    file.close();

    //---------------------------------------------------------------------------------//




    //send Result

    clientConnection->disconnectFromHost();
}
