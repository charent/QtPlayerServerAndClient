#include <QApplication>
#include "VideoPlayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    VideoPlayer player;

    player.show();

    return a.exec();
}
