/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/

import QtQuick 6.5
import QtQuick.Controls 6.5
import ViewLog
import QtQuick.Layouts 1.0
import QtMultimedia




Rectangle {
    id: mainScreen
    width: Constants.width
    height: Constants.height
    visible: true

    property bool showVolumeSlider: false
    property bool showPlaybackSpeedSlider: false
    property bool showSubtitlesAndAudioSelector: false
    property bool isPictureInPicture: false
    property bool isFullscreen: false
    property bool hideFilesTree: true
    property string elapsedTimeUiText: qsTr("00:00:00")
    property string totalDurationUiText: qsTr("00:00:00")
    property real lastVolumeValue: 1.0
    property bool isPaused: videoPlayer.playbackState === MediaPlayer.PlayingState
    property alias pausePlaybuttonIconsource: pausePlaybutton.icon.source
    property bool isMouseHovered: false
    property int filesTreeWidth: 0
    property int defaultfileTreeWidth: 350
    property string backgroundColor: "black"
    property string treeBackgroundColor: "#1b1b1b"
    property string textColor: "#FFFFF0"
    property string backgroundColorHover: "#0C0C0C"
    property string buttonsColor: "#FFFFF0"
    property string currentlyPlayingfilePath: ""
    property bool showHideDurationMenu: mainScreen.showVolumeSlider || mainScreen.showSubtitlesAndAudioSelector || mainScreen.showPlaybackSpeedSlider || subtilesAudioButton.hovered || playbackSpeedButton.hovered
    property bool hidebuttons: mainWindow.width > 500
    property bool hideFiles: mainWindow.width > 1000

    color: mainScreen.backgroundColor

    Timer {
        id: volumeSliderDelayTimer
        interval: 250  // 500 milliseconds = 0.5 seconds
        repeat: false   // Do not repeat, run only once
        onTriggered: {
            mainScreen.showVolumeSlider = showHideVolumeButton.hovered || volumeSlider.hovered
        }
    }

    Timer {
        id: speedSliderDelayTimer
        interval: 250
        repeat: false
        onTriggered: {
            mainScreen.showPlaybackSpeedSlider = playbackSpeedButton.hovered || speedSlider.hovered
        }
    }

    Rectangle {
        id: treeRectangle
        color: mainScreen.treeBackgroundColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: mainScreen.width - mainScreen.filesTreeWidth
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.rightMargin: 0
        visible: hideFiles

        FolderTreeView {
            id: foldertreeview
            rootFolder: "file:///"
            backgroundColor: mainScreen.treeBackgroundColor
            anchors.fill: parent
        }

        DropArea  {
            id: folderDropArea
            anchors.fill: parent

            onDropped: {
                if (drop.hasUrls) {
                    currentlyPlayingfilePath = drop.urls[0]
                    foldertreeview.rootFolder = currentlyPlayingfilePath
                }
            }
        }
    }

    GroupBox {
        id: videoBox
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        leftPadding: 0
        rightPadding: 0
        bottomPadding: 0
        topPadding: 0
        title: qsTr("")
        anchors.rightMargin: mainScreen.filesTreeWidth
        anchors.leftMargin: 0
        anchors.bottomMargin: 0

        hoverEnabled: true

        background: mainScreen.backgroundColor


        CustomMediaPlayer {
            id: videoPlayer
            videoOutput: videoPlayerOutput

            volumeSlider: volumeSlider.value
            speedSlider: speedSlider.value
        }


        VideoOutput {
            id: videoPlayerOutput
            anchors.fill: parent
            // Enable smooth scaling
            fillMode: VideoOutput.PreserveAspectFit
            
            // Enable hardware acceleration for rendering
            layer.enabled: true
            layer.smooth: true
        }

        // droping a video file and playing it immediately (does not verife if the file is media file)
        DropArea  {
            id: fileDropArea
            anchors.fill: parent

            onDropped: {
                videoPlayer.stop()
                videoPlayer.source = drop.urls[0]
                videoPlayer.play()
            }
        }



        // playing and pausing the video by clicking in the middle of the screen
        MouseArea {
            anchors.fill: parent
            property point lastMousePos
            property real dx
            property real dy

            onClicked: {
                if (dx == 0 && dy == 0) {
                    if (mainScreen.isPaused) {
                        videoPlayer.pause()
                    } else {
                        videoPlayer.play()
                    }

                    // hide the subtitles and audio track selector if open
                    mainScreen.showSubtitlesAndAudioSelector = false
                }
            }
            
            onPressed: {
                lastMousePos = Qt.point(mouseX, mouseY)
            }
            
            onMouseXChanged: {
                dx = mouseX - lastMousePos.x
                mainWindow.x += dx
            }
            
            onMouseYChanged: {
                dy = mouseY - lastMousePos.y
                mainWindow.y += dy
            }
        }


        // show and hide the file tree view
        MediaButton {
            id: hideShowFilesButton
            visible: mainScreen.isMouseHovered && hideFiles
            Layout.topMargin: 30
            anchors.right: parent.right
            topPadding: 0
            bottomPadding: 0
            rightPadding: 0
            leftPadding: 0
            icon.source: if (mainScreen.filesTreeWidth != 0) {
                            "../assets/images/collapse-Tree.png"
                         } else {
                            "../assets/images/show-Tree.png"
                         }
            anchors.rightMargin: 10

            Connections {
                target: hideShowFilesButton
                onClicked: if (mainScreen.filesTreeWidth != 0 && hideFiles) {
                               mainScreen.filesTreeWidth = 0
                           } else {
                               mainScreen.filesTreeWidth = defaultfileTreeWidth
                           }
            }
        }


        // pictureInPicture Button
        MediaButton {
            id: pictureInPictureButton
            anchors.right: parent.right
            y: mainScreen.width/4
            text: qsTr("")
            icon.source: if (mainScreen.isPictureInPicture) {
                            "../assets/images/picture_in_exit_icon.svg"
                        } else {
                            "../assets/images/picture_in_enter_icon.svg"
                        }
            icon.color: mainScreen.buttonsColor
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            flat: true

            Connections {
                target: pictureInPictureButton
                onClicked: {
                    mainScreen.isPictureInPicture = !mainScreen.isPictureInPicture

                
                    if (mainScreen.isPictureInPicture) {
                        mainWindow.flags =  Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
                        mainWindow.width = 1334
                        mainWindow.height = 750
                        mainWindow.x = 100
                        mainWindow.y = 100
                    } else {
                        mainWindow.flags =  mainWindow.flags & ~Qt.WindowStaysOnTopHint & ~Qt.FramelessWindowHint
                    }
                }
            }
        }

        Frame {
            visible: mainScreen.isMouseHovered || !mainScreen.isPaused
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            anchors.bottomMargin: 0




            background: Rectangle {
                    border.width: 0  // No border
                    gradient: Gradient {
                        GradientStop { position: 1.0; color: "#cc000000" }
                        GradientStop { position: 0.0; color: "transparent" }
                    }
               }


            ColumnLayout {
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                anchors.bottomMargin: 15
                anchors.fill: parent


                // on hover options (volume slider, speed slider, subtitles and audio track)
                Rectangle {
                    id: moreControlsBox
                    Layout.fillWidth: true

                    // volume slider
                    Slider {
                        id: volumeSlider
                        hoverEnabled: true
                        visible: mainScreen.showVolumeSlider
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        clip: false
                        bottomPadding: 5
                        rightPadding: 20
                        anchors.leftMargin: showHideVolumeButton.x + (showHideVolumeButton.width/2) - (volumeSlider.width/2)
                        anchors.bottomMargin: 5
                        leftPadding: 20
                        topPadding: 0
                        orientation: Qt.Vertical
                        rotation: 0
                        value: 1

                        width: 200
                        height: 200

                        Connections {
                            target: volumeSlider
                            onHoveredChanged: mainScreen.showVolumeSlider = subtilesAudioButton.hovered || volumeSlider.hovered
                        }


                        onPressedChanged: mainScreen.lastVolumeValue = (volumeSlider.value > 0.1)? volumeSlider.value : 0.1

                    }

                    // next media button
                    GroupBox {
                        id: nextMediaTitleBox
                        width: 200
                        height: 200
                        visible: playNextVideoButton.hovered
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: playNextVideoButton.x + playNextVideoButton.width/2 - nextMediaTitleBox.width/2
                        anchors.bottomMargin: 0
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        title: qsTr("Group Box")
                    }

                    // subtitles and audio tracks selector
                    ColumnLayout {
                        id: subtitlesAndAudioSelector
                        width: 420
                        height: 350
                        visible: mainScreen.showSubtitlesAndAudioSelector
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 25
                        anchors.bottomMargin: 0
                        // Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

                        // background
                        Rectangle {
                           anchors.fill: parent
                           color: "white"  // Background color for the text
                            radius: 5
                            z: -1  // Ensures the background is behind the text
                        }

                        // spliting the box into 2
                        Row {
                            anchors.fill: parent
                            height: parent.height
                            width: childrenRect.width
                            spacing: 10
                            padding: 10

                            // Audio tracks section
                            GroupBox {
                                title: "Audio Tracks"
                                width: parent.width/2 - 15
                                height: 330

                                ScrollView {
                                    height: parent.height
                                    width: parent.width
                                    
                                    Column {
                                        spacing: 5
                                        Repeater {
                                            model: videoPlayer.audioTracks
                                            delegate: RadioButton {
                                                text: modelData.title || `${modelData.stringValue(0)} (${modelData.stringValue(6)})`
                                                checked: videoPlayer.activeAudioTrack === index
                                                onClicked: {
                                                    videoPlayer.activeAudioTrack = index
                                                    mainScreen.showSubtitlesAndAudioSelector = false
                                                }

                                            }
                                        }


                                    }
                                }
                            }

                            // Subtitle tracks section
                            GroupBox {
                                title: "Subtitle Tracks"
                                width: parent.width/2 - 15
                                height: 330

                                ScrollView {
                                    height: parent.height
                                    width: parent.width
    
                                    Column {
                                        spacing: 5
                                        RadioButton {
                                            text: "No subtitles"
                                            checked: true
                                            onClicked: videoPlayer.subtitleTrack = null
                                        }
                                        Repeater {
                                            model: videoPlayer.subtitleTracks
                                            delegate: RadioButton {
                                                text: modelData.title || `${modelData.stringValue(0)} (${modelData.stringValue(6)})`
                                                checked: videoPlayer.subtitleTrack === index
                                                onClicked: {
                                                    videoPlayer.subtitleTrack = index
                                                    mainScreen.showSubtitlesAndAudioSelector = false
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }



                    }

                    // playback speed slider
                    ColumnLayout {
                        id: speedSliderBox
                        x: 854
                        y: -200
                        visible: mainScreen.showPlaybackSpeedSlider
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.rightMargin: 25
                        anchors.bottomMargin: 0

                        Rectangle {
                                    anchors.fill: parent
                                    color: "white"  // Background color for the text
                                    radius: 5
                                    z: -1  // Ensures the background is behind the text
                                }


                        Text {
                            id: textSpeed
                            text: qsTr("Speed:" + (speedSlider.value * 2).toFixed(2))
                            font.pixelSize: 12
                            Layout.leftMargin: 5
                            Layout.rightMargin: 5
                            color: "black"
                        }

                        Slider {
                            id: speedSlider
                            hoverEnabled: true
                            value: 0.5
                            clip: false
                            anchors.bottom: parent.bottom
                            Layout.leftMargin: 5
                            Layout.rightMargin: 5

                            Connections {
                                target: speedSlider
                                onHoveredChanged: mainScreen.showPlaybackSpeedSlider = playbackSpeedButton.hovered || speedSlider.hovered
                            }
                        }

                    }

                }

                // playback information (durationMenu e.i. elapsedTime, durantionSlider, totalDuration)
                DurationMenu {
                    id: durationMenu
                    Layout.fillWidth: true
                    showHideDurationMenu: mainScreen.showHideDurationMenu
                    elapsedTimeUiText: mainScreen.elapsedTimeUiText
                    totalDurationUiText: mainScreen.totalDurationUiText
                    textColor: mainScreen.textColor
                }


                // Media Controls
                RowLayout {
                    Layout.bottomMargin: 10

                    MediaButton {
                        id: pausePlaybutton

                        icon.source: if (mainScreen.isPaused) {
                                        "../assets/images/pause.svg"
                                    } else {
                                        "../assets/images/play.svg"
                                    }


                        Connections {
                            target: pausePlaybutton
                            onClicked: {
                                if (mainScreen.isPaused) {
                                    videoPlayer.pause()
                                } else {
                                    videoPlayer.play()
                                }
                            }

                        }
                    }

                    MediaButton {
                        id: seekBackButton
                        icon.source: "../assets/images/seekback.svg"
                        visible: hidebuttons
                        onClicked: {
                            videoPlayer.position -= videoPlayer.position < 10? videoPlayer.position : 10000
                        }
                    }

                    MediaButton {
                        id: seekForwardButton
                        icon.source: "../assets/images/seekforward.svg"
                        visible: hidebuttons
                        onClicked: {
                            videoPlayer.position += videoPlayer.position > videoPlayer.duration - 10? videoPlayer.duration - videoPlayer.position : 10000
                        }
                    }

                    MediaButton {
                        id: showHideVolumeButton

                        icon.source: if (volumeSlider.value <= 1 && volumeSlider.value >= 0.6) {
                                        "../assets/images/volume-3.svg"
                                    } else if (volumeSlider.value < 0.6 && volumeSlider.value >= 0.3) {
                                        "../assets/images/volume-2.svg"
                                    } else if (volumeSlider.value < 0.3 && volumeSlider.value > 0) {
                                        "../assets/images/volume-1.svg"
                                    } else {
                                        "../assets/images/volume-muted.svg"
                                    }

                        Connections {
                            target: showHideVolumeButton
                            onHoveredChanged: {
                                    mainScreen.showVolumeSlider = true;
                                    volumeSliderDelayTimer.start()
                                }
                        }

                        onClicked: {
                            volumeSlider.value = (volumeSlider.value > 0)? 0 : mainScreen.lastVolumeValue
                        }
                    }
                    

                    // media title 

                    Text {
                        id: currentPlayingMediaTitle
                        font.pixelSize: 18
                        horizontalAlignment: Text.AlignHCenter
                        color: mainScreen.textColor
                        text: (videoPlayer.metaData.stringValue(0) === "")? String(videoPlayer.source).split('/').pop(): videoPlayer.metaData.stringValue(0)
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        maximumLineCount: 1
                        width: 200
                        elide: Text.ElideRight
                    }

                    // next media
                    MediaButton {
                        id: playNextVideoButton
                        icon.source: "../assets/images/nextvideo.svg"
                    }

                    // subtilesAudioButton
                    MediaButton {
                        id: subtilesAudioButton
                        icon.source: "../assets/images/subtitles.svg"
                        visible: mainWindow.width > 470
                        onClicked: {
                            mainScreen.showSubtitlesAndAudioSelector = !mainScreen.showSubtitlesAndAudioSelector
                        }
                    }

                    MediaButton {
                        id: playbackSpeedButton
                        icon.source: "../assets/images/speed.svg"

                        Connections {
                            target: playbackSpeedButton
                            onHoveredChanged: {
                                    mainScreen.showPlaybackSpeedSlider = true;
                                    speedSliderDelayTimer.start()
                                }
                        }
                    }

                    MediaButton {
                        id: fullscreenButton
                        icon.source: if (!mainScreen.isFullscreen) {
                                        "../assets/images/fullscreen.svg"
                                    } else {
                                        "../assets/images/exitfullscreen.svg"
                                    }

                        Connections {
                            target: fullscreenButton
                            onClicked: mainScreen.isFullscreen = !mainScreen.isFullscreen
                        }
                    }

                }

            }

        }

        Connections {
            target: videoBox
            onHoveredChanged: mainScreen.isMouseHovered = videoBox.hovered

        }
    }


}
