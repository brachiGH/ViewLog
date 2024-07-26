#ifndef FOLDETTREE_H
#define FOLDETTREE_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <QtWidgets>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <iostream>
#include <fstream>
#include <vector>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "external-libs/json.hpp"
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <cctype>


namespace fs = std::filesystem;
using json = nlohmann::json;
static int nextID = 0;

class Node {
public:
    std::string name;
    QString path;
    int id;
    bool isFile;
    bool Expanded = false;
    bool watched = false;
    std::vector<Node*> children;

    Node(const fs::path& path, bool isFile) : name(path.filename().string()), isFile(isFile),
        path(QString::fromStdString(path.string())), id(nextID++) {

    }

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};



class FolderTree
{
    Node* root;
    QTreeWidget* MainTree;
    json rootJson;
    QString filePath;
    bool firstUpdateFromJsonfile = true;
public:
    FolderTree(QTreeWidget* _MainTree, const fs::path& path);

    Node* buildTree(const fs::path& path);

    std::vector<bool> getStats(std::string path); // {Expanded, watched}

    void uiBuildTree();

    void saveJsonTree();

    Node* getroot();

    ~FolderTree();
private:
    void _uiBuildTree(Node* node, QTreeWidgetItem* parent);

    void readTreeFile();

    void nodeToJson(Node* node);
};


Node* getPathFromID(Node* item, int id);

void setColorItem(QTreeWidgetItem* item, bool select);

QString getLastTree();

std::string getFileExtension(const std::string& filePath);

std::string toLower(const std::string& str);

#endif // FOLDETTREE_H
