#include "ClientServerThread.h"

ClientServerThread::ClientServerThread(QTcpSocket *socket, VideoDAO *vDao)
    :clientSocket(socket),videoDao(vDao)
{
    connect(socket,SIGNAL(readyRead()),this,SLOT(readCommand()));
}
void ClientServerThread::readCommand()
{
    QByteArray readArray;

   if (clientSocket->isReadable())
    {
        readArray = clientSocket->readAll();
    }
    QString cmd = QVariant(readArray).toString();

    if (cmd.startsWith("<ask=list/>"))
    {
        sendVideoLsit();
    }
    else if (cmd.startsWith("<ask=video/>"))
    {
        sendVideoData(cmd.section(">",-1));
    }
    else
    {
        qDebug()<<cmd<<" error!";
    }
}

void ClientServerThread::sendVideoData(QString id)
{
    videoDao->setVideoPlayTimes(id);
    QString path = videoDao->getVideoPathById(id);

    QFile sendFile(path);

    char respond[32] = "<respond=video/>";

    if (!sendFile.open(QFile::ReadOnly))
    {
        qDebug()<<"can't open file!";
        sendFile.close();
        return;
    }

    if (!clientSocket->write(respond,32))
    {
         qDebug()<<"error to write";
    }

    QByteArray sendArray = sendFile.readAll();

    if (!clientSocket->write(sendArray))
    {
        qDebug()<<"error to write";
    }

}

void ClientServerThread::sendVideoLsit()
{
    QByteArray writeArray;
    QDataStream out(&writeArray,QIODevice::WriteOnly);
    videoDao->getVideoList(out);

    char respond[32] = "<respond=list/>";
    if (!clientSocket->write(respond,32))
    {
        std::cout<<"write list head error!";
    }

    if (!clientSocket->write(writeArray))
    {
        std::cout<<"write list data error!";
    }

}

void ClientServerThread::setVideoDAO(VideoDAO *vDao)
{
    this->videoDao = vDao;
}

void ClientServerThread::run()
{


}

ClientServerThread::~ClientServerThread()
{
    clientSocket->close();
    delete videoDao;
    delete clientSocket;


}
