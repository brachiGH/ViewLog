#include "FoldetTree.h"

Node* buildTree(const fs::path& path) {
    Node* node = new Node(path.filename().string(), fs::is_regular_file(path));

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            node->children.push_back(buildTree(entry.path()));
        }
    }

    return node;
}

void uiBuildTree(Node* node, QTreeWidgetItem* root, QTreeWidget* MainTree) {
    if (!node) return;

    QTreeWidgetItem *item;
    QString fname = QString::fromStdString(node->name);

    if (root) {
        item = new QTreeWidgetItem(root, QStringList(QString(fname)));
    } else {
        item = new QTreeWidgetItem(MainTree, QStringList(QString(fname)));
    }

    QIcon icon(":/folder_icon");
    if (node->isFile) {
        icon = QIcon(":/video_icon");
    }

    item->setIcon(0, icon);

    item->setExpanded(node->Expanded);

    for (Node* child : node->children) {
        uiBuildTree(child, item, nullptr);
    }
}
