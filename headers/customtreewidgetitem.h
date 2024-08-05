#ifndef CUSTOMTREEWIDGETITEM_H
#define CUSTOMTREEWIDGETITEM_H

#include <QTreeWidgetItem>


class CustomTreeWidgetItem : public QTreeWidgetItem {
public:
    enum ItemType { Type = 0, UserType = 1000 };
    CustomTreeWidgetItem(QTreeWidget *parent = nullptr);
    CustomTreeWidgetItem(QTreeWidget *treeview, const QStringList &strings, int type = Type);
    CustomTreeWidgetItem(QTreeWidgetItem *parent, const QStringList &strings, int type = Type);

    bool operator<(const QTreeWidgetItem &other) const override;
};

#endif // CUSTOMTREEWIDGETITEM_H
