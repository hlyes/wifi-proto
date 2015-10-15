#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <client.h>
#include <iostream>
#define INITIALIZATION 0
#define WAITING_FOR_CONNECTIONS 1
#define BENCHMARKING 2


using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //Publicly exposed fields/methods/constructors/destructors
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//Private slots
private slots:
    void on_pushButton_clicked();
    void newConnection();
    void clientLeft(int rank);


// Private fields and methods
    void on_startButton_clicked();

    void onDownloadFinished(QString &message);
private:

    // Methods
    void removeValue(int value);
    QString findClientKey(int rank);

    void onStepFinished(int step);
    void startBenchmarking();

    // Fields
    Ui::MainWindow *ui;
    QTcpServer *server;
    QMap<QString ,Client*> clients;
    QList<qint32> ranks;

    // Ranks
    qint32 maxConnectedClients;
    qint32 currentConnections;
    qint32 currentStep;
    qint32 endedOperations;
    qint32 maxSteps;
    QList <Client *> clientsList;
};

#endif // MAINWINDOW_H
