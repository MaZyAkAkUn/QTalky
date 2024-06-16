#include "qtalkclient.h"

QTalkClient::QTalkClient(QObject *parent)
    : QObject{parent}
{

}

void QTalkClient::connectToServer(QString serverAddress, quint16 port)
{
    emit clientStateChanged("Connecting to: " + serverAddress +":"+ QString::number(port));

    clientSocket = new QTcpSocket(this);

    connect(clientSocket, &QTcpSocket::connected, this, [this](){
        emit this->clientStateChanged(
            "Connected to: " + clientSocket->peerAddress().toString() +":"+ QString::number(clientSocket->peerPort()));

        messageCheckTimer = new QTimer(this);
        messageCheckTimer->setInterval(1000);
        connect(messageCheckTimer, &QTimer::timeout, this, &QTalkClient::checkAvailableMessages);
        messageCheckTimer->start();


    });
    connect(clientSocket, &QTcpSocket::disconnected, this, [this](){
        emit this->clientStateChanged("Dissconnected");

    });

    QHostAddress *addr = new QHostAddress(serverAddress);
    clientSocket->connectToHost(addr->toString(), port);

}

void QTalkClient::dissconnectFromServer()
{
    this->clientSocket->close();

}

void QTalkClient::sendMessage(QString message)
{
    //lets try to write message into socket and read from server
    // at first lets make socket alive till client object is
    if(clientSocket->state() == QAbstractSocket::ConnectedState){
        qint64 bytesWritten = clientSocket->write(message.toStdString().c_str());
        clientSocket->flush();
        qDebug()<<"Sending message: "<<message<<", "<<bytesWritten<<" bytes written;";
    }


}

void QTalkClient::checkAvailableMessages()
{
    if(clientSocket->bytesAvailable() > 0){
        auto data = clientSocket->readAll();
        QString messageData = QString::fromStdString(data.toStdString());

        emit newMessageReceived(messageData);
    }
}
