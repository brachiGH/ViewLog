#include "FoldetTree.h"

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
    rootJson["files"];

    // Write the modified content to a writable location
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(writableLocation);  // Ensure the directory exists
    filePath = writableLocation + "/tree.json";


    readTreeFile();
    rootJson["lastFolder"] = path.string();

    root = buildTree(path);
    saveJsonTree();
    
    firstUpdateFromJsonfile = false;
}



Node* FolderTree::buildTree(const fs::path& path)
{
    Node* node = new Node(path, fs::is_regular_file(path));

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            node->children.push_back(buildTree(entry.path()));
        }
    }
    
    return node;
}

std::vector<bool> FolderTree::getStats(std::string path)
{
    for (auto file : rootJson["files"]) {
        if (file["path"] == path) {
            return {file["Expanded"], file["watched"]};
        }
    }

    return {false, false};
}


void FolderTree::uiBuildTree() {
    _uiBuildTree(root, nullptr);

    for (int i = 0; i < MainTree->columnCount(); ++i) {
        MainTree->resizeColumnToContents(i);
    }
}

void FolderTree::_uiBuildTree(Node* node, QTreeWidgetItem* parent) {
    if (!node) return;

    QTreeWidgetItem *item;
    QString fname = QString::fromStdString(node->name);

    if (parent) {
        item = new QTreeWidgetItem(parent, QStringList(QString(fname)));
    } else {
        item = new QTreeWidgetItem(MainTree, QStringList(QString(fname)));
    }

    QIcon icon;
    if (node->isFile) {
        std::string fileextension = toLower(getFileExtension(node->path.toStdString()));
        if (videoExtensions.find(fileextension) != videoExtensions.end()) {
            icon = QIcon(":/video_icon");
        } else {
            icon = QIcon(":/file_icon");
            node->id = -node->id;
        }
    } else {
        icon = QIcon(":/folder_icon");
    }

    item->setIcon(0, icon);
    item->setExpanded(node->Expanded);
    item->setData(0, Qt::UserRole, node->id);

    if (node->watched && node->isFile) {
        setColorItem(item, true);
    }

    if (!node->isFile) {
        item->setExpanded(node->Expanded);
    }


    for (Node* child : node->children) {
        _uiBuildTree(child, item);
    }
}


void FolderTree::saveJsonTree() {
    nodeToJson(root);


    QFile outFile(filePath);
    if (outFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&outFile);
        out << QString::fromStdString(rootJson.dump(4));
        outFile.close();
    } else {
        std::cerr << "Failed to open file for writing: " << outFile.errorString().toStdString() << std::endl;
    }
}


void FolderTree::readTreeFile()
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        QString jsonString = in.readAll();
        file.close();

        if (!jsonString.isEmpty()) {
            rootJson = json::parse(jsonString.toStdString());
        }
    }
}


FolderTree::~FolderTree() {
    delete root;
}

Node* FolderTree::getroot() {
    return root;
}


void FolderTree::nodeToJson(Node* node) {
    json j;
    j["path"] = node->path.toStdString();
    j["id"] = node->id;

    if (firstUpdateFromJsonfile) {
        std::vector<bool> s = getStats(node->path.toStdString());

        j["Expanded"] = (bool)s[0];
        j["watched"] = (bool)s[1];

        node->Expanded = (bool)s[0];
        node->watched = (bool)s[1];
    } else {
        j["Expanded"] = node->Expanded;
        j["watched"] = node->watched;
    }

    rootJson["files"][node->path.toStdString()] = j;

    for (Node* child : node->children) {
        nodeToJson(child);
    }
}


Node* getPathFromID(Node* item, int id)
{
    // negative ids are for none video file (note that folder have positive ids)
    if (item == nullptr) {
        return nullptr;
    }

    if (item->id == id) {
        return item;
    }

    for (Node* child : item->children) {
        Node* result = getPathFromID(child, id);
        if (result != nullptr) {
            return result;
        }
    }

    return nullptr;
}

void setColorItem(QTreeWidgetItem* item, bool select) {
    QColor color(0, 49, 76);
    if (!select) {
        color = QColor(255, 255, 255);
    }
    QBrush brush(color);

    item->setBackground(0, brush);
}


QString getLastTree() {
    json rootJson;
    rootJson["lastFolder"] = "";

    // Write the modified content to a writable location
    QString writableLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(writableLocation);  // Ensure the directory exists
    QString filePath = writableLocation + "/tree.json";

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

std::string getFileExtension(const std::string& filePath) {
    fs::path path(filePath);
    return path.extension().string();
}

std::string toLower(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = std::tolower(static_cast<unsigned char>(c));
    }
    return result;
}
