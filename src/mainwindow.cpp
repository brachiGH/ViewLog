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

    ui->pushButton_Play_Pause->setIcon(QIcon(":/assets/controls/play_icon.png"));
    ui->pushButton_Stop->setIcon(QIcon(":/assets/controls/box_icon.png"));
    ui->pushButton_Seek_Backward->setIcon(QIcon(":/assets/controls/play_back_icon.png"));
    ui->pushButton_Seek_Forward->setIcon(QIcon(":/assets/controls/play_forward_icon.png"));
    ui->pushButton_Volume->setIcon(QIcon(":/assets/controls/volume_high_icon.png"));

    ui->pushButton_Tree->setIcon(QIcon(":/assets/treeWidget/collapse-Tree.png"));

    ui->horizontalSlider_Volume->setMinimum(0);
    ui->horizontalSlider_Volume->setMaximum(100);
    ui->horizontalSlider_Volume->setValue(30);

    AudioOutput->setVolume(ui->horizontalSlider_Volume->value());

    connect(Player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(Player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    // #18171a bacground color
    ui->quickWidget->setClearColor(QColor(24, 23, 26));


    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/duration_slider.qml")));

    QObject *rootObject = ui->quickWidget->rootObject();
    sliderDurationObject = rootObject->findChild<QObject*>("sliderDurationObject");

    connect(rootObject, SIGNAL(slider_duration_Changed(int)), this, SLOT(on_slider_duration_Changed(int)));
    connect(rootObject, SIGNAL(durationsliderPressedChanged(bool)), this, SLOT(onDurationSliderPressedChanged(bool)));


    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    ui->treeWidget->setColumnCount(1);

    QString rootPath = getLastTree();
    if (rootPath != "") {
        foldertree = new FolderTree(ui->treeWidget, rootPath.toStdU16String());
        foldertree->uiBuildTree();
    }


    qInfo() << "Creating the treewidget";
    ui->treeWidget->setStyleSheet(_TREE_WIDGET_STYLE_);
    ui->treeWidget->sortItems(0, Qt::AscendingOrder);

    qInfo() << "Finished building UI";

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::durationChanged(qint64 duration)
{
    mDuration = duration / 1000;
    sliderDurationObject->setProperty("to", mDuration);
}

void MainWindow::onDurationSliderPressedChanged(bool pressed)
{
    qDebug() << "duration changed";

    durationSliderPressPressed = pressed;
}

void MainWindow::positionChanged(qint64 duration)
{
    if (!durationSliderPressPressed)
    {
        programmaticChangeDuration = true;
        sliderDurationObject->setProperty("value", duration / 1000);
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

    qInfo() << "Loading videos from " << folderPath.toStdString();

    if (folderPath != "") {
        std::filesystem::path fsPath = folderPath.toStdU16String();  // Convert QString to std::u16string

        ui->treeWidget->clear();
        if (foldertree != nullptr) {
            delete foldertree;
        }
        ui->treeWidget->clear();
        foldertree = new FolderTree(ui->treeWidget, fsPath);
        foldertree->uiBuildTree();
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
    AudioOutput->setVolume(value);
}


void MainWindow::on_pushButton_Seek_Backward_clicked()
{
    // ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() - 20);
    // Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
}


void MainWindow::on_pushButton_Seek_Forward_clicked()
{
    // ui->horizontalSlider_Duration->setValue(ui->horizontalSlider_Duration->value() + 20);
    // Player->setPosition(ui->horizontalSlider_Duration->value() * 1000);
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

void MainWindow::PlayVideo(QString FileName)
{
    qInfo() << "playing " << FileName;
    setWindowTitle("ViewLog - " + FileName);

    if (!Video) {
        delete Video;
    }

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
        if (!(*itemj)["Expanded"]) {
            (*itemj)["Expanded"] = false;
            foldertree->saveTreeWidgetToJson();
        }
    } else {
        qDebug() << "Item is not rended yet.";
    }
}


void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    if (ptr) {
        json* itemj = (json*)ptr;
        if (!(*itemj)["Expanded"]) {
            (*itemj)["Expanded"] = true;
            foldertree->saveTreeWidgetToJson();
        }
    } else {
        qDebug() << "Item is not rended yet.";
    }
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    uint64_t ptr = item->data(0, Qt::UserRole).toULongLong();
    json* itemj = (json*)ptr;
    if ((*itemj)["isfile"]) {
        std::string path = (*itemj)["path"];
        QString Qpath = QString::fromStdString(path);
        if ((*itemj)["isMediaFile"] && ui->playInViewLOg_check_box->isChecked()) {
            PlayVideo(Qpath);
        } else {
            QDesktopServices::openUrl(QUrl::fromLocalFile(Qpath));
        }

        (*itemj)["watched"] = true;
        setNodeBackgroundColor(item, true);
        foldertree->saveTreeWidgetToJson();
    }
}

void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);
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
    Buttonanim->setDuration(200);

    QPropertyAnimation *Treeanim = new QPropertyAnimation(ui->treeWidget, "minimumSize", this);
    Treeanim->setDuration(200);

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