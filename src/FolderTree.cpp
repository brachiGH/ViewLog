#include "headers/FolderTree.h"

namespace fs = std::filesystem;

std::unordered_set<std::string> videoExtensions = {
    ".3gp",   // 3GP (3GPP file format)
    ".3g2",   // 3GP2 (3GPP2 file format)
    ".avi",   // AVI (Audio Video Interleaved)
    ".flv",   // FLV (Flash Video)
    ".mov",   // QuickTime / MOV
    ".mp4",   // MP4 (MPEG-4 Part 14)
    ".mkv",   // Matroska
    ".webm",  // WebM
    ".mpeg",  // MPEG-1 Systems / MPEG program stream
    ".mpg",   // MPEG-1 Systems / MPEG program stream
    ".f4v",   // F4V Adobe Flash Video
    ".av1",   // AV1 Annex B
    ".amv",   // AMV (Anime Music Video)
    ".swf",   // SWF (ShockWave Flash)
    ".vob",   // MPEG-2 PS (VOB)
    ".m2ts",  // MPEG-2 TS (MPEG-2 Transport Stream)
    ".ts",    // MPEG-2 TS (MPEG-2 Transport Stream)
    ".dvr-ms",// DVR-MS (Microsoft Digital Video Recording)
    ".xvid",  // XviD
    ".xmv",   // Microsoft XMV
    ".dxx"    // DirectX Movie
};

FolderTree::FolderTree(QTreeWidget* _MainTree, const fs::path& path)
{
    MainTree = _MainTree;

    // Write the modified content to a writable location
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(writableLocation);  // Ensure the directory exists
    _TreeJsonFilePath = writableLocation + __TREE_WIDGET_FILE_;


    if (!readTreeFile()) {
        qInfo() << __TREE_WIDGET_FILE_ << " Does not exist";
        _TreeWidgetJson["nodes"] = {};
    }
    _TreeWidgetJson["lastFolder"] = path.string();

    root = buildNodesTree(path);
    _MergeJsonFromNodes(root);
    saveTreeWidgetToJson();
}



Node* FolderTree::buildNodesTree(const fs::path& path)
{
    bool isfile = fs::is_regular_file(path);
    bool isMediatFile = false;  
    if (isfile && path.has_extension()) {
        std::string fileextension = toLower(path.extension().string());
        if (videoExtensions.find(fileextension) != videoExtensions.end()) {
            isMediatFile = true;
        }
    }
    Node* node = new Node(path, isfile, isMediatFile);

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            node->children.push_back(buildNodesTree(entry.path()));
        }
    }
    
    return node;
}

void FolderTree::uiBuildTree() {
    _uiBuildTree(root, nullptr);

    for (int i = 0; i < MainTree->columnCount(); ++i) {
        MainTree->resizeColumnToContents(i);
    }
}

void FolderTree::_uiBuildTree(Node* node, CustomTreeWidgetItem* parent) {
    if (!node) return;

    CustomTreeWidgetItem *item;
    QString fname = QString::fromStdString(node->name);

    if (parent) {
        item = new CustomTreeWidgetItem(parent, QStringList(QString(fname)));
    } else {
        item = new CustomTreeWidgetItem(MainTree, QStringList(QString(fname)));
    }

    QIcon icon;
    if (node->isFile) {
        if (node->isMediaFile) {
            icon = QIcon(":/video_icon");
        } else {
            icon = QIcon(":/file_icon");
        }


        if (node->watched) {
            setNodeBackgroundColor(item, true);
        }
    } else {
        icon = QIcon(":/folder_icon");
    }

    item->setIcon(0, icon);
    item->setExpanded(node->Expanded);
    item->setData(0, Qt::UserRole, (uint64_t)(&_TreeWidgetJson["nodes"][node->path.toStdString()]));

    if (!node->isFile) {
        item->setExpanded(node->Expanded);
    }

    for (Node* child : node->children) {
        _uiBuildTree(child, item);
    }
}


bool FolderTree::saveTreeWidgetToJson() {
    qInfo() << "Saving json file " <<_TreeJsonFilePath;
    QFile outFile(_TreeJsonFilePath);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&outFile);
        out << QString::fromStdString(_TreeWidgetJson.dump(4));
        outFile.close();
    } else {
        qWarning() << "Failed to open file for writing: " << outFile.errorString().toStdString();
        return 0;
    }

    return 1;
}


bool FolderTree::readTreeFile()
{
    QFile file(_TreeJsonFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString jsonString = in.readAll();
        file.close();

        if (!jsonString.isEmpty()) {
            _TreeWidgetJson = json::parse(jsonString.toStdString());
        }
    } else {
        return false;
    }

    return true;
}


FolderTree::~FolderTree() {
    delete root;
}


void FolderTree::_MergeJsonFromNodes(Node* node) {
    std::string path = node->path.toStdString();
    auto fNode_res = _TreeWidgetJson["nodes"].find(path);

    if (fNode_res != _TreeWidgetJson["nodes"].end()) {
        node->watched = _TreeWidgetJson["nodes"][path]["watched"];
        node->Expanded = _TreeWidgetJson["nodes"][path]["Expanded"];
    } else {
        json j;
        j["path"] = path;
        j["isMediaFile"] = node->isMediaFile;
        j["isfile"] = node->isFile;
        j["watched"] = false;
        j["Expanded"] = false;
        
        _TreeWidgetJson["nodes"][path] = j;
    }


    for (Node* child : node->children) {
        _MergeJsonFromNodes(child);
    }
}

void FolderTree::Reset()
{
    QFile file(_TreeJsonFilePath);
    if (file.remove()) {
        qDebug() << "File deleted successfully:" << _TreeJsonFilePath;
    } else {
        qDebug() << "Failed to delete file:" << _TreeJsonFilePath << "Error:" << file.errorString();
    }

    MainTree->clear();
}


void setNodeBackgroundColor(QTreeWidgetItem* item, bool select) {
    QBrush defaultBrush;
    if (select) {
        QColor color _SELECT_BACKGROUND_COLOR_;
        defaultBrush = QBrush(color);
    }

    item->setBackground(0, defaultBrush);
}


QString getLastTree() {
    json rootJson;
    rootJson["lastFolder"] = "";

    // Write the modified content to a writable location
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(writableLocation);  // Ensure the directory exists
    QString filePath = writableLocation + __TREE_WIDGET_FILE_;

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString jsonString = in.readAll();
        file.close();

        if (!jsonString.isEmpty()) {
            rootJson = json::parse(jsonString.toStdString());
        }
    }

    if (rootJson["lastFolder"] != "") {
        if (fs::is_directory(rootJson["lastFolder"])) {
            return QString::fromStdString(rootJson["lastFolder"]);
        }

    }
    
    return QString::fromStdString("");
}


std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}
