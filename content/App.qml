// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import ViewLog
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.0
import QtQuick.Window 2.15

Window {
    id: mainWindow
    width: 1334
    height: 750

    // Set the minimum and maximum size to control resizing limits
    minimumWidth: 350
    minimumHeight: 230

    property real resizeStartWidth: 0
    property real resizeStartHeight: 0
    property real targetWidth: 0
    property real targetHeight: 0


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
        anchors.right: parent.right
        width: 10
        height: parent.height
        cursorShape: Qt.SizeHorCursor
        enabled: mainScreen.isPictureInPicture

        property point lastMousePos

        onPressed: {
            lastMousePos = Qt.point(mouseX, mouseY)
        }

        onPositionChanged: {
            var deltaX = mouseX - lastMousePos.x

            // Apply size constraints
            var newWidth = Math.max(mainWindow.width + deltaX, mainWindow.minimumWidth)

            mainWindow.width = newWidth
        }
    }

    MouseArea {
        anchors.bottom: parent.bottom
        height: 10
        width: parent.width
        cursorShape: Qt.SizeVerCursor
        enabled: mainScreen.isPictureInPicture

        property point lastMousePos

        onPressed: {
            lastMousePos = Qt.point(mouseX, mouseY)
        }

        onPositionChanged: {
            var deltaY = mouseY - lastMousePos.y

            // Apply size constraints
            var newHeight = Math.max(mainWindow.height + deltaY, mainWindow.minimumHeight)

            mainWindow.height = newHeight
        }
    }

    // Corner for resizing (bottom-right)
    MouseArea {
        id:mouseArea
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 10
        height: 10
        cursorShape: Qt.SizeFDiagCursor
        enabled: mainScreen.isPictureInPicture

        property point lastMousePos

        onPressed: {
            lastMousePos = Qt.point(mouseX, mouseY)
        }

        onPositionChanged: {
            var deltaX = mouseX - lastMousePos.x
            var deltaY = mouseY - lastMousePos.y

            // Apply size constraints
            var newWidth = Math.max(mainWindow.width + deltaX, mainWindow.minimumWidth)
            var newHeight = Math.max(mainWindow.height + deltaY, mainWindow.minimumHeight)

            mainWindow.width = newWidth
            mainWindow.height = newHeight
        }
    }



    Behavior on width {
        NumberAnimation {
            duration: 50
            easing.type: Easing.Linear
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: 50
            easing.type: Easing.Linear
        }
    }
}

