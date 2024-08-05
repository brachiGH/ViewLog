#include "headers/mainwindow.h"
#include "ui_mainwindow.h"



void highlightItem(QTreeWidget* tree, QTreeWidgetItem* item) {
    if (!item) return;

    tree->clearSelection();
    item->setSelected(true);
    tree->scrollToItem(item);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Player = new QMediaPlayer();
    AudioOutput = new QAudioOutput();
    Player->setAudioOutput(AudioOutput);

    ui->pushButton_Play_Pause->setIcon(QIcon(":/assets/controls/play_icon.png"));
    ui->pushButton_Stop->setIcon(QIcon(":/assets/controls/box_icon.png"));
    ui->pushButton_Seek_Backward->setIcon(QIcon(":/assets/controls/play_back_icon.png"));
    ui->pushButton_Seek_Forward->setIcon(QIcon(":/assets/controls/play_forward_icon.png"));
    ui->pushButton_Volume->setIcon(QIcon(":/assets/controls/volume_high_icon.png"));
    ui->nextButton->setIcon(QIcon(":/assets/controls/play_skip_forward_icon.png"));
    ui->prevButton->setIcon(QIcon(":/assets/controls/play_skip_back_icon.png"));

    ui->pushButton_Tree->setIcon(QIcon(":/assets/treeWidget/collapse-Tree.png"));

    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);

    ui->horizontalSlider_Volume->setValue(30);
    AudioOutput->setVolume(0.3);


    ui->horizontalSlider_Volume->setStyleSheet(
            "QSlider::groove:horizontal {"
            "    height: 5px;"
            "    background: #724c8a;"
            "    border-radius: 2px;"
            "}"
            "QSlider::sub-page:horizontal {"
            "    background: #a17bb9;"
            "    border-radius: 2px;"
            "}"
            "QSlider::handle:horizontal {"
            "    backkground: transparent;"
            "    border-radius: 2px;"
            "}"
        );


    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(ui->actionReset_ViewLog, &QAction::triggered, this, &MainWindow::resetViewLog);

    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);
    ui->horizontalSlider_Duration->setStyleSheet(R"(
            QSlider {
            width: 9000px;
            }

            QSlider::groove:horizontal {
                border: none;
                height: 5px;
                background: #834c8a;
                border-radius: 2px;
            }

            QSlider::sub-page:horizontal {
                background: #ba6cb7;
                border: none;
                height: 5px;
                border-radius: 2px;
            }

            QSlider::add-page:horizontal {
                background: #834c8a;
                border: none;
                height: 5px;
                border-radius: 2px;
            }

            QSlider::handle:horizontal {
                backkground: transparent;
                border-radius: 2px;
            }
        )");

    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->treeWidget->setColumnCount(1);

    qInfo() << "Creating the treewidget";
    QString rootPath = getLastTree();
    if (rootPath != "") {
        buildTree(rootPath.toStdU16String());
    }


    ui->treeWidget->setStyleSheet(_TREE_WIDGET_STYLE_);
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);

    qInfo() << "Finished building UI";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buildTree(std::filesystem::path path) {
    foldertree = new FolderTree(ui->treeWidget, path);
    foldertree->uiBuildTree();
    currentItem = ui->treeWidget->topLevelItem(0);
}

void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    ui->horizontalSlider_Duration->setMaximum(mDuration);
}


void MainWindow::positionChanged(qint64 duration)
{
    if (!ui->horizontalSlider_Duration->isSliderDown())
    {
        programmaticChangeDuration = true;
        ui->horizontalSlider_Duration->setValue(duration / 1000);
        programmaticChangeDuration = false;
    }
    updateDuration(duration / 1000);
}


void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
{
    if (!programmaticChangeDuration) {
        Player->setPosition(value * 1000);
    }
}

void MainWindow::updateDuration(qint64 Duration)
{
    if (Duration || mDuration)
    {
        QTime CurrentTime((Duration / 3600) % 60, (Duration / 60) % 60, Duration % 60, (Duration * 1000) % 1000);
        QTime TotalTime((mDuration / 3600) % 60, (mDuration / 60) % 60, mDuration % 60, (mDuration * 1000) % 1000);
        QString Format ="";
        if (mDuration > 3600) Format = "hh:mm:ss";
        else Format = "mm:ss";

        ui->label_current_Time->setText(CurrentTime.toString(Format));
        ui->label_Total_Time->setText(TotalTime.toString(Format));
    }
}

void MainWindow::resetViewLog() {
    foldertree->Reset();
}

void MainWindow::on_actionOpen_triggered()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"));

    qInfo() << "Loading videos from " << folderPath.toStdString();

    if (folderPath != "") {
        std::filesystem::path fsPath = folderPath.toStdU16String();  // Convert QString to std::u16string

        ui->treeWidget->clear();
        if (foldertree != nullptr) {
            delete foldertree;
        }
        ui->treeWidget->clear();
        buildTree(fsPath);

        ui->treeWidget->sortItems(0, Qt::AscendingOrder);
    }
}


void MainWindow::on_slider_duration_Changed(int value)
{
    if (!programmaticChangeDuration) {
        Player->setPosition(value * 1000);
    }
}


void MainWindow::on_pushButton_Play_Pause_clicked()
{
    if (IS_Pause == true)
    {
        IS_Pause = false;
        Player->play();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/assets/controls/pause_icon.png"));
    }
    else
    {
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(QIcon(":/assets/controls/play_icon.png"));
    }
}


void MainWindow::on_pushButton_Stop_clicked()
{
    Player->stop();
}


void MainWindow::on_pushButton_Volume_clicked()
{
    if (IS_Muted == false)
    {
        IS_Muted = true;
        ui->pushButton_Volume->setIcon(QIcon(":/assets/controls/volume_mute_icon.png"));
        AudioOutput->setMuted(true);
    }
    else
    {
        IS_Muted = false;
        ui->pushButton_Volume->setIcon(QIcon(":/assets/controls/volume_high_icon.png"));
        AudioOutput->setMuted(false);
    }
}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    qInfo() << "Volume is set to " << value << "%";
    AudioOutput->setVolume((double)value / 100);
}


void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 5);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}


void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 5);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    json* itemj = (json*)ptr;
    if (!(*itemj)["isfile"]) {
        (*itemj)["Expanded"] = !(*itemj)["Expanded"];
        item->setExpanded((bool)((*itemj)["Expanded"]));
    }

    foldertree->saveTreeWidgetToJson();
}


void MainWindow::PlayVideo(QTreeWidgetItem *item)
{
    currentItem = item;

    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    json* itemj = (json*)ptr;

    std::string path = (*itemj)["path"];
    QString Qpath = QString::fromStdString(path);

    PlayVideo(Qpath);

    (*itemj)["watched"] = true;
    setNodeBackgroundColor(item, true);
    highlightItem(ui->treeWidget, item);
    foldertree->saveTreeWidgetToJson();
}

void MainWindow::PlayVideo(QString FileName)
{

    if (Video != nullptr) {
        qInfo() << "Removing old video " << FileName;
        delete Video;
    }

    qInfo() << "playing " << FileName;
    setWindowTitle("ViewLog - " + FileName);

    Video = new QVideoWidget();
    ui->groupBox_Video->setVideo(Video);
    Video->setGeometry(0, 0, ui->groupBox_Video->width(), ui->groupBox_Video->height());
    Video->setParent(ui->groupBox_Video);
    Player->setVideoOutput(Video);
    Player->setSource(QUrl::fromLocalFile(FileName));
    Video->setVisible(true);
    Video->show();

    IS_Pause = true;
    on_pushButton_Play_Pause_clicked();
}


void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{

    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    if (ptr) {
        json* itemj = (json*)ptr;
        (*itemj)["Expanded"] = false;
        foldertree->saveTreeWidgetToJson();
    } else {
        qDebug() << "Item is not rended yet.";
    }
}


void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    if (ptr) {
        json* itemj = (json*)ptr;
        (*itemj)["Expanded"] = true;
        foldertree->saveTreeWidgetToJson();
    } else {
        qDebug() << "Item is not rended yet.";
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    CustomTreeWidgetItem *_item = (CustomTreeWidgetItem *)item;
    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    json* itemj = (json*)ptr;


    if ((*itemj)["isfile"]) {
        std::string path = (*itemj)["path"];
        QString Qpath = QString::fromStdString(path);

        if ((*itemj)["isMediaFile"] && ui->playInViewLOg_check_box->isChecked()) {
            PlayVideo(item);
        } else {
            QDesktopServices::openUrl(QUrl::fromLocalFile(Qpath));

            (*itemj)["watched"] = true;
            setNodeBackgroundColor(_item, true);
            foldertree->saveTreeWidgetToJson();
        }
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    CustomTreeWidgetItem *item = (CustomTreeWidgetItem *)ui->treeWidget->itemAt(pos);
    if (item) {
        QMenu contextMenu(this);

        uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
        json* itemj = (json*)ptr;
        qDebug() << (std::string)(*itemj)["path"];

        QAction *action1 = new QAction("Unselect", this);
        connect(action1, &QAction::triggered, this, [=]() {
            (*itemj)["watched"] = false;
            setNodeBackgroundColor(item, false);

            foldertree->saveTreeWidgetToJson();
        });
        contextMenu.addAction(action1);

        QAction *action2 = new QAction("Open with default", this);
        connect(action2, &QAction::triggered, this, [=]() {
            QString Qpath = QString::fromStdString((std::string)(*itemj)["path"]);
            openFileWithDefaultApp(Qpath);

            (*itemj)["watched"] = true;
            setNodeBackgroundColor(item, true);

            foldertree->saveTreeWidgetToJson();
        });
        contextMenu.addAction(action2);

        contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::openFileWithDefaultApp(const QString &filePath) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}



void MainWindow::on_pushButton_Tree_released()
{

    QPropertyAnimation *Buttonanim = new QPropertyAnimation(ui->pushButton_Tree, "minimumSize", this);
    Buttonanim->setDuration(115);

    QPropertyAnimation *Treeanim = new QPropertyAnimation(ui->treeWidget, "minimumSize", this);
    Treeanim->setDuration(115);

    if (!Is_TreeCollapased) {
        Is_TreeCollapased = true;


        Buttonanim->setStartValue(QSize(400, 0));
        Treeanim->setStartValue(QSize(400, 0));
        Buttonanim->setEndValue(QSize(0, 0));
        Treeanim->setEndValue(QSize(0, 0));
    
        ui->pushButton_Tree->setIcon(QIcon(":/assets/treeWidget/show-Tree.png"));
    } else {
        Is_TreeCollapased = false;


        Buttonanim->setStartValue(QSize(26, 0));
        Treeanim->setStartValue(QSize(0, 0));
        Buttonanim->setEndValue(QSize(400, 0));
        Treeanim->setEndValue(QSize(400, 0));
        
        ui->pushButton_Tree->setIcon(QIcon(":/assets/treeWidget/collapse-Tree.png"));
    }

    Buttonanim->start();
    Treeanim->start();
}

void MainWindow::on_nextButton_released()
{
    currentItem = getNextItem(ui->treeWidget, currentItem);

    if (currentItem) {
        uint64_t ptr = currentItem->data(0, Qt::UserRole).toULongLong();
        json* itemj = (json*)ptr;


        if (!(*itemj)["isfile"]) {
            currentItem->setExpanded(true);
        }

        if ((*itemj)["isMediaFile"]) {
            PlayVideo(currentItem);
        } else {
            on_nextButton_released();
        }

    } else {
        qDebug() << "No more items.";
    }
}


void MainWindow::on_prevButton_released()
{
    currentItem = getPreviousItem(ui->treeWidget, currentItem);

    if (currentItem) {
        uint64_t ptr = currentItem->data(0, Qt::UserRole).toULongLong();
        json* itemj = (json*)ptr;

        if (!(*itemj)["isfile"]) {
            currentItem->setExpanded(true);
        }

        if ((*itemj)["isMediaFile"]) {
            PlayVideo(currentItem);
        } else {
            on_prevButton_released();
        }
    } else {
        qDebug() << "No more items.";
    }
}


QTreeWidgetItem* getNextItem(QTreeWidget* tree, QTreeWidgetItem* currentItem) {
    if (!currentItem) return nullptr;

    // Get the QModelIndex of the current item
    QModelIndex currentIndex = tree->indexFromItem(currentItem);
    if (!currentIndex.isValid()) return nullptr;

    // Get the next index in the view
    QModelIndex nextIndex = tree->indexBelow(currentIndex);
    if (!nextIndex.isValid()) return nullptr;

    // Return the QTreeWidgetItem corresponding to the next index
    return tree->itemFromIndex(nextIndex);
}

QTreeWidgetItem* getPreviousItem(QTreeWidget* tree, QTreeWidgetItem* currentItem) {
    if (!currentItem) return nullptr;

    QModelIndex currentIndex = tree->indexFromItem(currentItem);
    if (!currentIndex.isValid()) return nullptr;

    QModelIndex previousIndex = tree->indexAbove(currentIndex);
    if (!previousIndex.isValid()) return nullptr;

    return tree->itemFromIndex(previousIndex);
}
