#ifndef QTALKCLIENT_H
#define QTALKCLIENT_H

#include <QObject>

#include <QTcpSocket>
#include <QUrl>
#include <QHostAddress>
#include <QHostInfo>
#include <QTimer>
#include <QFile>


class QTalkClient : public QObject
{
    Q_OBJECT
public:
    explicit QTalkClient(QObject *parent = nullptr);

public slots:
    void connectToServer(QString serverAddress, quint16 port);
    void dissconnectFromServer();
    void sendMessage(QString message);
    void fetchAvailableServers();

private:
    QTcpSocket* clientSocket;
    QTimer *messageCheckTimer;

    void checkAvailableMessages();

signals:
    void clientStateChanged(QString state);
    void newMessageReceived(QString messageData);
    void availableServersFetched(QStringList servers);
};

#endif // QTALKCLIENT_H
