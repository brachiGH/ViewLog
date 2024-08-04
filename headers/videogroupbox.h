#ifndef VIDEOGROUPBOX_H
#define VIDEOGROUPBOX_H

#include <QGroupBox>
#include <QResizeEvent>
#include <qvideowidget.h>

class VideoGroupBox : public QGroupBox {
    Q_OBJECT

public:
    explicit VideoGroupBox(QWidget *parent = nullptr);

    void setVideo(QVideoWidget *video);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QVideoWidget *Video = nullptr;

};

#endif // VIDEOGROUPBOX_H
