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

namespace fs = std::filesystem;

class Node {
public:
    std::string name;
    bool isFile;
    bool Expanded = true;
    std::vector<Node*> children;

    Node(const std::string& name, bool isFile) : name(name), isFile(isFile) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};


Node* buildTree(const fs::path& path);

void uiBuildTree(Node* node, QTreeWidgetItem *root, QTreeWidget* MainTree = nullptr);

#endif // FOLDETTREE_H
