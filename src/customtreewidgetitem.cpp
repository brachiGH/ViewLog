// customtreewidgetitem.cpp
#include "headers/customtreewidgetitem.h"
#include <QTreeWidget>

CustomTreeWidgetItem::CustomTreeWidgetItem(QTreeWidget *parent)
    : QTreeWidgetItem(parent) {}


CustomTreeWidgetItem::CustomTreeWidgetItem(QTreeWidget *treeview, const QStringList &strings, int type)
    : QTreeWidgetItem(treeview, strings, type) {}


CustomTreeWidgetItem::CustomTreeWidgetItem(QTreeWidgetItem *parent, const QStringList &strings, int type)
    : QTreeWidgetItem(parent, strings, type) {}


bool CustomTreeWidgetItem::operator<(const QTreeWidgetItem &other) const {
    int column = treeWidget()->sortColumn();
    QString filename = text(column);
    QString otherfilename = other.text(column);

    int i = 0;
    int minlen = std::min(filename.length(), otherfilename.length());

    QString stringNum, otherstringNum;
    while (true) {
        if (i > minlen-1) {
            if (stringNum != otherstringNum) {
                return stringNum.toDouble() < otherstringNum.toDouble();
            }

            if (filename.length() == otherfilename.length()) {
                return true;
            }

            if (i > filename.length()) {
                return true;
            }

            return false;
        }

        int num = filename[i].digitValue();
        int othernum = otherfilename[i].digitValue();
        if (num != -1 && othernum != -1) {
            stringNum += filename[i];
            otherstringNum += otherfilename[i];

            i++;
            continue;
        } else if (stringNum != otherstringNum) {
                if (num != -1 || othernum != -1) {
                    int j = i, num ,othernum;
                    bool nolongerAnumber = false, othernolongerAnumber = false;

                    do {
                        num = filename[j].digitValue();
                        othernum = otherfilename[j].digitValue();

                        if (num !=-1 && !nolongerAnumber) {
                            stringNum += filename[j];
                        } else {
                            nolongerAnumber = true;
                        }

                        if (othernum !=-1 && !othernolongerAnumber) {
                            otherstringNum += otherfilename[j];
                        } else {
                            othernolongerAnumber = true;
                        }

                        j++;
                    }
                    while ((!nolongerAnumber || !othernolongerAnumber) && j < minlen);

                    return stringNum.toDouble() < otherstringNum.toDouble();
                }

            return stringNum.toDouble() < otherstringNum.toDouble();
        } else {
            otherstringNum = "";
            stringNum = "";
            if (filename[i] != otherfilename[i]) {
                return filename[i].toUpper() < otherfilename[i].toUpper();
            }
        }

        i++;
    }
}
