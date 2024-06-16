#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* w_central = new QWidget(this);
    setCentralWidget(w_central);
    ly_main = new QVBoxLayout(w_central);
    te_messages = new QTextEdit();
    te_messages->setReadOnly(true);
    ly_main->addWidget(te_messages);

    QHBoxLayout* ly_sendMessage = new QHBoxLayout();
    le_messageField = new QLineEdit();
    pb_sendMessage = new QPushButton("Send");

    ly_sendMessage->addWidget(le_messageField);
    ly_sendMessage->addWidget(pb_sendMessage);


    ly_main->addLayout(ly_sendMessage);


    QHBoxLayout* ly_serverControl = new QHBoxLayout();

    lb_serverStatusLb = new QLabel("Server status: ");
    lb_serverStatus = new QLabel("Offline");
    pb_switchServerState = new QPushButton("Start");

    ly_serverControl->addWidget(lb_serverStatusLb);
    ly_serverControl->addWidget(lb_serverStatus);
    ly_serverControl->addWidget(pb_switchServerState);


    ly_main->addLayout(ly_serverControl);

    QFormLayout* ly_clientControl = new QFormLayout();

    lb_clientStatusLb = new QLabel("Client status: ");
    lb_clientStatus = new QLabel("Offline");
    ly_clientControl->addRow(lb_clientStatusLb, lb_clientStatus);
    le_serverAddress = new QLineEdit();
    pb_connectToServer = new QPushButton("Connect");
    ly_clientControl->addRow(le_serverAddress, pb_connectToServer);

    ly_main->addLayout(ly_clientControl);

    connect(pb_sendMessage, &QPushButton::clicked, this, &MainWindow::sendMessage);

    connect(pb_switchServerState, &QPushButton::clicked, this, [this](){
        if(this->pb_switchServerState->text() == "Start"){
            this->pb_switchServerState->setText("Stop");
            emit this->gui_runServerRequested();
        }else{
            this->pb_switchServerState->setText("Start");
            emit this->gui_stopServerRequested();
        }
    });

    connect(pb_connectToServer, &QPushButton::clicked, this, [this](){
        if(!le_serverAddress->text().isEmpty()){
            if(this->pb_connectToServer->text() == "Connect"){
                this->pb_connectToServer->setText("Dissconnect");
                auto splittedAddress = le_serverAddress->text().split(":");

                emit this->gui_connectToServerRequested(splittedAddress.at(0), splittedAddress.at(1).toInt());
            }else{
                this->pb_connectToServer->setText("Connect");
                emit this->gui_dissconnectFromServerRequested();
            }
        }
    });


}

MainWindow::~MainWindow()
{
}

void MainWindow::gui_setServerStatus(QString status)
{
    lb_serverStatus->setText(status);
    te_messages->insertHtml("<span style='color:blue;'>" + QString("Server " + status) + "</span><br>");

}


void MainWindow::gui_setClientStatus(QString status)
{
    lb_clientStatus->setText(status);
    te_messages->insertHtml("<span style='color:blue;'>" + QString("Client " + status) + "</span><br>");

}

void MainWindow::gui_newMessageReceived(QString clientId, QString messageData)
{
    //here we received new message from our clients so print it
    te_messages->insertHtml("<span style='color:gray;'>" + QString(clientId + ": ") + messageData +"</span><br>");

}

void MainWindow::gui_newMessageReceivedSimple(QString messageData)
{
    //here we received new message from our clients so print it
    te_messages->insertHtml("<span style='color:gray;'>"  + messageData +"</span><br>");

}
void MainWindow::gui_clientStateChanged(QString clientId, int state)
{
    if(state==1){
        te_messages->insertHtml("<span style='color:green;'>" + QString(clientId + ": connected") + "</span><br>");

    }else{
        te_messages->insertHtml("<span style='color:red;'>" + QString(clientId + ": dissconnected") + "</span><br>");
    }


}


void MainWindow::sendMessage()
{
    //here we do all magic
    QString message = le_messageField->text();

    emit gui_sendMessageRequested(message);

}

