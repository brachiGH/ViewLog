import QtQuick 6.5
import QtQuick.Layouts 1.15
import QtQuick.Controls 6.5

Item {
    id: root
    implicitWidth: controlsLayout.implicitWidth
    implicitHeight: controlsLayout.implicitHeight

    // Expose pausePlayButton directly
    property alias pausePlayButton: pausePlayButton

    // Other properties
    property bool isPaused: true
    property bool isFullscreen: false
    property bool showVolumeSlider: false
    property bool showPlaybackSpeedSlider: false
    property bool showSubtitlesAndAudioSelector: false
    property real lastVolumeValue: 1.0
    property color textColor: "white"
    property var videoPlayer: null
    property alias volumeSliderValue: volumeSlider.value

    // Signals
    signal volumeSliderVisibilityChanged(bool visible)
    signal speedSliderVisibilityChanged(bool visible)
    signal subtitlesAndAudioSelectorChanged(bool visible)
    signal fullscreenChanged(bool fullscreen)

    RowLayout {
        id: controlsLayout
        anchors.fill: parent
        Layout.bottomMargin: 10

        MediaButton {
            id: pausePlayButton
            icon.source: root.isPaused ?
                "../assets/images/pause.svg" :
                "../assets/images/play.svg"
            onClicked: {
                if (root.isPaused) {
                    videoPlayer.pause()
                } else {
                    videoPlayer.play()
                }
            }
        }

        MediaButton {
            id: seekBackButton
            icon.source: "../assets/images/seekback.svg"
            onClicked: {
                videoPlayer.position -= videoPlayer.position < 10 ?
                    videoPlayer.position : 10000
            }
        }

        MediaButton {
            id: seekForwardButton
            icon.source: "../assets/images/seekforward.svg"
            onClicked: {
                videoPlayer.position += videoPlayer.position > videoPlayer.duration - 10 ?
                    videoPlayer.duration - videoPlayer.position : 10000
            }
        }

        MediaButton {
            id: showHideVolumeButton
            icon.source: {
                if (volumeSlider.value <= 1 && volumeSlider.value >= 0.6) {
                    return "../assets/images/volume-3.svg"
                } else if (volumeSlider.value < 0.6 && volumeSlider.value >= 0.3) {
                    return "../assets/images/volume-2.svg"
                } else if (volumeSlider.value < 0.3 && volumeSlider.value > 0) {
                    return "../assets/images/volume-1.svg"
                } else {
                    return "../assets/images/volume-muted.svg"
                }
            }

            onHoveredChanged: {
                root.showVolumeSlider = true
                root.volumeSliderVisibilityChanged(true)
            }

            onClicked: {
                volumeSlider.value = (volumeSlider.value > 0) ?
                    0 : root.lastVolumeValue
            }
        }

        Text {
            id: currentPlayingMediaTitle
            text: (videoPlayer.metaData.stringValue(0) === "") ?
                videoPlayer.source : videoPlayer.metaData.stringValue(0)
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            color: root.textColor
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }

        MediaButton {
            id: playNextVideoButton
            icon.source: "../assets/images/nextvideo.svg"
        }

        MediaButton {
            id: showHideVideosControlButton
            icon.source: "../assets/images/controlvideos.svg"
        }

        MediaButton {
            id: subtilesAudioButton
            icon.source: "../assets/images/subtitles.svg"
            onClicked: {
                root.showSubtitlesAndAudioSelector = !root.showSubtitlesAndAudioSelector
                root.subtitlesAndAudioSelectorChanged(root.showSubtitlesAndAudioSelector)
            }
        }

        MediaButton {
            id: playbackSpeedButton
            icon.source: "../assets/images/speed.svg"
            onHoveredChanged: {
                root.showPlaybackSpeedSlider = true
                root.speedSliderVisibilityChanged(true)
            }
        }

        MediaButton {
            id: fullscreenButton
            icon.source: root.isFullscreen ?
                "../assets/images/exitfullscreen.svg" :
                "../assets/images/fullscreen.svg"
            onClicked: {
                root.isFullscreen = !root.isFullscreen
                root.fullscreenChanged(root.isFullscreen)
            }
        }
    }

    // Hidden Slider for volume control
    Slider {
        id: volumeSlider
        visible: false  // This will be controlled by the parent
        value: 1.0
        // Add other necessary properties for the volume slider
    }
}
