#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QtMultimediaWidgets/QVideoWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>




class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0);
    ~VideoWidget();
public:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
};



#endif // VIDEOWIDGET_H
