#include "qtalkserver.h"

QTalkServer::QTalkServer(QObject *parent)
    : QObject{parent}
{
    tcps_server = new QTcpServer(this);
    connect(tcps_server, &QTcpServer::newConnection, this, &QTalkServer::handleNewConnection);


}

QString QTalkServer::generateUniqueClientId()
{
    QUuid uuid = QUuid::createUuid();


    return uuid.toString();
}

bool QTalkServer::sendMessageToClientSocket(QTcpSocket* socket, QString message)
{
    //here just send message to socket corresponding to clientId val
    auto bytesWritten = socket->write(message.toStdString().c_str());
    socket->flush();
    qDebug()<<"Broadcasting message, "<<bytesWritten<<" bytes written;";

    if(bytesWritten > 0)return true;
    else return false;
}

void QTalkServer::startServer()
{
    tcps_server->listen(QHostAddress::Any , 0);
    if(tcps_server->isListening()){
        qDebug()<<"Server listening on :"<<tcps_server->serverAddress().toString()<< ":"<<tcps_server->serverPort();

//        broadcastMessageQueue = new QQueue<QPair<QString, QString>>();

        messageCheckTimer = new QTimer(this);
        messageCheckTimer->setInterval(1000);
        connect(messageCheckTimer, &QTimer::timeout, this, &QTalkServer::checkAvailableMessages);
        messageCheckTimer->start();

        messageBroadcastTimer = new QTimer(this);
        messageBroadcastTimer->setInterval(3000);
        connect(messageBroadcastTimer, &QTimer::timeout, this, &QTalkServer::broadcastQueuedMessages);
        messageBroadcastTimer->start();
        QString serverState = QString("Listen on: "+tcps_server->serverAddress().toString() + ":"+ QString::number(tcps_server->serverPort()));
        emit serverStateChanged(serverState);
        //now we can accept new connections
    }


}

void QTalkServer::stopServer()
{
    tcps_server->close();
    messageCheckTimer->stop();
    messageBroadcastTimer->stop();
    emit serverStateChanged("Stoped");

}



void QTalkServer::handleNewConnection()
{
    //here we got a new connection, lets care it
    if(tcps_server->hasPendingConnections()){
        QTcpSocket* clientConn = tcps_server->nextPendingConnection();
        //here we got a conn socket, so we can read/write from/to it
        //just make unique id, for client , and put into a map
        QString clientId = generateUniqueClientId();
        connect(clientConn, &QTcpSocket::disconnected, this, [this, clientId](){
            emit this->clientStateChanged(clientId, 0);
        });

        m_connectedClients[clientId] = clientConn;
        emit clientStateChanged(clientId, 1);
    }
}

void QTalkServer::checkAvailableMessages()
{
    //here check if clients sockets have a new data available to read
    for(auto socket: m_connectedClients){
        if(socket->bytesAvailable() > 0){
            auto data = socket->readAll();
            QString messageData = QString::fromStdString(data.toStdString());

            emit newMessageReceived(m_connectedClients.key(socket), messageData);
            messageBroadcastQueueLock.lock();
            broadcastMessageQueue.enqueue(QPair<QString, QString>(m_connectedClients.key(socket), messageData));
            messageBroadcastQueueLock.unlock();
            qDebug()<<"New message received from: "<<m_connectedClients.key(socket);


        }
    }
}

void QTalkServer::broadcastQueuedMessages()
{
    //here we check if any new message received from clients, if it - broadcast tham to all connected clients

    qDebug()<<"Current queue size is: "<<broadcastMessageQueue.size()<<", queue status: "<<broadcastMessageQueue.isEmpty();

    //here we need to lock access to queue for mt safety
    messageBroadcastQueueLock.lock();
    while(!broadcastMessageQueue.isEmpty()){
        //take first message
        QPair<QString, QString> message = broadcastMessageQueue.dequeue();
        QString messageStr = message.first + ": "+message.second;//forming message(clientId with data)
        for(auto socket: m_connectedClients){
            //now we want to send it to all connected clients so lets make new method
            if(!sendMessageToClientSocket(socket, messageStr)){
                qDebug()<<"Error while broadcasting message: "
                         <<message.second
                         <<", to client: "
                         <<m_connectedClients.key(socket);
            }
        }
    }
    messageBroadcastQueueLock.unlock();

}

//void QTalkServer::conversationLoop(QTcpSocket* clientConn){
//    //here we loop adn read/write messages?
//    //or better will be make it async? with list of clients and slots/signals mechanism

//}
