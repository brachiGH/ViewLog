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
    property real position: 0
    property real duration: 1

    // Changed from signal to function to avoid duplicate signal error
    function updatePosition(newPosition) {
        position = newPosition
    }

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
            value: root.position / Math.max(root.duration, 1) // Prevent division by zero
            onMoved: {
                root.updatePosition(durationSlider.visualPosition * root.duration)
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
