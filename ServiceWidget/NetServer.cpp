#include "NetServer.h"

NetServer::NetServer(QObject *parent, VideoDAO *vDao)
    : QObject(parent),videoDao(vDao)
{
    clientThread = NULL;
    server = new QTcpServer(this);
}

void NetServer::startService(QString hostName, int port)
{
    server->listen(QHostAddress(hostName),port);
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnect()));
    connect(server,SIGNAL(acceptError(QAbstractSocket::SocketError)),this,SLOT(acceptErrorShow()));
    qDebug()<<"now listen"<<hostName<<":"<<port;
}

void NetServer::acceptErrorShow()
{
    qDebug()<<"error "<<server->errorString();
}

void NetServer::stopService()
{
    if(server->isListening())
    {
        server->close();
    }
}

void NetServer::newConnect()
{
    qDebug()<<"new connection";
    clientThread = new ClientServerThread(server->nextPendingConnection(),videoDao);
    clientThread->start();
}

NetServer::~NetServer()

{
    server->close();
    delete clientThread;
    delete server;
    delete videoDao;
}
