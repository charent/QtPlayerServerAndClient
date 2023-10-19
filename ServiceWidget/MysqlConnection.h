#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

static bool createConnection(QString hostName,int port,QString userName,QString password,QString databaseName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
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

    return true;
}

#endif // MYSQLCONNECTION_H
