#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QLabel>
#include <QMessageBox>

#include <QFile>
#include <QList>
#include <algorithm>
#include <QByteArray>
#include <QTextStream>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>


namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;
    const QHostAddress myIp;

    QTcpServer *tcpServer;

    struct Player{
        QString name;
        int score;

        bool operator< (const struct Player &t) const{
            return score>t.score;
        }
    };

private slots:
    void connected();
};

#endif // SERVER_H
