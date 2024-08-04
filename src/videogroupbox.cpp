#include "headers/videogroupbox.h"
#include <QDebug>

VideoGroupBox::VideoGroupBox(QWidget *parent)
    : QGroupBox(parent) {
}

void VideoGroupBox::resizeEvent(QResizeEvent *event) {
    QGroupBox::resizeEvent(event);
    // Handle the resize event here
    QSize newSize = event->size();
    qDebug() << "Video GroupBox resized to:" << newSize;
    if (Video) {
        Video->setGeometry(0, 0, this->width(), this->height());
    }
}

void VideoGroupBox::setVideo(QVideoWidget *v) {
    Video = v;
}
