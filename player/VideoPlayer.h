#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QtMultimedia>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QTableView>
#include <QListView>
#include <QTcpSocket>
#include <QMutex>
#include <QByteArray>
#include <QStandardItemModel>
#include <QList>
#include <QFile>
#include <iostream>
#include <QTimer>

#include <QDebug>

#include "NetworkSetting.h"
#include "VideoWidget.h"
#include "Video.h"

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget *parent = 0);
    ~VideoPlayer();
    void myInit();

    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QMediaPlayer *player;
    VideoWidget *videoOut;

    QVBoxLayout *mainLayout;
    QHBoxLayout *windowListLayout;
    QVBoxLayout *controlLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *controlLayout2;

    QPushButton *playButton;
    QPushButton *nextButton;
    QPushButton *prevButton;
    QPushButton *fullScreenButton;
    QPushButton *volumeButton;
    QHBoxLayout *sliderLayout;
    QSlider *playSlider;
    QSlider *volumeSlider;
    QLabel *durationLabel;

    QTimer *timer;

    QTableView *listView;
    QStandardItemModel *qmodel;

    QString trackInfo;
    QString statusInfo;
    qint64 duration;
    QLabel *statu;
    int volume;

    QTcpSocket *clientSocket;
    QByteArray videoBuffer;
    QMutex *mutex;
    QPushButton *connectButton;
    QPushButton *refreshButton;
    NetworkSetting *settingWidget;
    //template <class Video>
    QList<Video> playList;

    QString ip;
    int port = 0;

    bool hasRequested;
    bool isReadList;

    int currentMedialIndex;
    QString cachePath;
    QFile videoCahe;
    bool isConnectToserver;

    //function
    void updateDurationInfo(qint64 currentInfo);
    void setPlayList();
    void askForVideo(QString id);
    void askForList();
    void setListViewDefault();
    void receiveList(QByteArray &readArray);
    void receiveVideo(QByteArray &readArray);
    void changeMediaContent();

protected:
    void mousePressEvent(QMouseEvent *event) ;
    void stateChange();
signals:
    void bufferReceived();
private slots:
    void playButtonClicked();
    void prevButtonClicked();
    void nextButtonClicked();
    void fullScreenButtonClicked();

    void volumeSliderChange(int vol);
    void volumeButtonClicked();
    void refreshButtonClicked();
    void connectButtonCLicked();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);

    void seek(int seconds);
    void displayErrorMessage();
    void socketReadyToRead();
    void socketConnected();
    void setNewMdieaContent();
    void mediaChange();
    void connectToServer();
};

#endif // PLAYER_H
