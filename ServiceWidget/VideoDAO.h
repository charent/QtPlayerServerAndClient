#ifndef VIDEODAO_H
#define VIDEODAO_H

#include <QSqlQueryModel>
#include <QtSql>
#include <QList>
#include <QDataStream>
#include "Video.h"

class VideoDAO :public QObject
{
    Q_OBJECT
public:
    VideoDAO();
    ~VideoDAO();

    void insertVideo(QString fileName);
    void deleteVideo(int id);
    void getVideoList(QDataStream &out);
    QString getVideoPathById(QString id);
    void setVideoPlayTimes(QString id);
    bool createConnection(QString hostName,int port,QString userName,
                                 QString password,QString databaseName);
private:
    QSqlQuery *sqlQuery;

    qint64 fileSize;
    QString sizeString;
    QString sqlString;
    QString name;
    QString path;
};

#endif // VIDEODAO_H
