import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts 6.5
import QtMultimedia 6.5

MediaPlayer {
    id: videoPlayer
    property real volumeSlider: 0
    property real speedSlider: 0
    
    source: "../assets/test.mkv"
    audioOutput: AudioOutput {
        volume: volumeSlider
    }
    
    // Handle errors
    onErrorChanged: {
        if (error !== MediaPlayer.NoError) {
            console.error("Media Player Error:", errorString)
        }
    }

    // Handle playback state changes
    onPlaybackStateChanged: {
        if (playbackState === MediaPlayer.StoppedState) {
            source = "" // Clear source when stopped
        }
    }

    // Handle subtitle tracks
    onSubtitleTracksChanged: {
        console.log("videoPlayer.activeSubtitleTrack")

        console.log(videoPlayer.activeSubtitleTrack)
    }

    


    playbackRate: speedSlider * 2

    property int totalDurationHours: 0
    property int totalDurationMinutes: 0
    property int totalDurationSeconds: 0
    property int elapsedHours: 0
    property int elapsedMinutes: 0
    property int elapsedSeconds: 0

    onDurationChanged: {
        totalDurationHours = Math.floor((videoPlayer.duration / (6000)*60)%60)
        totalDurationMinutes = Math.floor(videoPlayer.duration / 60000)
        totalDurationSeconds = Math.floor((videoPlayer.duration / 1000)%60)
        mainScreen.totalDurationUiText = qsTr((totalDurationHours == "00:00:00"? (totalDurationHours < 10? "0" + totalDurationHours : totalDurationHours) : "")
                                                +
                                                (totalDurationMinutes < 10? "0" + totalDurationMinutes : totalDurationMinutes)
                                                + ":" + 
                                                (totalDurationSeconds < 10? "0" + totalDurationSeconds : totalDurationSeconds)
                                            )
    }

    onPositionChanged: {
        elapsedHours = Math.floor((videoPlayer.position / (6000)*60)%60)
        elapsedMinutes = Math.floor(videoPlayer.position / 60000);
        elapsedSeconds = Math.floor(videoPlayer.position / 1000 % 60);
        mainScreen.elapsedTimeUiText = qsTr((elapsedHours == "00:00:00"? (elapsedHours < 10? "0" + elapsedHours : elapsedHours) : "")
                                                +
                                                (elapsedMinutes < 10? "0" + elapsedMinutes : elapsedMinutes)
                                                + ":" + 
                                                (elapsedSeconds < 10? "0" + elapsedSeconds : elapsedSeconds)
                                            )
    }


    onMetaDataChanged:  {
        if (mediaStatus === MediaPlayer.LoadedMedia) {
            console.log("Media loaded")
            console.log("Has audio tracks:", audioTracks !== undefined)
            console.log("Number of audio tracks:", audioTracks ? audioTracks.length : 0)
            if (audioTracks) {
                for (let i = 0; i < audioTracks.length; i++) {
                    console.log(`Track ${i}:`, audioTracks[i].language, audioTracks[i].title)
                }
            }
        }
    }
}
