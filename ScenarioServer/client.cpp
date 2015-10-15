#include "client.h"
/**
 * @brief Client::Client
 * @param sock
 * @param parent
 */
Client::Client(QTcpSocket * sock, QObject *parent) : QObject(parent)
{

    this->socket = sock;
    QHostAddress hostAddr = socket->peerAddress();
    this->ipAddrStr = hostAddr.toString();
    connect(this->socket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(this->socket,SIGNAL(aboutToClose()), this,SLOT(socketAboutToClose()));
    connect(this->socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    connect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    _connectionState = Client::ON;
    if (disconnectTimer) {
        disconnectTimer->stop();
        disconnectTimer=NULL;
    } else {
        disconnectTimer = NULL;
    }
}
/**
 * @brief Client::setSocket
 * @param soc
 */
void Client::setSocket(QTcpSocket *soc) {
    // disconnect the old socket signals
    disconnect(this->socket,SIGNAL(readyRead()),this,SLOT(readData()));
    disconnect(this->socket,SIGNAL(aboutToClose()), this,SLOT(socketAboutToClose()));
    disconnect(this->socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    disconnect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    // replace the old socket by a new one
    this->socket = soc;
    // reconnect the new socket signals to the CLient's class slots
    connect(this->socket,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(this->socket,SIGNAL(aboutToClose()), this,SLOT(socketAboutToClose()));
    connect(this->socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    connect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    _connectionState = Client::ON;
    if (disconnectTimer) {
        disconnectTimer->stop();
        disconnectTimer=NULL;
    }
}
/**
 * @brief Client::~Client
 */
Client::~Client()
{

    disconnect(this->socket,SIGNAL(readyRead()),this,SLOT(readData()));
    disconnect(this->socket,SIGNAL(aboutToClose()), this,SLOT(socketAboutToClose()));
    disconnect(this->socket,SIGNAL(disconnected()),this,SLOT(socketDisconnected()));
    disconnect(this->socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    this->socket = NULL;
    this->ipAddrStr = "";

}
/**
 * @brief Client::readData
 */
void Client::readData() {
    QByteArray bytes  = socket->readAll();
    QString st(bytes);
    QStringList messageArgs = st.split(":");
    int len = messageArgs.size();
    if (len < 2 ) {
        printf("Not enough arguments\n");
        fflush(NULL);
        return;
    }
    QString command = messageArgs.at(0);
    if (command.startsWith("Position")) {
        this->rank = messageArgs.at(1).toInt();
    } else if (command.startsWith("RecordValues")) {
        // Write into a file the recorded values from the table
        emit downloadFinished(st);
    }
}

/**
 * @brief Client::socketAboutToClose
 */
void Client::socketAboutToClose() {
    cout << "Connection with the server is about to close" << endl;
    // must take care of storing all the required data
}

/**
 * @brief Client::socketDisconnected
 */
void Client::socketDisconnected() {
    cout << "Socket disconnected from server" <<endl;
    _connectionState = Client::OFF;
    emit disconnectedFromDevice(this->rank);
}

/**
 * @brief Client::errorWithSocket
 */
void Client::errorWithSocket(QAbstractSocket::SocketError e) {
     _connectionState = Client::OFF;
     cout << e << endl;
     //emit disconnectedFromDevice(this->rank);
     if (!disconnectTimer) {
         disconnectTimer = new QTimer();
         disconnectTimer->setInterval(60000);
         connect(disconnectTimer,SIGNAL(timeout()),this,SLOT(disconnectTimerFired()));
     }
}
/**
 * @brief Client::bytesWritten
 * @param bytesWritten
 */
void Client::bytesWritten(qint64 bytesWritten) {
    //
    Q_UNUSED(bytesWritten);

}


void Client::disconnectTimerFired() {
    emit disconnectedFromDevice(this->rank);
}

