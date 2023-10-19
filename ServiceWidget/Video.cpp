#include "Video.h"

Video::Video(QObject *parent) : QObject(parent)
{

}

Video::Video(QString id, QString name)
{
    this->id = id;
    this->name = name;
}

QDataStream& operator >> (QDataStream &in,Video &video)
{
    in>>video.id>>video.name;
    return in;
}

QDataStream& operator << (QDataStream &out,Video &video)
{
    out<<video.id<<video.name;
    return out;
}

Video::~Video()
{

}
