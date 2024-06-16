#include "mainwindow.h"

#include <QApplication>
#include "qtalkserver.h"
#include "qtalkclient.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTalkServer server;
    QTalkClient client;
    QObject::connect(&server, &QTalkServer::clientStateChanged, &w, &MainWindow::gui_clientStateChanged);
    QObject::connect(&server, &QTalkServer::serverStateChanged, &w, &MainWindow::gui_setServerStatus);
    QObject::connect(&server, &QTalkServer::newMessageReceived, &w, &MainWindow::gui_newMessageReceived);

    QObject::connect(&w, &MainWindow::gui_runServerRequested, &server, &QTalkServer::startServer);
    QObject::connect(&w, &MainWindow::gui_stopServerRequested, &server, &QTalkServer::stopServer);

    QObject::connect(&w, &MainWindow::gui_connectToServerRequested, &client, &QTalkClient::connectToServer);
    QObject::connect(&w, &MainWindow::gui_dissconnectFromServerRequested, &client, &QTalkClient::dissconnectFromServer);
    QObject::connect(&w, &MainWindow::gui_sendMessageRequested, &client, &QTalkClient::sendMessage);
    QObject::connect(&client, &QTalkClient::clientStateChanged, &w, &MainWindow::gui_setClientStatus);
    QObject::connect(&client, &QTalkClient::newMessageReceived, &w, &MainWindow::gui_newMessageReceivedSimple);


    w.show();
    return a.exec();
}
