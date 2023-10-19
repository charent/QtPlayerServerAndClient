#ifndef CLIENTSERVERTHREAD_H
#define CLIENTSERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QDataStream>
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <iostream>

#include "VideoDAO.h"
class ClientServerThread : public QThread
{
    Q_OBJECT
public:
    ClientServerThread(QTcpSocket *socket,VideoDAO *vDao);
    ~ClientServerThread();
    void setVideoDAO(VideoDAO *vDao);
    void sendVideoData(QString id);
    void sendVideoLsit();

signals:

private:
    QTcpSocket *clientSocket;
    VideoDAO *videoDao;
protected:
    void run();
public slots:
    void readCommand();
};

#endif // CLIENTSERVERTHREAD_H
