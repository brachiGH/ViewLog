#ifndef FOLDETTREE_H
#define FOLDETTREE_H
#define __TREE_WIDGET_FILE_  "/treeWidgetData.json"
#define  _SELECT_BACKGROUND_COLOR_ (192, 154, 219, 128);

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
#include "external-libs/json/json.hpp"
#include <vector>
#include <unordered_set>
#include <filesystem>
#include <cctype>

#include "customtreewidgetitem.h"


using json = nlohmann::json;

class Node {
public:
    std::string name;
    QString path;
    bool isFile;
    bool Expanded = false;
    bool watched = false;
    bool isMediaFile = false;
    std::vector<Node*> children;

    Node(const std::filesystem::path& path, bool isFile, bool isMediaFile) : name(path.filename().string()), isFile(isFile),
        isMediaFile(isMediaFile), path(QString::fromStdString(path.string())) {}

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
    json _TreeWidgetJson;
    QString _TreeJsonFilePath;
public:
    FolderTree(QTreeWidget* _MainTree, const std::filesystem::path& path);

    Node* buildNodesTree(const std::filesystem::path& path);

    void uiBuildTree();

    bool saveTreeWidgetToJson();

    void Reset();

    ~FolderTree();
private:
    void _uiBuildTree(Node* node, CustomTreeWidgetItem* parent);

    bool readTreeFile();

    void _MergeJsonFromNodes(Node* node);
};


void setNodeBackgroundColor(QTreeWidgetItem* item, bool select);

QString getLastTree();

std::string toLower(const std::string& str);

#endif // FOLDETTREE_H
