#include "VideoDAO.h"

VideoDAO::VideoDAO()
{
    if (!createConnection("localhost",3306,"root","mysql123","videos"))
    {
        qDebug()<<"conneect databse error";
    }
}

void VideoDAO::insertVideo(QString fileName)
{
    name  = fileName.section("/",-1);
    name.truncate(name.lastIndexOf("."));
    path = fileName;

    QFile insertFile(fileName);

    if (!insertFile.open(QFile::ReadOnly))
    {
        qDebug()<<"open error";
        return;
    }

    sizeString = QString::number(insertFile.size());

    sqlString = "insert into videos.videoInfo(video_name,path,file_size,play_times) values('"+
                name+"','"+path+"','"+sizeString+"',0)";

    if (!sqlQuery->exec(sqlString))
    {
        qDebug()<<sqlQuery->lastError().text();
        qDebug()<<sqlString;
    }

    sqlQuery->finish();

    insertFile.close();
}

void VideoDAO::deleteVideo(int id)
{

    QString sqlString = "delete from videos.videoInfo where id = " +QString::number(id);
    if (!sqlQuery->exec(sqlString))
        {
            qDebug()<<sqlQuery->lastError().text();
            qDebug()<<sqlString;
        }
     sqlQuery->finish();
}

void VideoDAO::getVideoList(QDataStream &out)
{
    QString sqlString = "select id,video_name from videos.videoInfo";

    if (!sqlQuery->exec(sqlString))
    {
        qDebug()<<sqlQuery->lastError().text();
        qDebug()<<sqlString;
    }

    int i = 0;
    while (sqlQuery->next())
    {
        Video v(sqlQuery->value(0).toString(),sqlQuery->value(1).toString());
        out<<v;
        i++;
    }
}

QString VideoDAO::getVideoPathById(QString id)
{
    QString sqlString = "select path from videos.videoInfo where id ='" + id + "'";
    QString res;

    if (!sqlQuery->exec(sqlString))
    {
        qDebug()<<sqlQuery->lastError().text();
        qDebug()<<sqlString;
    }
    if (sqlQuery->next())
    {
        res = sqlQuery->value(0).toString();
    }

    return  res;
}

void VideoDAO::setVideoPlayTimes(QString id)
{
    QString sqlString = "update videos.videoInfo set play_times = play_times+1 where id ='" + id + "'";

    if (!sqlQuery->exec(sqlString))
    {
        qDebug()<<sqlQuery->lastError().text();
        qDebug()<<sqlString;
    }
}

VideoDAO::~VideoDAO()
{
    delete sqlQuery;
    sqlQuery->~QSqlQuery();

}

bool VideoDAO::createConnection(QString hostName,int port,QString userName,QString password,
                                QString databaseName)
{
    QSqlDatabase db;

    if (QSqlDatabase::contains())
    {
       db = QSqlDatabase::database("new_con");
       qDebug()<<"contains";
    }
    else
    {
        db = QSqlDatabase::addDatabase("QMYSQL");
    }

    db.setHostName(hostName);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(password);
    db.setDatabaseName(databaseName);
    //db.setConnectOptions("CLIENT_SSL=1;CLIENT_IGNORE_SPACE=1");

    if (!db.open()) {
        qDebug()<<"open database error! "<<db.lastError().text();
        return false;
    }

    sqlQuery = new QSqlQuery(db);


    return true;
}
