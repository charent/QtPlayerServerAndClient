#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTableView>
#include <QVBoxLayout>
#include <QSqlQueryModel>
#include <QDebug>
#include <QPoint>
#include <QEvent>
#include <QEvent>
#include <QSqlQueryModel>
#include <QtSql>
#include <QtWidgets>
#include <QTcpServer>
#include <QIcon>
#include <iostream>

#include "VideoDAO.h"
#include "NetServer.h"
class ServerWindow : public QWidget
{
    Q_OBJECT
public:

    QPushButton* start_btn;
    QPushButton* add_btn;
    QPushButton* del_btn;
    QPushButton* search_btn;
    QPushButton *refreshButton;

    QLabel* ip_lab;
    QLabel* port_lab;
    QLabel* keyWd_lab;

    QLineEdit* ip_edit;
    QLineEdit* port_edit;
    QLineEdit* keyWd_edit;

    QTableView* view;
    QSqlTableModel* qmodel;

    QHBoxLayout* startService;
    QHBoxLayout* viewShow;
    QHBoxLayout* options;
    QVBoxLayout* all;

    QString start_btn_icon_path;
    QString add_btn_icon_path;
    QString del_btn_icon_path;
    QString search_btn_icon_path;
    VideoDAO *videoDao;

    NetServer *server;

    bool isChanged;// the port or ip has been changed

public:
    explicit ServerWindow(QWidget *parent=0);
    ~ServerWindow();
    void setStart();
    void setTableView();
    void setOptions();
    void setConnection();

public slots:

    void start_btn_down();
    void ip_edit_change();
    void port_edit_change();
    void add_btn_down();
    void del_btn_down();
    void search_btn_down();
    void refreshButtonClicked();

signals:

};

#endif // SERVERWINDOW_H
