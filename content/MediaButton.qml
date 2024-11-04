/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    id: thisButton
    property string backgroundColorDefault: "transparent"
    property string backgroundColorHover: "#1a1a1a"
    property string backgroundColorDown: "#242424"
    property string buttonsColor: "#FFFFF0"

    implicitWidth: 50
    implicitHeight: 50
    leftPadding: 12
    rightPadding: 12
    hoverEnabled: true
    flat: true
    icon.cache: true
    icon.source: "../assets/images/nextvideo.svg"
    icon.color: thisButton.buttonsColor
    text: qsTr("")


    background: buttonBackground
    Rectangle {
        id: buttonBackground
        implicitWidth: 50
        implicitHeight: 50
        color: thisButton.backgroundColorDefault
        radius: 8  // Optional: rounded corner

        anchors.fill: parent
    }

    states: [
        State {
            name: "normal"

            PropertyChanges {
                target: buttonBackground
                color: thisButton.backgroundColorDefault
            }

        },
        State {
            name: "down"
            when: thisButton.down


            PropertyChanges {
                target: buttonBackground
                color: thisButton.backgroundColorDown
            }
        },
        State {
            name: "hovered"
            when: thisButton.hovered

            PropertyChanges {
                target: buttonBackground
                color: thisButton.backgroundColorHover
            }
        }
    ]
}
