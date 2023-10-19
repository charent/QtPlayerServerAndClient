#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QDataStream>
#include <QString>

class Video : public QObject
{
    Q_OBJECT
public:
    explicit Video(QObject *parent = 0);
    Video(QString id,QString name);
    ~Video();

    friend QDataStream& operator >> (QDataStream &in,Video &video);
    friend QDataStream& operator << (QDataStream &out,Video &video);

    QString id;
    QString name;
};

#endif // VIDEO_H
