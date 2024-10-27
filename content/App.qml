// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick 6.5
import ViewLog
import QtQuick.Controls 6.5
import QtQuick.Layouts 1.0

Window {
    width: 1334
    height: 750


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

}

