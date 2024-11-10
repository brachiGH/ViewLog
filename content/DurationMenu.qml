import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: root
    implicitWidth: durationLayout.implicitWidth
    implicitHeight: durationLayout.implicitHeight

    property bool showHideDurationMenu: false
    property string elapsedTimeUiText: ""
    property string totalDurationUiText: ""
    property color textColor: "white"
    property bool seeking: false

    RowLayout {
        id: durationLayout
        visible: !root.showHideDurationMenu
        anchors.fill: parent

        Text {
            id: elapsedTime
            text: root.elapsedTimeUiText
            color: root.textColor
            font.pixelSize: 17
        }

        Slider {
            id: durationSlider
            Layout.fillWidth: true
            value: videoPlayer.position / Math.max(videoPlayer.duration, 1) // Prevent division by zero
            // live: true
            // width: 200
            height: 5
            
            background: Rectangle {
                x: 0
                y: (parent.height - height) / 2
                width: parent.width
                height: 4
                radius: 2
                color: "#4D4D4D"

                Rectangle {
                    width: durationSlider.visualPosition * parent.width
                    height: parent.height
                    color: "#1DB954"
                    radius: 2
                }
            }


            handle: Rectangle {
                x: durationSlider.leftPadding + durationSlider.visualPosition * (durationSlider.availableWidth - width)
                y: durationSlider.topPadding + durationSlider.availableHeight / 2 - height / 2
                width: 13
                height: 13
                radius: 8
                color: durationSlider.pressed ? "#1DB954" : "#FFFFFF"
                border.color: "#1DB954"
                border.width: 2

                Behavior on color {
                    ColorAnimation { duration: 100 }
                }
            }

            onPressedChanged: {
                if (pressed) {
                    seeking = true
                } else {
                    seeking = false
                }
            }

            onMoved: {
                if (seeking) {
                    videoPlayer.position = durationSlider.visualPosition * videoPlayer.duration
                }
            }
            
        }

        Text {
            id: totalDuration
            text: root.totalDurationUiText
            color: root.textColor
            font.pixelSize: 17
        }
    }
}
