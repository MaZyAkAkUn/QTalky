#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void gui_setServerStatus(QString status);
    void gui_setClientStatus(QString status);

    void gui_newMessageReceived(QString clientId, QString messageData);
    void gui_newMessageReceivedSimple(QString messageData);
    void gui_clientStateChanged(QString clientId, int state);

    void gui_fillAvailableServers(QStringList servers);

signals:
    void gui_runServerRequested();
    void gui_stopServerRequested();

    void gui_connectToServerRequested(QString serverAddress, quint16 port);
    void gui_dissconnectFromServerRequested();

    void gui_sendMessageRequested(QString message);

    void gui_fetchAvailableServersRequested();
private:
    QTextEdit* te_messages;
    QPushButton* pb_sendMessage;
    QVBoxLayout* ly_main;

    QLineEdit* le_messageField;


    QLabel* lb_serverStatusLb;
    QLabel* lb_serverStatus;
    QPushButton* pb_switchServerState;

    QLabel* lb_clientStatusLb;
    QLabel* lb_clientStatus;

    QLineEdit* le_serverAddress;
    QPushButton* pb_connectToServer;

    QLineEdit* le_nickname;

    QPushButton* pb_fetchAvailableServers;
    QComboBox* cb_availableServers;



private slots:
    void sendMessage();


};
#endif // MAINWINDOW_H
