#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QWidget *parent)
    :QWidget(parent),videoOut(0)
{
    cachePath = "";
    isConnectToserver = false;
    myInit();
    setWindowTitle("网络播放器");
    setMinimumSize(700,400);
}

void VideoPlayer::myInit()
{
    hasRequested = false;
    isReadList = false;
    videoOut = new VideoWidget(this);
    player = new QMediaPlayer(this);

    player->setVolume(50);

    videoOut->setMinimumSize(500,300);
    player->setVideoOutput(videoOut);

    if (!player->isAvailable())
    {
        qDebug()<<"fail to  start a service!";
        qDebug()<<player->errorString();
    }

    mainLayout = new QVBoxLayout;
    windowListLayout = new QHBoxLayout;

    listView = new QTableView (this);
    qmodel = new QStandardItemModel(this);
    listView->setModel(qmodel);
    qmodel->setHorizontalHeaderItem(0,new QStandardItem("id"));
    qmodel->setHorizontalHeaderItem(1,new QStandardItem("视频名称"));
    listView->setColumnHidden(0,true);
    listView->setColumnWidth(1,190);
    listView->adjustSize();
    listView->setItemDelegateForColumn(1,NULL);
    windowListLayout->addWidget(videoOut);
    windowListLayout->addWidget(listView);
    windowListLayout->setStretchFactor(videoOut,2);
    windowListLayout->setStretchFactor(listView,1);


    controlLayout = new QVBoxLayout;
    controlLayout2 = new QHBoxLayout;

    sliderLayout = new QHBoxLayout;
    playSlider = new QSlider(Qt::Horizontal);
    playSlider->setRange(0, player->duration() / 1000);

    sliderLayout->addWidget(playSlider);
    controlLayout->addLayout(sliderLayout);


    buttonLayout = new QHBoxLayout;
    fullScreenButton = new QPushButton("全屏");

    QIcon icon1(":/icon/play.png");
    playButton = new QPushButton("");
    playButton->setIconSize(QSize(80,30));
    playButton->setIcon(icon1);

    QIcon icon2(":/icon/prev.png");
    prevButton = new QPushButton();
    prevButton->setIconSize(QSize(80,30));
    prevButton->setIcon(icon2);

    QIcon icon3(":/icon/next.png");
    nextButton = new QPushButton("");
    nextButton->setIconSize(QSize(80,30));
    nextButton->setIcon(icon3);

    volumeSlider = new QSlider(Qt::Horizontal);

    QIcon icon4(":/icon/vol.png");
    volumeButton = new QPushButton();
    volumeButton->setIcon(icon4);
    durationLabel = new QLabel;

    volumeSlider->setRange(0,100);
    volumeSlider->setValue(50);
    refreshButton = new QPushButton;
    QIcon icon5(":/icon/refresh.png");
    refreshButton->setIconSize(QSize(15,15));
    refreshButton->setIcon(icon5);
    connectButton = new QPushButton();
    QIcon icon6(":/icon/settings.png");
    connectButton->setIconSize(QSize(15,15));
    connectButton->setIcon(icon6);
    controlLayout2->addWidget(durationLabel);
    controlLayout2->addStretch(16);
    controlLayout2->addWidget(connectButton,1);
    controlLayout2->addWidget(refreshButton,1);
    controlLayout->addLayout(controlLayout2);

    buttonLayout->addStretch(4);
    buttonLayout->addWidget(prevButton);
    buttonLayout->addWidget(playButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addStretch(4);
    buttonLayout->addWidget(fullScreenButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(volumeButton);
    buttonLayout->addWidget(volumeSlider);

    controlLayout->addLayout(buttonLayout);

    mainLayout->addLayout(windowListLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->setStretchFactor(windowListLayout,3);
    mainLayout->setStretchFactor(controlLayout,1);
    statu = new QLabel;
    statu->setText("请连接服务器");
    mainLayout->addWidget(statu);
    setLayout(mainLayout);

    settingWidget = new NetworkSetting(ip,port);
    connect(settingWidget,SIGNAL(inputFinush()),this,SLOT(connectToServer()));

    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

    connect(listView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(mediaChange()));

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));

    connect(playButton,SIGNAL(clicked()),this,SLOT(playButtonClicked()));
    connect(fullScreenButton,SIGNAL(clicked()),this,SLOT(fullScreenButtonClicked()));
    connect(nextButton,SIGNAL(clicked(bool)),this,SLOT(nextButtonClicked()));
    connect(prevButton,SIGNAL(clicked(bool)),this,SLOT(prevButtonClicked()));


    //connect(playSlider, SIGNAL(actionTriggered(int)), this, SLOT(seek(int)));
    connect(playSlider,SIGNAL(sliderMoved(int)),this,SLOT(seek(int)));
    connect(volumeButton,SIGNAL(clicked()),this,SLOT(volumeButtonClicked()));
    connect(volumeSlider,SIGNAL(sliderMoved(int)),this,SLOT(volumeSliderChange(int)));
    connect(volumeSlider,SIGNAL(valueChanged(int)),this,SLOT(volumeSliderChange(int)));
    connect(connectButton,SIGNAL(clicked(bool)),this,SLOT(connectButtonCLicked()));
    connect(refreshButton,SIGNAL(clicked(bool)),this,SLOT(refreshButtonClicked()));

    setListViewDefault();
    clientSocket = new QTcpSocket;

    connect(clientSocket,SIGNAL(connected()),this,SLOT(socketConnected()));
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(socketReadyToRead()));
    //connect(this,SIGNAL(bufferReceived()),this,SLOT(setNewMdieaContent()));

    timer = new QTimer;

    connect(timer,SIGNAL(timeout()),this,SLOT(setNewMdieaContent()));

}

void VideoPlayer::refreshButtonClicked()
{
    askForList();
}

void VideoPlayer::connectButtonCLicked()
{
    settingWidget->show();
}

void VideoPlayer::connectToServer()
{
    if (!isConnectToserver)
    {
        clientSocket->connectToHost(QHostAddress(ip),port);
        isConnectToserver = true;
    }
    else
    {
        clientSocket->close();
        clientSocket->connectToHost(QHostAddress(ip),port);
        isConnectToserver = true;
    }
    statu->setText("服务器已连接");
}

void VideoPlayer::setNewMdieaContent()
{
    timer->stop();
    player->stop();
    player->setMedia(QUrl::fromLocalFile(cachePath));
    QIcon icon2(":/icon/pause.png");
    playButton->setIcon(icon2);
    player->play();
    statu->setText("");
}

void VideoPlayer::mediaChange()
{
    QModelIndex index = qmodel->index(listView->currentIndex().row(),0);
    currentMedialIndex = listView->currentIndex().row();
    QVariant id = qmodel->data(index);

    if (cachePath != "")
    {
       videoCahe.close();
    }

    cachePath = "/tmp/videoCache0000"+id.toString()+".cache";
    videoCahe.setFileName(cachePath);
    if (!videoCahe.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open dir error";
    }

    askForVideo(id.toString());
    statu->setText("请稍等，正在缓冲......");
    timer->start(1500);
 }

void VideoPlayer::changeMediaContent()
{

    setNewMdieaContent();
}

void VideoPlayer::askForVideo(QString id)
{
    QByteArray writeArray;
    writeArray.append("<ask=video/>"+id);

    clientSocket->write(writeArray);

    hasRequested = true;
}

void VideoPlayer::stateChange()
{
    QIcon icon1(":/icon/pause.png");
    QIcon icon2(":/icon/play.png");
    switch (player->state())
    {
        case QMediaPlayer::StoppedState:
        case QMediaPlayer::PausedState:
            emit player->play();
            playButton->setIcon(icon1);
            break;
        case QMediaPlayer::PlayingState:
            emit player->pause();
            playButton->setIcon(icon2);
            break;
    }
}

void VideoPlayer::volumeButtonClicked()
{
    if (volumeSlider->value() != 0)
    {
        volume = volumeSlider->value();
        volumeSlider->setValue(0);
        QIcon icon1(":/icon/nosound.png");
        volumeButton->setIcon(icon1);
        player->setVolume(0);
    }
    else
    {
        volumeSlider->setValue(volume);
        QIcon icon2(":/icon/vol.png");
        volumeButton->setIcon(icon2);
        player->setVolume(volume);
    }
}

void VideoPlayer::volumeSliderChange(int vol)
{
    player->setVolume(vol);
}

void VideoPlayer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || player == NULL)
    {
        return;
    }

    stateChange();
}

void VideoPlayer::playButtonClicked()
{
    stateChange();
}

void VideoPlayer::prevButtonClicked()
{
    if (currentMedialIndex == 0)
    {
        return;
    }
    currentMedialIndex -= 1;
    QModelIndex index = qmodel->index(currentMedialIndex,0);
    listView->setCurrentIndex(qmodel->index(currentMedialIndex,1));
    QVariant id = qmodel->data(index);

    cachePath = "/tmp/videoCache0000"+id.toString()+".cache";
    videoCahe.setFileName(cachePath);
    if (!videoCahe.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open dir error";
    }

    askForVideo(id.toString());
    statu->setText("请稍等，正在缓冲......");
    timer->start(1500);


    changeMediaContent();
}

void VideoPlayer::nextButtonClicked()
{
    if (qmodel->rowCount() == currentMedialIndex + 1)
    {
        return;
    }
    currentMedialIndex += 1;
    listView->setCurrentIndex(qmodel->index(currentMedialIndex,1));
    QModelIndex index = qmodel->index(currentMedialIndex,0);

    QVariant id = qmodel->data(index);

    cachePath = "/tmp/videoCache0000"+id.toString()+".cache";
    videoCahe.setFileName(cachePath);
    if (!videoCahe.open(QIODevice::ReadWrite))
    {
        qDebug()<<"open dir error";
    }

    askForVideo(id.toString());
    statu->setText("请稍等，正在缓冲......");
    timer->start(1500);

    changeMediaContent();
}

void VideoPlayer::socketConnected()
{
    askForList();
}

void VideoPlayer::askForList()
{
    QByteArray writeArray;
    writeArray.append("<ask=list/>");

   if (!clientSocket->write(writeArray))
   {
       qDebug()<<"write error!";
   }

   hasRequested = true;
}

void VideoPlayer::socketReadyToRead()
{
    QByteArray readArray;
    qint64 resSize = 32;
    char respond[resSize] = {'\0'};

    if (hasRequested)
    {
        clientSocket->read(respond,resSize);
        hasRequested = false;
    }

    QString cmd = QString(respond);
    if (cmd == "<respond=list/>")
    {
        isReadList = true;
    }
    else if(cmd == "<respond=video/>")
    {
        isReadList = false;
    }


    if (isReadList)
    {
        receiveList(readArray);
    }
    else
    {
        receiveVideo(readArray);
    }
}

void VideoPlayer::receiveList(QByteArray &readArray)
{
    readArray.clear();

    QDataStream in(&readArray,QIODevice::ReadWrite);
    readArray = clientSocket->readAll();

    playList.clear();

    while (!in.atEnd())
    {
        Video v;
        in>>v;
        playList.push_back(v);
    }
    setPlayList();
    isReadList = false;
}

void VideoPlayer::receiveVideo(QByteArray &readArray)
{
    readArray.clear();
    readArray = clientSocket->readAll();
    videoCahe.write(readArray);
}

void VideoPlayer::setPlayList()
{
    int i;

    qmodel->clear();

    for (i = 0; i < playList.size(); i++)
    {
        qmodel->setItem(i,0,new QStandardItem(playList.at(i).id));
        qmodel->setItem(i,1,new QStandardItem(playList.at(i).name));
    }

    listView->setCurrentIndex(qmodel->index(0,1));
    setListViewDefault();
}

void VideoPlayer::setListViewDefault()
{
    qmodel->setHorizontalHeaderItem(0,new QStandardItem("id"));
    qmodel->setHorizontalHeaderItem(1,new QStandardItem("视频"));
    listView->setColumnHidden(0,true);
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void VideoPlayer::fullScreenButtonClicked()
{
    videoOut->setFullScreen(!(videoOut->isFullScreen()));
}

void VideoPlayer::durationChanged(qint64 duration)
{
   this->duration = duration/1000;
    playSlider->setMaximum(duration / 1000);
}

void VideoPlayer::positionChanged(qint64 progress)
{
    if (!playSlider->isSliderDown()) {
        playSlider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void VideoPlayer::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void VideoPlayer::displayErrorMessage()
{
    qDebug()<<player->errorString();

}

void VideoPlayer::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration)
    {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    durationLabel->setText(tStr);
}

void VideoPlayer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen())
    {
        videoOut->setFullScreen(false);
        videoOut->adjustSize();
    }
    else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt)
    {
        videoOut->setFullScreen(!videoOut->isFullScreen());
        event->accept();
    }
}

void VideoPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    videoOut->setFullScreen(!videoOut->isFullScreen());
    event->accept();
}


VideoPlayer::~VideoPlayer()
{
    delete timer;
    delete player;
    delete videoOut;
    delete refreshButton;
    delete settingWidget;

    delete playButton;
    delete nextButton;
    delete prevButton;
    delete fullScreenButton;
    delete volumeButton;

    delete playSlider;
    delete volumeSlider;
    delete durationLabel;
    delete listView;
    delete qmodel;

    clientSocket->close();
    delete clientSocket;
    delete connectButton;

    delete buttonLayout;
    delete sliderLayout;
    delete windowListLayout;
    delete controlLayout2;
    delete controlLayout;
    delete mainLayout;
}
