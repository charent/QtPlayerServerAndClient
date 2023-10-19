#include "ServerWindow.h"

ServerWindow::ServerWindow(QWidget *parent):QWidget(parent)
{
//    start_btn_icon_path = ":/btn_pic/icon.png";
//    add_btn_icon_path = ":/btn_pic/icon.png";
//    del_btn_icon_path = ":/btn_pic/icon.png";
//    search_btn_icon_path = ":/btn_pic/icon.png";


    videoDao = new VideoDAO();
    server = new NetServer(this,videoDao);

    setStart();
    setTableView();
    setOptions();
    setConnection();
    all = new QVBoxLayout(this);
    all->addLayout(startService);
    all->addLayout(viewShow);
    all->addLayout(options);
    setLayout(all);

    setMinimumSize(600,400);

    ip_edit->setText("127.0.0.1");
    port_edit->setText("6666");

    setWindowTitle("视频与网络服务管理");
}

void ServerWindow::setStart()
{
    startService = new QHBoxLayout();
    ip_lab = new QLabel("主机IP:");
    ip_edit = new QLineEdit();
    startService->addWidget(ip_lab);
    startService->addWidget(ip_edit);
    //startService->addStretch(1);

    port_lab = new QLabel("主机端口：");
    port_edit = new QLineEdit();
    startService->addWidget(port_lab);
    startService->addWidget(port_edit);

    start_btn = new QPushButton("启动服务");
    start_btn->setDisabled(true);

    startService->addWidget(start_btn);

}

void ServerWindow::setTableView()
{
    qmodel = new  QSqlTableModel(this);
    view = new QTableView();
    view->setModel(qmodel);
    qmodel->setTable("videoInfo");
    qmodel->select();
    qmodel->setHeaderData(0, Qt::Horizontal, tr("id"));
    qmodel->setHeaderData(1, Qt::Horizontal, tr("视频名称"));
    qmodel->setHeaderData(2, Qt::Horizontal, tr("视频路径"));
    qmodel->setHeaderData(3,Qt::Horizontal,tr("文件大小"));
    qmodel->setHeaderData(4,Qt::Horizontal,tr("播放次数"));

    //设置选中时为整行选中
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置表格的单元为只读属性，即不能编辑
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //如果你用在QTableView中使用右键菜单，需启用该属性
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    view->setColumnHidden(0,true);
    view->adjustSize();
    view->show();

    viewShow = new QHBoxLayout();
    viewShow->addWidget(view);
}

void ServerWindow::setOptions()
{

    refreshButton = new QPushButton("刷新");
    add_btn = new QPushButton("添加视频");
    del_btn = new QPushButton("删除视频");
    search_btn = new QPushButton("搜索");
    keyWd_lab = new QLabel("关键字：");
    keyWd_edit = new QLineEdit();

    options = new QHBoxLayout();
    options->addStretch(1);
    options->addWidget(add_btn);
    options->addWidget(del_btn);
    options->addWidget(refreshButton);
    options->addStretch(1);
    options->addWidget(keyWd_lab);

    options->addWidget(keyWd_edit);
    options->addWidget(search_btn);
    options->addStretch(1);
}

void ServerWindow::setConnection()
{
    connect(start_btn, SIGNAL(clicked()), this, SLOT(start_btn_down()));
    connect(add_btn,SIGNAL(clicked()), this, SLOT(add_btn_down()));
    connect(del_btn,SIGNAL(clicked()), this, SLOT(del_btn_down()));
    connect(search_btn,SIGNAL(clicked()), this, SLOT(search_btn_down()));

    connect(ip_edit,SIGNAL(textChanged(QString)),this,SLOT(ip_edit_change()));
    connect(port_edit,SIGNAL(textChanged(QString)),this,SLOT(port_edit_change()));
    connect(refreshButton,SIGNAL(clicked(bool)),this,SLOT(refreshButtonClicked()));
}

void ServerWindow::refreshButtonClicked()
{
    qmodel->select();
}

void ServerWindow::start_btn_down()//push down
{
    QString ip = ip_edit->text();
    QString port = port_edit->text();

    if (start_btn->text() == "启动服务")
    {
        start_btn->setText("停止服务");
        ip_edit->setDisabled(true);
        port_edit->setDisabled(true);

       server->startService(ip,port.toInt());
    }
    else
    {
        start_btn->setText("启动服务");
        ip_edit->setDisabled(false);
        port_edit->setDisabled(false);
        server->stopService();
    }   
}

void ServerWindow::ip_edit_change()
{
    if (port_edit->text() == "")
    {
       return;
    }
    else
    {
        start_btn->setDisabled(false);
    }

 }

void ServerWindow::port_edit_change()
{
    if (ip_edit->text() == "")
    {
       return;
    }
    else
    {
        start_btn->setDisabled(false);
    }
}

void ServerWindow::add_btn_down()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), " ",
                                                    tr("全部文件(*.*)"));
    if (fileName == "")
    {
        return;
    }

    videoDao->insertVideo(fileName);

    qmodel->select();

}

void ServerWindow::del_btn_down()
{
    QModelIndex index = qmodel->index(view->currentIndex().row(),0);
    QVariant id = qmodel->data(index);

    videoDao->deleteVideo(id.toInt());
    qmodel->select();

}

void ServerWindow::search_btn_down()
{

}

ServerWindow::~ServerWindow()
{
    delete start_btn;
    delete add_btn;
    delete del_btn;
    delete search_btn;
    delete refreshButton;

    delete ip_lab;
    delete port_lab;
    delete keyWd_lab;

    delete ip_edit;
    delete port_edit;
    delete keyWd_edit;

    delete view;
    delete qmodel;

    delete startService;
    delete viewShow;
    delete options;
    delete all;
    delete server;
}
