#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore>
#include <QtWidgets>
#include <QtGui>
#include <QAudioOutput>
#include "headers/FolderTree.h"
#include <iostream>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include <Qstring>
#include <string>

#include "headers/videogroupbox.h"
#include "headers/customtreewidgetitem.h"
#include <QPropertyAnimation>


const QString _TREE_WIDGET_STYLE_  = "QTreeView::branch:has-siblings:!adjoins-item {"
        "    border-image: none;"
        "    border-image: url(:/images/vline.png) 0;"
        "    border: none;"

        "}"
        "QTreeView::branch:has-siblings:adjoins-item {"
        "    border-image: none;"
        "    border-image: url(:/images/branch-more.png) 0;"
        "    border: none;"
        "}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
        "    border-image: none;"
        "    border-image: url(:/images/branch-end.png) 0;"
        "    border: none;"
        "}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {"
        "    border-image: none;"
        "    border: none;"
        "    image: url(:/images/branch-closed.png);"
        "}"
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings {"
        "    border-image: none;"
        "    border: none;"
        "    image: url(:/images/branch-open.png);"
        "}"

        
        "QTreeView::item:selected {"
        "    background-color: #7a14d4;"
        "}"
        "QTreeView {"
	    "   border:0.5px solid #2e2d31;"
        "   background-color: #1e1d21;"
        "   border-top: none;"
        "   color:white;"
        "}";


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void durationChanged(qint64 duration);
    void positionChanged(qint64 duration);

    void on_actionOpen_triggered();

    void on_slider_duration_Changed(int value);
    void on_horizontalSlider_Duration_valueChanged(int value);

    void on_pushButton_Play_Pause_clicked();

    void on_pushButton_Stop_clicked();

    void on_pushButton_Volume_clicked();

    void on_horizontalSlider_Volume_valueChanged(int value);

    void on_pushButton_Seek_Backward_clicked();

    void on_pushButton_Seek_Forward_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void PlayVideo(QTreeWidgetItem *item);

    void PlayVideo(QString FileName);

    void on_treeWidget_itemCollapsed(QTreeWidgetItem *item);

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

    void on_treeWidget_customContextMenuRequested(const QPoint &pos);

    void openFileWithDefaultApp(const QString &filePath);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_Tree_released();

    void on_nextButton_released();

    void on_prevButton_released();

    void resetViewLog();

private:
    Ui::MainWindow *ui;
    QMediaPlayer *Player;
    QAudioOutput *AudioOutput;
    QVideoWidget *Video = nullptr;
    qint64 mDuration;
    QObject *sliderDurationObject;
    QTreeWidgetItem* currentItem = nullptr;
    bool IS_Pause = true;
    bool IS_Muted = false;
    bool programmaticChangeDuration = false;
    bool Is_TreeCollapased = false;

    FolderTree* foldertree = nullptr;

    void updateDuration(qint64 Duration);
    void buildTree(std::filesystem::path path);
};


QTreeWidgetItem* getNextItem(QTreeWidget* tree, QTreeWidgetItem* currentItem);

QTreeWidgetItem* getPreviousItem(QTreeWidget* tree, QTreeWidgetItem* currentItem);
#endif // MAINWINDOW_H
