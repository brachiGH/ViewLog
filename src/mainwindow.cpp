#include "headers/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Player = new QMediaPlayer();
    AudioOutput = new QAudioOutput();
    Player->setAudioOutput(AudioOutput);

    ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->pushButton_Stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->pushButton_Seek_Backward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->pushButton_Seek_Forward->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);

    AudioOutput->setVolume(ui->horizontalSlider_Volume->value());

    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    ui->horizontalSlider_Duration->setRange(0, Player->duration() / 1000);

    ui->horizontalSlider_Duration->setStyleSheet(
        "QSlider::handle:horizontal {"
            "background: transparent;"
            "width: 0px;"
        "}");


    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->treeWidget->setColumnCount(1);

    QString rootPath = getLastTree();
    if (rootPath != "") {
        foldertree = new FolderTree(ui->treeWidget, rootPath.toStdU16String());
        foldertree->uiBuildTree();
    }

    ui->treeWidget->setStyleSheet(
        "QTreeView::branch:has-siblings:!adjoins-item {"
        "    border-image: url(:/images/vline.png) 0;"
        "}"
        "QTreeView::branch:has-siblings:adjoins-item {"
        "    border-image: url(:/images/branch-more.png) 0;"
        "}"
        "QTreeView::branch:!has-children:!has-siblings:adjoins-item {"
        "    border-image: url(:/images/branch-end.png) 0;"
        "}"
        "QTreeView::branch:has-children:!has-siblings:closed,"
        "QTreeView::branch:closed:has-children:has-siblings {"
        "    border-image: none;"
        "    image: url(:/images/branch-closed.png);"
        "}"
        "QTreeView::branch:open:has-children:!has-siblings,"
        "QTreeView::branch:open:has-children:has-siblings {"
        "    border-image: none;"
        "    image: url(:/images/branch-open.png);"
        "}"
    );
}

MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::on_actionOpen_triggered()
{
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder"));

    std::cout << folderPath.toStdString() << std::endl;
    // PlayVideo(FileName);

    if (folderPath != "") {
        std::filesystem::path fsPath = folderPath.toStdU16String();  // Convert QString to std::u16string

        if (foldertree != nullptr) {
            delete foldertree;
        }
        ui->treeWidget->clear();
        foldertree = new FolderTree(ui->treeWidget, fsPath);
        foldertree->uiBuildTree();
    }
}


void MainWindow::on_horizontalSlider_Duration_valueChanged(int value)
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
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        IS_Pause = true;
        Player->pause();
        ui->pushButton_Play_Pause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
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
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        AudioOutput->setMuted(true);
    }
    else
    {
        IS_Muted = false;
        ui->pushButton_Volume->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        AudioOutput->setMuted(false);
    }
}


void MainWindow::on_horizontalSlider_Volume_valueChanged(int value)
{
    AudioOutput->setVolume(value);
}


void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}


void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
    Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}



void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    int id = item->data(0, Qt::UserRole).toInt();
    Node* node = getPathFromID(foldertree->getroot(), id);
    if (node) {
        std::cout << node->name << std::endl;
        if (!(node->isFile)) {
            node->Expanded = !(item->isExpanded());
            item->setExpanded(node->Expanded);
        }

        foldertree->saveJsonTree();
    } else {
        std::cout<<"error file not located."<<std::endl;
    }
}

void MainWindow::PlayVideo(QString FileName)
{
    Video = new QVideoWidget();
    Video->setGeometry(5, 5, ui->groupBox_Video->width() - 10, ui->groupBox_Video->height() - 10);
    Video->setParent(ui->groupBox_Video);
    Player->setVideoOutput(Video);
    Player->setSource(QUrl::fromLocalFile(FileName));
    Video->setVisible(true);
    Video->show();

    on_pushButton_Play_Pause_clicked();
}


void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem *item)
{
    int id = item->data(0, Qt::UserRole).toInt();
    Node* node = getPathFromID(foldertree->getroot(), id);
    if (node) {
        if (node->Expanded) {
            node->Expanded = false;
            foldertree->saveJsonTree();
        }
    } else {
        std::cout<<"error file not located."<<std::endl;
    }
}


void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    int id = item->data(0, Qt::UserRole).toInt();
    Node* node = getPathFromID(foldertree->getroot(), id);
    if (node) {
        if (!node->Expanded) {
            node->Expanded = true;
            foldertree->saveJsonTree();
        }
    } else {
        std::cout<<"error file not located."<<std::endl;
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    int id = item->data(0, Qt::UserRole).toInt();
    Node* node = getPathFromID(foldertree->getroot(), id);
    if (node) {
        std::cout << node->name << std::endl;
        if (node->isFile) {
            if (id > 0 && ui->playInViewLOg_check_box->isChecked()) {
                PlayVideo(node->path);
            } else {
                QDesktopServices::openUrl(QUrl::fromLocalFile(node->path));
            }

            node->watched = true;
            setColorItem(item, true);
        }

        foldertree->saveJsonTree();
    } else {
        std::cout<<"error file not located."<<std::endl;
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
    if (item) {
        QMenu contextMenu(this);

        QAction *action1 = new QAction("Unselect", this);
        connect(action1, &QAction::triggered, this, [=]() {
            int id = item->data(0, Qt::UserRole).toInt();
            Node* node = getPathFromID(foldertree->getroot(), id);
            if (node) {
                std::cout << node->name << std::endl;
                node->watched = false;
                setColorItem(item, false);

                foldertree->saveJsonTree();
            } else {
                std::cout<<"error file not located."<<std::endl;
            }
        });
        contextMenu.addAction(action1);

        QAction *action2 = new QAction("Open with default", this);
        connect(action2, &QAction::triggered, this, [=]() {
            int id = item->data(0, Qt::UserRole).toInt();
            Node* node = getPathFromID(foldertree->getroot(), id);
            if (node) {
                std::cout << node->name << std::endl;
                openFileWithDefaultApp(node->path);

                node->watched = true;
                setColorItem(item, true);

                foldertree->saveJsonTree();
            } else {
                std::cout<<"error file not located."<<std::endl;
            }
        });
        contextMenu.addAction(action2);

        contextMenu.exec(ui->treeWidget->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::openFileWithDefaultApp(const QString &filePath) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
