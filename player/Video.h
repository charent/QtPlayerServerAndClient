#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QDataStream>
#include <QString>

class Video
{
public:
    explicit Video();
    Video(QString id,QString name);
    ~Video();

    friend QDataStream& operator >> (QDataStream &in,Video &video);
    friend QDataStream& operator << (QDataStream &out,Video &video);

    QString id;
    QString name;


};

#endif // VIDEO_H
