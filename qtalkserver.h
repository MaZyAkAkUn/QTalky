#ifndef QTALKSERVER_H
#define QTALKSERVER_H

#include <QObject>

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QUuid>
#include <QTimer>
#include <QQueue>
#include <QPair>
#include <QByteArray>
#include <QList>


#include <QMutex>
#include <QMutexLocker>
class QTalkServer : public QObject
{
    Q_OBJECT
public:
    explicit QTalkServer(QObject *parent = nullptr);
    QString generateUniqueClientId();
private:
    QTcpServer* tcps_server;
    QMap<QString, QTcpSocket*> m_connectedClients;
    QQueue<QPair<QString, QString>> broadcastMessageQueue;
    bool sendMessageToClientSocket(QTcpSocket* socket, QString message);

    QTimer *messageCheckTimer;
    QTimer *messageBroadcastTimer;

    QMutex messageBroadcastQueueLock;

signals:
    void serverStateChanged(QString serverState);
    void clientStateChanged(QString clientId, int state);

    void newMessageReceived(QString clientId, QString message);

public slots:
    void startServer();
    void stopServer();

private slots:
    void handleNewConnection();
    void checkAvailableMessages();
    void broadcastQueuedMessages();
};

#endif // QTALKSERVER_H
