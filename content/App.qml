// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import ViewLog
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.0

Window {
    id:mainWindow
    width: 1334
    height: 750

    // Set the minimum and maximum size to control resizing limits
    minimumWidth: 200
    minimumHeight: 150

    // Property to store the initial mouse position
    property var resizeStartMouseX
    property var resizeStartMouseY
    property var resizeStartWidth
    property var resizeStartHeight

    visible: true
    visibility: if (mainScreen.isFullscreen) {
                    Window.FullScreen
                } else {
                    Window.Maximized
                }
    title: "ViewLog"




    Screen01 {
        id: mainScreen
        anchors.fill: parent
    }


    // Corners and edges for resizing
    MouseArea {
        anchors.left: parent.left
        width: 10
        height: parent.height
        cursorShape: Qt.SizeHorCursor
        enabled: mainScreen.isPictureInPicture

        onPressed: {
            mainWindow.resizeStartMouseX = mouse.x
            mainWindow.resizeStartWidth = mainWindow.width
        }

        onMouseXChanged: {
            mainWindow.width = mainWindow.resizeStartWidth + (mouse.x - mainWindow.resizeStartMouseX)
        }
    }

    MouseArea {
        anchors.right: parent.right
        width: 10
        height: parent.height
        cursorShape: Qt.SizeHorCursor
        enabled: mainScreen.isPictureInPicture

        onPressed: {
            mainWindow.resizeStartMouseX = mouse.x
            mainWindow.resizeStartWidth = mainWindow.width
        }

        onMouseXChanged: {
            mainWindow.width = mainWindow.resizeStartWidth + (mouse.x - mainWindow.resizeStartMouseX)
        }
    }

    MouseArea {
        anchors.bottom: parent.bottom
        height: 10
        width: parent.width
        cursorShape: Qt.SizeVerCursor
        enabled: mainScreen.isPictureInPicture

        onPressed: {
            mainWindow.resizeStartMouseY = mouse.y
            mainWindow.resizeStartHeight = mainWindow.height
        }

        onMouseYChanged: {
            mainWindow.height = mainWindow.resizeStartHeight + (mouse.y - mainWindow.resizeStartMouseY)
        }
    }

    // Corner for resizing (bottom-right)
    MouseArea {
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 10
        height: 10
        cursorShape: Qt.SizeFDiagCursor
        enabled: mainScreen.isPictureInPicture

        onPressed: {
            mainWindow.resizeStartMouseX = mouse.x
            mainWindow.resizeStartMouseY = mouse.y
            mainWindow.resizeStartWidth = mainWindow.width
            mainWindow.resizeStartHeight = mainWindow.height
        }

        onMouseXChanged: {
            mainWindow.width = mainWindow.resizeStartWidth + (mouse.x - mainWindow.resizeStartMouseX)
        }

        onMouseYChanged: {
            mainWindow.height = mainWindow.resizeStartHeight + (mouse.y - mainWindow.resizeStartMouseY)
        }
    }


    
}

