#ifndef NETSERVER_H
#define NETSERVER_H

#include <QObject>
#include <QTcpServer>

#include <QDebug>
#include "ClientServerThread.h"
#include "VideoDAO.h"

class NetServer : public QObject
{
    Q_OBJECT
public:
    explicit NetServer(QObject *parent,VideoDAO *vDao);
    ~NetServer();
    void startService(QString hostName, int port);
    void stopService();
signals:

public slots:
    void newConnect();
    void acceptErrorShow();
private:
    QTcpServer *server;
    ClientServerThread *clientThread;
    VideoDAO *videoDao;
};

#endif // NETSERVER_H
