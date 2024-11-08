import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.folderlistmodel

Item {
    id: root
    width: 400
    height: 600
    property string iconColor: "#FFFFF0"
    property string rootFolder: "file:///"
    property string backgroundColor: "#FFFFF0"

    Component {
        id: folderDelegate


        Column {
            id: delegateItem
            width: treeView.width

            // Calculate total height based on content
            height: itemRow.height + (isExpanded ? subFolderView.height : 0)

            property bool isExpanded: false
            property var subFolderModel: null
            property bool isFolder: fileIsDir

            // Main item row
            
            RowLayout {
                id: itemRow
                width: parent.width
                // height: 20

                // Indentation based on depth
                Item {
                    width: 10 * model.depth
                    height: 1
                }

                // Expand/Collapse icon for folders
                IconImage  {
                    visible: isFolder
                    source: !delegateItem.isExpanded ? "../assets/treeWidget/stylesheet-branch-closed.png" : "../assets/treeWidget/stylesheet-branch-open.png"
                    sourceSize.width: 10
                    sourceSize.height: 10
                    color: root.iconColor
                }

                IconImage  {
                    visible: !isFolder
                    source: "../assets/treeWidget/stylesheet-vline.png"
                    sourceSize.width: 10
                    sourceSize.height: 10
                    color: root.iconColor
                }

                // File/Folder icon
                Image {
                    source: isFolder ? "../assets/images/folder.svg" : "../assets/images/file.svg"
                    sourceSize.width: 13
                    sourceSize.height: 13
                }

                // File/Folder name
                Text {
                    text: fileName
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    color: "white"
                    MouseArea {
                        anchors.fill: parent
                        anchors.margins: -5
                        onClicked: {
                            delegateItem.isExpanded = !delegateItem.isExpanded
                            if (delegateItem.isExpanded && !delegateItem.subFolderModel) {
                                createSubFolder()
                            }
                            if (!isFolder) {
                                videoPlayer.source = fileURL
                                videoPlayer.play()
                            }
                        }
                    }
                }

            }

            
            // Subfolder contents
            ListView {
                id: subFolderView
                visible: delegateItem.isExpanded
                width: parent.width
                height: delegateItem.isExpanded ? contentHeight : 0
                interactive: false // Disable scrolling for nested lists
                model: delegateItem.subFolderModel
                delegate: folderDelegate
                anchors.left: parent.left
                anchors.leftMargin: 20

                // Add transition for smooth expansion/collapse
                Behavior on height {
                    NumberAnimation { duration: 100 }
                }
            }

            function createSubFolder() {
                if (isFolder) {
                    delegateItem.subFolderModel = Qt.createQmlObject(`
                        import Qt.labs.folderlistmodel 2.15
                        FolderListModel {
                            folder: "${fileURL}"
                            showDirsFirst: true
                            showDotAndDotDot: false
                        }
                    `, delegateItem)
                }
            }
        }

    }

    ScrollView {
        id: scrollView
        anchors.fill: parent
        clip: true

        ListView {
            id: treeView
            anchors.fill: parent
            model: FolderListModel {
                folder: root.rootFolder
                showDirsFirst: true
                showDotAndDotDot: false
            }
            delegate: folderDelegate
            interactive: true // Enable scrolling for the main list
        }
    }
}
