#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currTemp(0.0)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    connect(&server, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTemperature);

    if(server.listen(QHostAddress("127.0.0.1"), 9999)) {
        logMessage("Server started");
    }
    else {
        logMessage("Server failed to start");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTemperature() {
    currTemp += 0.1;
    ui->temperatureLineEdit->setText(QString::number(currTemp));
}

void MainWindow::logMessage(const QString &msg) {
    ui->logTextEdit->append(msg);
}

void MainWindow::onNewConnection() {
    QTcpSocket *socket = server.nextPendingConnection();
    logMessage("Client connected");

    connect(socket, &QTcpSocket::readyRead, this, [=]()
    {
        QByteArray data = socket->readAll();
        qDebug() << "data hex =" << data.toHex();
        QString command = QString(data).trimmed();
        logMessage("Recieved: " + command);
        QString response;

        if(command == "GET_TEMP") {
            response = ui->temperatureLineEdit->text() + "\n";
        }
        else if(command.startsWith("SET_TEMP ")) {
            float temp = command.mid(9).toFloat();
            currTemp = temp;
            ui->temperatureLineEdit->setText(command.mid(9));
        }
        else if(command == "GET_STATUS") {
            if(ui->pushButton->text() == "STOP") {
                response = "RUN\n";
            }
            else {
                response = "STOP\n";
            }
        }
        else if(command.startsWith("SET_STATUS ")) {
            QString status = command.mid(11);
            ui->pushButton->setText(status);
            if(status == "STOP") {
                timer->stop();
            }
            else {
                currTemp = 0.0;
                ui->temperatureLineEdit->setText(QString::number(currTemp));
                timer->start(100);
            }
        }
        else {
            response = "ERROR\n";
        }

        socket->write(response.toUtf8());
        socket->flush();

        logMessage("Sent: " + response.trimmed());
    });

    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "RUN") {
        currTemp = 0.0;
        ui->temperatureLineEdit->setText(QString::number(currTemp));
        ui->pushButton->setText("STOP");
        timer->start(100);
    }
    else {
        ui->pushButton->setText("RUN");
        timer->stop();
    }


}

