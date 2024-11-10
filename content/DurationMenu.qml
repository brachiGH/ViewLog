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
            
            onPressedChanged: {
                videoPlayer.position = durationSlider.visualPosition * videoPlayer.duration
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
