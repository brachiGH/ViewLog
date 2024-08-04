import QtQuick 6.7
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12

Item {
    width: 9000
    height: 35
    Material.theme: Material.Dark

    Slider {
        id: slider
        objectName: "sliderDurationObject"
        from: 0
        to: 100
        value: 0
        
        anchors.left: parent.left
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter


        handle: Rectangle {
            border.color: "transparent"
        }

        onValueChanged: {
            slider_duration_Changed(value);
        }

        onPressedChanged: {
            durationsliderPressedChanged(slider.pressed);
        }
    }

    signal slider_duration_Changed(int newValue)
    signal durationsliderPressedChanged(bool pressed)
}
