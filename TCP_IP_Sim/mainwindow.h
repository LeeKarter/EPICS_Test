#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onNewConnection();
    void updateTemperature();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    void logMessage(const QString &msg);

    QTcpServer server;
    QTimer *timer;
    float currTemp;
};
#endif // MAINWINDOW_H
