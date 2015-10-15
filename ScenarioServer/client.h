#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <iostream>
#include <QTimer>
using namespace std;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QTcpSocket * socket, QObject *parent = 0);
    ~Client();
    void setRank(int r) { rank = r; }
    int getRank() { return rank; }
    void setSocket(QTcpSocket * soc);
    QTcpSocket *getSocket() { return  this->socket; }
    QString getAddress() { return ipAddrStr; }
    enum ConnectionState {
        ON,OFF
    };
    Client::ConnectionState connectionState() { return _connectionState; }
    bool operator !=(Client &cl) { return getRank() != cl.getRank(); }
    bool operator ==(Client &cl2) { return getRank() == cl2.getRank(); }
    bool operator > (Client &cl2) { return getRank() > cl2.getRank(); }
    bool operator < (Client &cl2) { return getRank() < cl2.getRank(); }
    bool operator >= (Client &cl2) { return getRank() >= cl2.getRank(); }
    bool operator <= (Client &cl2) { return getRank() <= cl2.getRank(); }
signals:
    void disconnectedFromDevice(int rank);
    void downloadFinished(QString message);
public slots:
    void readData();
    void socketAboutToClose();
    void socketDisconnected();
    void bytesWritten(qint64 bytes_written);
    void errorWithSocket(QAbstractSocket::SocketError e);
    void disconnectTimerFired();
private:
    QTcpSocket *socket;
    QTimer *disconnectTimer;
    int rank;
    QString ipAddrStr;
    Client::ConnectionState _connectionState;
};

#endif // CLIENT_H
