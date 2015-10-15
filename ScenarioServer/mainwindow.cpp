#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->server = new QTcpServer();
    server->setMaxPendingConnections(ui->iPadCount->value());
    server->listen(QHostAddress::Any, 12345);
    connect (server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    // Fill the ranks with positions
    for (int i =  0; i < ui->iPadCount->value() ;  i++) {
        ranks.append(i);
    }
    ui->iPadCount->setEnabled(false);
    ui->startButton->setEnabled(false);
    ui->tabWidget->setCurrentIndex(WAITING_FOR_CONNECTIONS);
    ui->settings->setEnabled(false);
    maxConnectedClients = ui->iPadCount->value();
    maxSteps = maxConnectedClients;
    currentConnections = 0;
    currentStep = 0;
}



void MainWindow::newConnection() {

    QTcpSocket *s = server->nextPendingConnection();
    QString address = s->peerAddress().toString();
    Client *client = clients.value(address);
    if (!client) {
        // If the client does not exist
        if (ranks.size() == 0 ) {
            s->write("ServerIsFull\n\n");
            s->flush();
            s->close();
            return;
        } else {
            Client *c = new Client(s);
            int rank  = ranks.at(0);
            ranks.pop_front();
            c->setRank(rank);
            clients.insert(s->peerAddress().toString(),c);
            connect(c,SIGNAL(disconnectedFromDevice(int)),this,SLOT(clientLeft(int)));
        }
        if (ranks.size()==0) {
            ui->tabWidget->setCurrentIndex(BENCHMARKING);
        }
    } else {
        client->setSocket(s);
    }
}

void MainWindow::removeValue(int value) {
    for (int  i  = 0 ; i < ranks.size() ; i ++ ){
        if (ranks.at(i)==value)  {
            ranks.removeAt(i);
            return;
        }
    }
}

QString MainWindow::findClientKey(int rank) {
    QList<Client*>cls = clients.values();
    for (int  i= 0 ; i < cls.length(); i++) {
        if (cls.value(i)->getRank() ==rank) {
            return cls.value(i)->getAddress();
        }
    }
    return QString("NONE");
}




void MainWindow::on_startButton_clicked() {
    this->on_pushButton_clicked();
}

/**
 * @brief MainWindow::clientLeft
 * @param rank
 * We consider that the client don't leave the network
 */
void MainWindow::clientLeft(int rank) {
    QString key = findClientKey(rank);
    if (key.startsWith("NONE")) {
        return;
    }
    Client* leavingClient = clients.value(key);
    if (leavingClient) {
        ranks.push_back(leavingClient->getRank());
        qSort(ranks);
    }

}



void MainWindow::startBenchmarking() {
    clientsList = clients.values();
    qSort(clientsList);
    currentStep = 1;
    endedOperations = 0;
    for (int i = 0; i < currentStep ; i++) {
        Client * c = clientsList.at(i);
        c->getSocket()->write(QString("GO %1\n\n").arg(currentStep).toStdString().c_str());
        c->getSocket()->flush();
    }

}


void MainWindow::onStepFinished(int step) {

    if (currentStep == maxSteps) {
        // Show a message which says that the measures have ended

        return;
    }
    // go to next Step
    currentStep = step +  1;
    for (int i = 0; i < currentStep ; i++) {
        Client * c = clientsList.at(i);
        c->getSocket()->write(QString("GO %1\n\n").arg(currentStep).toStdString().c_str());
        c->getSocket()->flush();
    }

}


void MainWindow::onDownloadFinished(QString &message) {
    QStringList list = message.split(":");
    list.pop_front();
    // @TODO read the values and store them into a file
    endedOperations ++;
    if (endedOperations) {
        this->onStepFinished(currentStep);
    }
}
