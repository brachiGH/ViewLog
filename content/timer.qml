Timer {
    id: delayTimer
    interval: 500  // 500 milliseconds = 0.5 seconds
    repeat: false   // Do not repeat, run only once
    onTriggered: {
        mainScreen.showVolumeSlider = showHideVolumeButton.hovered || volumeSlider.hovered
        console.log("2 seconds delay completed!")
        // Your delayed action goes here
    }
}