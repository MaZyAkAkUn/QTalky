#ifndef QTALKCLIENT_H
#define QTALKCLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QUrl>
#include <QHostAddress>
#include <QHostInfo>
#include <QTimer>
class QTalkClient : public QObject
{
    Q_OBJECT
public:
    explicit QTalkClient(QObject *parent = nullptr);

public slots:
    void connectToServer(QString serverAddress, quint16 port);
    void dissconnectFromServer();
    void sendMessage(QString message);
private:
    QTcpSocket* clientSocket;
    QTimer *messageCheckTimer;

    void checkAvailableMessages();

signals:
    void clientStateChanged(QString state);
    void newMessageReceived(QString messageData);

};

#endif // QTALKCLIENT_H
